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
#include <iostream>
#include <random>

#include "ssssort.h"
#include "timer.h"

const bool debug = false;

int main() {
    size_t size = 1024*1024;
    size_t iterations = 10;

    size_t *data = new size_t[size],
        *out = new size_t[size],
        *copy = new size_t[size];

    Timer timer;
    // Generate random numbers as input
    std::mt19937 rng{ std::random_device{}() };
    for (size_t i = 0; i < size; ++i) {
        data[i] = rng();
    }

    // create a copy to be able to sort it multiple times
    memcpy(copy, data, size * sizeof(size_t));
    double t_generate = timer.get_and_reset();

    // 1. Super Scalar Sample Sort
    // warmup
    ssssort(data, data + size, out);

    double t_ssssort = 0.0;
    for (size_t it = 0; it < iterations; ++it) {
        // reset data and timer
        memcpy(data, copy, size * sizeof(size_t));
        std::fill(out, out+size, 0);
        timer.reset();

        ssssort(data, data + size, out);

        t_ssssort += timer.get();
    }
    timer.reset();

    // 2. std::sort
    // warmup
    memcpy(data, copy, size * sizeof(size_t));
    std::sort(data, data + size);

    double t_stdsort = 0.0;
    for (size_t it = 0; it < iterations; ++it) {
        memcpy(data, copy, size * sizeof(size_t));

        timer.reset();
        std::sort(data, data + size);
        t_stdsort += timer.get();
    }
    timer.reset();

    // verify
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

    std::cout << "RESULT size=" << size
              << " iterations=" << iterations
              << " t_ssssort=" << t_ssssort/iterations
              << " t_stdsort=" << t_stdsort/iterations
              << " t_generate=" << t_generate
              << " t_verify=" << t_verify
              << " correct=" << !incorrect
              << std::endl;

    return 0;
}
