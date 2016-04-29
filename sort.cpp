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
                  << " [outer iteratons] [inner iterations]"
                  << " [statistics output file]" << std::endl
                  << "Defaults are 5 outer iteration, 3 inner iterations,"
                  << " and output to stats.txt" << std::endl;
        return 0;
    }

    std::cout << "This benchmark suite writes output for SqlPlotTools to allow "
              << "for easy plotting." << std::endl << "Grab a copy at "
              << "https://github.com/bingmann/sqlplot-tools, point it to "
              << "speed.plot and run gnuplot on it!" << std::endl;

    // Parse flags
    size_t outer_its = 5, inner_its = 3;
    if (argc > 1) outer_its = static_cast<size_t>(atol(argv[1]));
    if (argc > 2) inner_its = static_cast<size_t>(atol(argv[2]));

    std::string stat_file = "stats.txt";
    if (argc > 3) stat_file = std::string{argv[3]};
    std::ofstream *stat_stream = nullptr;
    if (stat_file != "-") {
        stat_stream = new std::ofstream;
        stat_stream->open(stat_file);
    }

    auto random_gen = [](auto data, size_t size){
        using T = std::remove_reference_t<decltype(*data)>;
        std::mt19937 rng{ std::random_device{}() };
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<T>(rng());
        }
    };

    // Warmup
    benchmark_generator<data_t>(random_gen, "warmup", 1, 3, stat_stream, 20);


    // Run Benchmarks
    benchmark_generator<data_t>(random_gen, "random", outer_its, inner_its,
                                stat_stream);


    // nearly sorted data generator factory
    auto nearly_sorted_gen = [](size_t rfrac) {
        return [rfrac=rfrac](auto data, size_t size) {
            using T = std::remove_reference_t<decltype(*data)>;
            std::mt19937 rng{ std::random_device{}() };
            // fill with sorted data, using entire range of RNG
            size_t factor = static_cast<size_t>(static_cast<double>(rng.max()) / size);
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(i * factor);
            }
            // set 1/rfrac of the items to random values
            for (size_t i = 0; i < size/rfrac; ++i) {
                data[rng() % size] = static_cast<T>(rng());
            }
        };
    };

    benchmark_generator<data_t>(nearly_sorted_gen(5), "80pcsorted",
                                outer_its, inner_its, stat_stream);
    benchmark_generator<data_t>(nearly_sorted_gen(10), "90pcsorted",
                                outer_its, inner_its, stat_stream);
    benchmark_generator<data_t>(nearly_sorted_gen(100), "99pcsorted",
                                outer_its, inner_its, stat_stream);
    benchmark_generator<data_t>(nearly_sorted_gen(1000), "99.9pcsorted",
                                outer_its, inner_its, stat_stream);


    // nearly sorted data generator factory
    auto unsorted_tail_gen = [](size_t rfrac) {
        return [rfrac=rfrac](auto data, size_t size) {
            using T = std::remove_reference_t<decltype(*data)>;
            std::mt19937 rng{ std::random_device{}() };
            // fill with sorted data, using entire range of RNG
            size_t ordered_max = size - (size / rfrac);
            size_t factor = static_cast<size_t>(static_cast<double>(rng.max()) / ordered_max);
            for (size_t i = 0; i < ordered_max; ++i) {
                data[i] = static_cast<T>(i * factor);
            }
            // set 1/rfrac of the items to random values
            for (size_t i = ordered_max; i < size; ++i) {
                data[i] = static_cast<T>(rng());
            }
        };
    };

    benchmark_generator<data_t>(unsorted_tail_gen(10), "tail90",
                                outer_its, inner_its, stat_stream);
    benchmark_generator<data_t>(unsorted_tail_gen(100), "tail99",
                                outer_its, inner_its, stat_stream);


    benchmark_generator<data_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(i);
            }
        }, "sorted", outer_its, inner_its, stat_stream, true);


    benchmark_generator<data_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            for (size_t i = 0; i < size; ++i) {
                data[i] = static_cast<T>(size - i);
            }
        }, "reverse", outer_its, inner_its, stat_stream, true);


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
        }, "many-dupes", outer_its, inner_its, stat_stream, true);


    /* Benchmark due to Armin Weiß at Universität Stuttgart
     *
     * This is an interesting case because the distribution has few very large
     * spikes and lots of elements around them. Thus the buckets aren't
     * all-equal, and without a break on big buckets, it would recurse a lot.
     */
    benchmark_generator<data_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;

            uint64_t prev_pow_2 = 1;
            while (2 * prev_pow_2 <= size) { prev_pow_2 *= 2; }
            const size_t offset_zw = prev_pow_2 / 2;

            for (size_t i = 0; i < size; i++) {
                uint64_t temp = (i*i) % prev_pow_2;
                temp = (temp*temp) % prev_pow_2;
                data[i] = static_cast<T>(
                    (offset_zw + temp*temp) % prev_pow_2);
            }
        }, "few-spikes-with-noise", outer_its, inner_its, stat_stream, true);


    benchmark_generator<data_t>([](auto data, size_t size){
            for (size_t i = 0; i < size; ++i) {
                data[i] = 1;
            }
        }, "ones", outer_its, inner_its, stat_stream, true);


    if (stat_stream != nullptr) {
        stat_stream->close();
        delete stat_stream;
    }
}
