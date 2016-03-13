#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "ssssort.h"
#include "Timer.h"


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
