/*******************************************************************************
 * benchmark.h
 *
 * Benchmark utilities
 *
 *******************************************************************************
 * Copyright (C) 2016 Lorenz Hübschle-Schneider <lorenz@4z2.de>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#pragma once

const bool debug = false;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>

#include "ssssort.h"
#include "timer.h"
#include "progress_bar.h"

struct statistics {
    // Single-pass standard deviation calculation as described in Donald Knuth:
    // The Art of Computer Programming, Volume 2, Chapter 4.2.2, Equations 15&16
    double mean;
    double nvar; // approx n * variance; stddev = sqrt(nvar / (count-1))
    size_t count;

    statistics() : mean(0.0), nvar(0.0), count(0) {}

    void push(double t) {
        ++count;
        if (count == 1) {
            mean = t;
        } else {
            double oldmean = mean;
            mean += (t - oldmean) / count;
            nvar += (t - oldmean) * (t - mean);
        }
    }

    double avg() {
        return mean;
    }
    double stddev() {
        assert(count > 1);
        return sqrt(nvar / (count - 1));
    }
};

template <typename T, typename Sorter>
void run(T* data, const T* const copy, T* out, size_t size, Sorter sorter,
         size_t iterations, statistics& stats, progress_bar &bar,
         bool reset_out = true) {
    // warmup
    sorter(data, out, size);
    ++bar;

    Timer timer;
    for (size_t it = 0; it < iterations; ++it) {
        // reset data and timer
        std::copy(copy, copy+size, data);
        if (reset_out)
            memset(out, 0, size * sizeof(T));
        timer.reset();

        sorter(data, out, size);

        stats.push(timer.get());
        ++bar;
    }
}

template <typename T, typename Generator>
size_t benchmark(size_t size, Generator generator,  const std::string &name,
                 size_t outer_its, size_t inner_its,
                 std::ofstream *stat_stream, bool deterministic_gen = false) {
    T *data = new T[size],
        *out = new T[size],
        *copy = new T[size];

    // Number of iterations
    if (outer_its == static_cast<size_t>(-1)) {
        if (deterministic_gen) {
            // deterministic is boring
            outer_its = 1;
            if (inner_its == static_cast<size_t>(-1)) {
                if (size < (1<<14)) inner_its = 1000;
                else if (size < (1<<16)) inner_its = 500;
                else if (size < (1<<18)) inner_its = 250;
                else inner_its = 100;
            }
        } else {
            if (size < (1<<16)) outer_its = 100;
            else if (size < (1<<18)) outer_its = 50;
            else if (size < (1<<22)) outer_its = 35;
            else outer_its = 25;
        }
    }
    if (inner_its == static_cast<size_t>(-1)) {
        inner_its = 10;
    }

    // the label maker
    auto bar_label = [&](size_t it) {
        return name + " (" + std::to_string(it + 1) + "/" +
            std::to_string(outer_its) + "): ";
    };

    progress_bar bar(2 * outer_its * (inner_its + 1), bar_label(0));
    Timer timer;

    double t_generate(0.0), t_verify(0.0);
    bool incorrect = false;
    statistics t_ssssort, t_stdsort;
    for (size_t it = 0; it < outer_its; ++it) {
        bar.set_extra(bar_label(it));
        // Generate random numbers as input
        timer.reset();
        size = generator(data, size);

        // create a copy to be able to sort it multiple times
        std::copy(data, data+size, copy);
        t_generate += timer.get_and_reset();

        // Sorting algorithms have their own time tracking
        // 1. Super Scalar Sample Sort
        run(data, copy, out, size,
            [](T* data, T* out, size_t size)
            { ssssort::ssssort(data, data + size, out); },
            inner_its, t_ssssort, bar);

        // 2. std::sort
        run(data, copy, out, size,
            [](T* data, T* /*ignored*/, size_t size)
            { std::sort(data, data + size); },
            inner_its, t_stdsort, bar, false);


        // verify
        timer.reset();
        bool it_incorrect = !std::is_sorted(out, out + size);
        if (it_incorrect) {
            std::cerr << "Output data isn't sorted" << std::endl;
        }
        for (size_t i = 0; i < size; ++i) {
            it_incorrect |= (out[i] != data[i]);
            if (debug && out[i] != data[i]) {
                std::cerr << "Err at pos " << i << " expected " << data[i]
                          << " got " << out[i] << std::endl;
            }
        }
        incorrect |= it_incorrect;
        t_verify += timer.get_and_reset();

    }

    bar.undraw();

    delete[] out;
    delete[] data;
    delete[] copy;

    std::stringstream output;
    output << "RESULT algo=ssssort"
           << " name=" << name
           << " size=" << size
           << " iters=" << outer_its << "*" << inner_its
           << " time=" << t_ssssort.avg()
           << " stddev=" << t_ssssort.stddev()
           << " t_gen=" << t_generate
           << " t_check=" << t_verify
           << " ok=" << !incorrect
           << std::endl
           << "RESULT algo=stdsort"
           << " name=" << name
           << " size=" << size
           << " iters=" << outer_its << "*" << inner_its
           << " time=" << t_stdsort.avg()
           << " stddev=" << t_stdsort.stddev()
           << " t_gen=" << t_generate
           << " t_check=0"
           << " ok=1"
           << std::endl;
    auto result_str = output.str();
    std::cout << result_str;
    if (stat_stream != nullptr)
        *stat_stream << result_str << std::flush;

    return size;
}

template <typename T, typename Generator>
void benchmark_generator(Generator generator, const std::string &name,
                         const size_t outer_its, const size_t inner_its,
                         std::ofstream *stat_stream,
                         bool deterministic_gen = false,
                         const size_t max_log_size = 27) {
    auto wrapped_generator = [generator](T* data, size_t size) {
        generator(data, size);
        return size;
    };

    // warmup
    benchmark<T>(1<<10, wrapped_generator, "warmup", 1, 10, nullptr);

    for (size_t log_size = 10; log_size < max_log_size; ++log_size) {
        size_t size = 1 << log_size;
        benchmark<T>(size, wrapped_generator, name, outer_its, inner_its,
                     stat_stream, deterministic_gen);
    }
}


template <typename T, typename Generator>
void sized_benchmark_generator(Generator generator, const std::string &name,
                               const size_t outer_its, const size_t inner_its,
                               std::ofstream *stat_stream,
                               bool deterministic_gen = false,
                               const size_t max_log_size = 27) {
    // warmup
    benchmark<T>(1<<10, generator, "warmup", 1, 10, nullptr);

    for (size_t log_size = 10; log_size < max_log_size; ++log_size) {
        size_t size = 1 << log_size;
        size_t last_size = benchmark<T>(
            size, generator, name, outer_its, inner_its,
            stat_stream, deterministic_gen);
        if (last_size < size) break;
    }
}
