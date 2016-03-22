/*******************************************************************************
 * sort.cpp
 *
 * Test runner
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


#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>

#include "ssssort.h"
#include "timer.h"

const bool debug = false;

template <typename T, typename Sorter>
double run(T* data, const T* const copy, T* out, size_t size, Sorter sorter,
           size_t iterations, bool reset_out = true) {
    // warmup
    sorter(data, out, size);

    double time = 0.0;
    Timer timer;
    for (size_t it = 0; it < iterations; ++it) {
        // reset data and timer
        memcpy(data, copy, size * sizeof(T));
        if (reset_out)
            memset(out, 0, size * sizeof(T));
        timer.reset();

        sorter(data, out, size);

        time += timer.get();
    }
    return time;
}

template <typename T, typename Generator>
void benchmark(size_t size, size_t iterations, Generator generator,
               const std::string &name) {
    T *data = new T[size],
        *out = new T[size],
        *copy = new T[size];

    Timer timer;
    // Generate random numbers as input
    generator(data, size);

    // create a copy to be able to sort it multiple times
    memcpy(copy, data, size * sizeof(T));
    double t_generate = timer.get_and_reset();

    // 1. Super Scalar Sample Sort
    double t_ssssort = run(data, copy, out, size,
                           [](T* data, T* out, size_t size)
                           { ssssort(data, data + size, out); },
                           iterations);

    // 2. std::sort
    double t_stdsort = run(data, copy, out, size,
                           [](T* data, T* /*ignored*/, size_t size)
                           { std::sort(data, data + size); },
                           iterations, false);


    // verify
    timer.reset();
    bool incorrect = !std::is_sorted(out, out + size);
    if (incorrect) {
        std::cerr << "Output data isn't sorted" << std::endl;
    }
    for (size_t i = 0; i < size; ++i) {
        incorrect |= (out[i] != data[i]);
        if (debug && out[i] != data[i]) {
            std::cerr << "Err at pos " << i << " expected " << data[i]
                      << " got " << out[i] << std::endl;
        }
    }
    double t_verify = timer.get_and_reset();

    delete[] out;
    delete[] data;
    delete[] copy;

    std::cout << "RESULT algo=ssssort"
              << " name=" << name
              << " size=" << size
              << " iterations=" << iterations
              << " time=" << t_ssssort/iterations
              << " t_generate=" << t_generate
              << " t_verify=" << t_verify
              << " correct=" << !incorrect
              << std::endl
              << "RESULT algo=stdsort"
              << " name=" << name
              << " size=" << size
              << " iterations=" << iterations
              << " time=" << t_stdsort/iterations
              << " t_generate=" << t_generate
              << " t_verify=0"
              << " correct=1"
              << std::endl;

}

template <typename T, typename Generator>
void benchmark_generator(Generator generator, const std::string &name,
                         size_t iterations = 10) {
    for (size_t log_size = 10; log_size < 27; ++log_size) {
        size_t size = 1 << log_size;
        benchmark<T>(size, iterations, generator, name);
    }
}

int main(int argc, char *argv[]) {
    size_t iterations = 10;

    if (argc > 1) iterations = atoi(argv[1]);

    benchmark_generator<size_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            std::mt19937 rng{ std::random_device{}() };
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(rng());
            }
        }, "random", iterations);


    // nearly sorted data generator factory
    auto nearly_sorted_gen = [](size_t rfrac) {
        return [rfrac=rfrac](auto data, size_t size) {
            using T = std::remove_reference_t<decltype(*data)>;
            std::mt19937 rng{ std::random_device{}() };
            // fill with sorted data, using entire range of RNG
            T factor = static_cast<T>(static_cast<double>(rng.max()) / size);
            for (size_t i = 0; i < size; ++i) {
                data[i] = i * factor;
            }
            // set 1/rfrac of the items to random values
            for (size_t i = 0; i < size/rfrac; ++i) {
                data[rng() % size] = static_cast<T>(rng());
            }
        };
    };

    benchmark_generator<size_t>(nearly_sorted_gen(10), "90pcsorted", iterations);
    benchmark_generator<size_t>(nearly_sorted_gen(100), "99pcsorted", iterations);

    benchmark_generator<size_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(i);
            }
        }, "sorted", iterations);


    benchmark_generator<size_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(size - i);
            }
        }, "reverse", iterations);

}
