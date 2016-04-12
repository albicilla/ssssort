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
#include <fstream>
#include <iostream>
#include <random>

#include "benchmark.h"

// Change this to some other integral type to test other data types
using data_t = int;

int main(int argc, char *argv[]) {
    if (argc > 1 && std::string{argv[1]} == "-h") {
        std::cout << "Usage: " << argv[0]
                  << " [iteratons] [statistics output file]" << std::endl
                  << "Defaults are 10 iterations and output to stats.txt"
                  << std::endl;
        return 0;
    }

    std::cout << "This benchmark suite writes output for SqlPlotTools to allow "
              << "for easy plotting." << std::endl << "Grab a copy at "
              << "https://github.com/bingmann/sqlplot-tools, point it to "
              << "speed.plot and run gnuplot on it!" << std::endl;

    // Parse flags
    size_t iterations = 10;
    if (argc > 1) iterations = atoi(argv[1]);

    std::string stat_file = "stats.txt";
    if (argc > 2) stat_file = std::string{argv[2]};
    std::ofstream *stat_stream = nullptr;
    if (stat_file != "-") {
        stat_stream = new std::ofstream;
        stat_stream->open(stat_file);
    }


    // Run Benchmarks
    benchmark_generator<data_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            std::mt19937 rng{ std::random_device{}() };
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(rng());
            }
        }, "random", iterations, stat_stream);


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

    benchmark_generator<data_t>(nearly_sorted_gen(5), "80pcsorted", iterations, stat_stream);
    benchmark_generator<data_t>(nearly_sorted_gen(10), "90pcsorted", iterations, stat_stream);
    benchmark_generator<data_t>(nearly_sorted_gen(100), "99pcsorted", iterations, stat_stream);
    benchmark_generator<data_t>(nearly_sorted_gen(1000), "99.9pcsorted", iterations, stat_stream);


    // nearly sorted data generator factory
    auto unsorted_tail_gen = [](size_t rfrac) {
        return [rfrac=rfrac](auto data, size_t size) {
            using T = std::remove_reference_t<decltype(*data)>;
            std::mt19937 rng{ std::random_device{}() };
            // fill with sorted data, using entire range of RNG
            size_t ordered_max = size - (size / rfrac);
            T factor = static_cast<T>(static_cast<double>(rng.max()) / ordered_max);
            for (size_t i = 0; i < ordered_max; ++i) {
                data[i] = i * factor;
            }
            // set 1/rfrac of the items to random values
            for (size_t i = ordered_max; i < size; ++i) {
                data[i] = static_cast<T>(rng());
            }
        };
    };

    benchmark_generator<data_t>(unsorted_tail_gen(10), "tail90", iterations, stat_stream);
    benchmark_generator<data_t>(unsorted_tail_gen(100), "tail99", iterations, stat_stream);


    benchmark_generator<data_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(i);
            }
        }, "sorted", iterations, stat_stream);


    benchmark_generator<data_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(size - i);
            }
        }, "reverse", iterations, stat_stream);


    // Benchmark due to Armin Weiß at Universität Stuttgart
    benchmark_generator<data_t>([](auto data, size_t size) {
            using T = std::remove_reference_t<decltype(*data)>;
            size_t flogn = 0, s = size;
            while (s >>= 1) ++flogn; // floor(log2(n))

            for (size_t i = 0; i < size; ++i) {
                size_t j = i;
                j *= j; j *= j; j *= j; j *= j;
                data[i] = static_cast<T>(j % flogn);
            }
        }, "many-dupes", iterations, stat_stream);

    benchmark_generator<data_t>([](auto data, size_t size){
            for (size_t i = 0; i < size; ++i) {
                data[i] = 1;
            }
        }, "ones", iterations, stat_stream);


    if (stat_stream != nullptr) {
        stat_stream->close();
        delete stat_stream;
    }
}
