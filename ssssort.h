/*******************************************************************************
 * ssssort.h
 *
 * Super Scalar Sample Sort
 *
 *******************************************************************************
 * Copyright (C) 2014 Timo Bingmann <tb@panthema.net>
 * Copyright (C) 2016 Lorenz Hübschle-Schneider <lorenz@4z2.de>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#pragma once

#include <cmath>
#include <cstring>

constexpr size_t logBuckets = 8;
constexpr size_t numBuckets = 1 << logBuckets;

inline size_t oversampling_factor(size_t n) {
    double r = std::sqrt(double(n)/(2*numBuckets*(logBuckets+4)));
    return std::max(static_cast<size_t>(r), 1UL);
}

// Random number generation engine for sampling.  You can swap this out for
// std::minstd_rand if the Mersenne Twister is too slow on your hardware.
// It's only minimally slower on mine (Haswell i7-4790T).
static std::mt19937 gen{std::random_device{}()};

// Draw a random sample without replacement using the Fisher-Yates Shuffle.
// This reorders the input somewhat but the sorting does that anyway.
template <typename Iterator,
          typename value_type = typename std::iterator_traits<Iterator>::value_type>
void draw_sample_fisheryates(Iterator begin, Iterator end,
                             value_type* samples, size_t sample_size)
{
    // Random generator
    size_t max = end - begin;
    assert(gen.max() >= max);

    for (size_t i = 0; i < sample_size; ++i) {
        size_t index = gen() % max; // biased, don't care
        std::swap(*(begin + index), *(begin + max));
        samples[i] = *(begin + max);
        max--;
    }
}


// Draw a random sample with replacement by generating random indices. On my
// machine this results in measurably slower sorting than a Fisher-Yates-based
// sample, so beware the apparent simplicity.
template <typename Iterator,
          typename value_type = typename std::iterator_traits<Iterator>::value_type>
void draw_sample_simplerand(Iterator begin, Iterator end,
                             value_type* samples, size_t sample_size)
{
    // Random generator
    size_t size = end - begin;
    assert(gen.max() >= size);

    for (size_t i = 0; i < sample_size; ++i) {
        size_t index = gen() % size; // biased, don't care
        samples[i] = *(begin + index);
    }
}


// A completely non-random sample that's beyond terrible on sorted inputs
template <typename Iterator,
          typename value_type = typename std::iterator_traits<Iterator>::value_type>
void draw_sample_first(Iterator begin, Iterator end,
                       value_type *samples, size_t sample_size) {
    for (size_t i = 0; i < sample_size; ++i) {
        samples[i] = *(begin + i);
    }
}

template <typename Iterator,
          typename value_type = typename std::iterator_traits<Iterator>::value_type>
void draw_sample(Iterator begin, Iterator end,
                 value_type *samples, size_t sample_size)
{
    draw_sample_fisheryates(begin, end, samples, sample_size);
}


template <typename Iterator, size_t treebits = logBuckets,
          typename value_type = typename std::iterator_traits<Iterator>::value_type>
struct Classifier {
    const size_t num_splitters = (1 << treebits) - 1;
    const size_t splitters_size = 1 << treebits;
    value_type splitters[1 << treebits];

    unsigned int* const bktout;
    size_t* const bktsize;

    Classifier(const value_type *samples, const size_t sample_size,
               unsigned int* const bktout)
        : bktout(bktout)
        , bktsize(new size_t[1 << treebits])
    {
        std::fill(bktsize, bktsize + (1 << treebits), 0);
        build_recursive(samples, samples + sample_size, 1);
    }

    ~Classifier() {
        delete[] bktsize;
    }

    void build_recursive(const value_type* lo, const value_type* hi, unsigned int pos) {
        const value_type *mid = lo + (ssize_t)(hi - lo)/2;
        splitters[pos] = *mid;

        if (2 * pos < num_splitters) {
            build_recursive(lo, mid, 2*pos);
            build_recursive(mid + 1, hi , 2*pos + 1);
        }
    }

    constexpr unsigned int step(unsigned int i, const value_type &key) {
        return 2*i + (key > splitters[i]);
    }

    constexpr unsigned int find_bucket(const value_type &key) {
        unsigned int i = 1;
        while (i <= num_splitters) i = step(i, key);
        return (i - splitters_size);
    }

    template <int U>
    inline void find_bucket_unroll(const value_type *key, unsigned int *obkt)
    {
        unsigned int i[U];
        for (int u = 0; u < U; ++u) i[u] = 1;

        for (size_t l = 0; l < treebits; ++l) {
            // step on all U keys
            for (int u = 0; u < U; ++u) i[u] = step(i[u], key[u]);
        }
        for (int u = 0; u < U; ++u) {
            unsigned int bucket = i[u] - splitters_size;
            obkt[u] = bucket;
            bktsize[bucket]++;
        }
    }

    // classify all elements by walking tree and saving bucket id
    inline void classify(Iterator begin, Iterator end, unsigned int* bktout = nullptr)  {
        if (bktout == nullptr) bktout = this->bktout;
        for (Iterator it = begin; it != end;) {
            unsigned int bucket = find_bucket(*it++);
            *bktout++ = bucket;
            bktsize[bucket]++;
        }
    }

    template <int U>
    inline void
    classify_unroll(Iterator begin, Iterator end) {
        unsigned int* bktout = this->bktout;
        value_type key[U];
        Iterator it = begin;
        for (; it + U < end; it += U, bktout += U) {
            for (int u = 0; u < U; ++u) key[u] = *(it+u);
            find_bucket_unroll<U>(key, bktout);
        }
        // process remainder
        classify(it, end, bktout);
    }

    template <int U>
    inline void
    distribute(Iterator in_begin, Iterator in_end, Iterator out_begin)
    {
        // exclusive prefix sum
        for (size_t i = 0, sum = 0; i < numBuckets; ++i) {
            size_t curr_size = bktsize[i];
            bktsize[i] = sum;
            sum += curr_size;
        }
        const size_t n = in_end - in_begin;
        size_t i;
        for (i = 0; i + U < n; i += U) {
            for (int u = 0; u < U; ++u) {
                *(out_begin + bktsize[bktout[i+u]]++) = *(in_begin + i + u);
            }
        }
        // process the rest
        for (; i < n; ++i) {
            *(out_begin + bktsize[bktout[i]]++) = *(in_begin + i);
        }
    }

};

template <typename Iterator, typename value_type = typename std::iterator_traits<Iterator>::value_type>
void ssssort_int(Iterator begin, Iterator end, Iterator out_begin,
                 unsigned int *bktout, bool begin_is_home) {
    const size_t n = end - begin;

    // draw and sort sample
    const size_t sample_size = oversampling_factor(n) * numBuckets;
    value_type *samples = new value_type[sample_size];
    draw_sample(begin, end, samples, sample_size);
    std::sort(samples, samples + sample_size);

    // classify elements
    Classifier<Iterator, logBuckets, value_type> classifier(samples, sample_size, bktout);
    delete[] samples;
    classifier.template classify_unroll<4>(begin, end);
    classifier.template distribute<4>(begin, end, out_begin);

    size_t offset = 0;
    for (size_t i = 0; i < numBuckets; ++i) {
        auto size = classifier.bktsize[i] - offset;
        if (size == 0) continue; // empty bucket
        if (size <= 1024) {
            // small bucket
            std::sort(out_begin + offset, out_begin + classifier.bktsize[i]);
            if (begin_is_home) {
                // uneven recursion level, we have to move the result
                memcpy(begin + offset, out_begin + offset, size*sizeof(value_type));
            }
        } else {
            ssssort_int(out_begin + offset,
                        out_begin + classifier.bktsize[i], // = out_begin + offset + size
                        begin + offset,
                        bktout + offset,
                        !begin_is_home);
        }
        offset += size;
    }
}

template <typename Iterator, typename value_type = typename std::iterator_traits<Iterator>::value_type>
void ssssort(Iterator begin, Iterator end, Iterator out_begin) {
    size_t n = end - begin;
    if (n < 1024) {
        // base case
        std::sort(begin, end);
        return;
    }

    unsigned int *bktout = new unsigned int[n];
    ssssort_int(begin, end, out_begin, bktout, false);
    delete[] bktout;
}

template <typename Iterator, typename value_type = typename std::iterator_traits<Iterator>::value_type>
void ssssort(Iterator begin, Iterator end) {
    const size_t n = end - begin;

    if (n < 1024) {
        // base case
        std::sort(begin, end);
        return;
    }

    value_type* out = new value_type[n];
    unsigned int *bktout = new unsigned int[n];
    ssssort_int(begin, end, out, bktout, true);
    delete[] bktout;
    delete[] out;
}
