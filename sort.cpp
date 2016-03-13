#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "ssssort.h"
#include "Timer.h"

int main(int argc, char *argv[]) {
    size_t size = 1024*1024;
    size_t *data = new size_t[size],
        *out = new size_t[size];

    Timer timer;
    for (size_t i = 0; i < size; ++i) {
        data[i] = i;
    }
    std::mt19937 rng{ std::random_device{}() };
    std::shuffle(data, data + size, rng);
    double t_generate = timer.get_and_reset();

    ssssort(data, data + size, out, false);
    double t_ssssort = timer.get_and_reset();


    bool incorrect = false;
    for (size_t i = 0; i < size; ++i) {
        incorrect |= (out[i] != i);
        if (out[i] != i) {
            std::cout << "Err at pos " << i << ": got " << out[i] << std::endl;
            std::cout << *(int*)(0);
        }
    }

    double t_verify = timer.get_and_reset();

    std::sort(data, data + size);
    double t_stdsort = timer.get_and_reset();

    std::cout << "RESULT size=" << size
              << " t_ssssort=" << t_ssssort
              << " t_stdsort=" << t_stdsort
              << " t_generate=" << t_generate
              << " t_verify=" << t_verify
              << " correct=" << !incorrect
              << std::endl;

    return 0;
}
