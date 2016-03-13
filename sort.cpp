#include <algorithm>
#include <cassert>
#include <random>

#include "ssssort.h"

int main(int argc, char *argv[]) {
    size_t size = 1024*1024;
    size_t *data = new size_t[size],
        *out = new size_t[size];

    for (size_t i = 0; i < size; ++i) {
        data[i] = i;
    }
    std::mt19937 rng{ std::random_device{}() };
    std::shuffle(data, data + size, rng);

    ssssort(data, data + size, out);

    for (size_t i = 0; i < size; ++i) {
        assert(out[i] == i);
    }

    return 0;
}
