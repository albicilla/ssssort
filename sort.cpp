/*******************************************************************************
 * sort.cpp
 *
 * Test runner
 *
 *******************************************************************************
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


#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "ssssort.h"
#include "timer.h"


int main(int argc, char *argv[]) {
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
    ssssort(data, data + size, out, false);

    double t_ssssort = 0.0;
    for (size_t it = 0; it < iterations; ++it) {
        // reset data and timer
        memcpy(data, copy, size * sizeof(size_t));
        std::fill(out, out+size, 0);
        timer.reset();

        ssssort(data, data + size, out, false);

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
    bool incorrect = false;
    for (size_t i = 0; i < size; ++i) {
        incorrect |= (out[i] != data[i]);
    }
    double t_verify = timer.get_and_reset();

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
