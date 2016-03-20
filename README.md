# ssssort — Super Scalar Sample Sort

This is an implementation of [Super Scalar Sample Sort](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.72.366&rep=rep1&type=pdf) in modern C++.  It is faster than `std::sort` in many cases, but uses more memory (approximately n\*sizeof(T) + n\*sizeof(unsigned) + 4√n \* sizeof(size_t), vs logarithmic additional memory for in-place quicksort).

## Benchmarks

TODO; initial benchmark (sorting 1 million random 64-bit integers) showed a speedup between 35 % on a Sandy Bridge laptop and 43 % on a Haswell Desktop vs `std::sort`.  Planned benchmarks include nearly sorted inputs and inputs with many duplicates.  Various input sizes should be considered (2^10 to 2^30 elements?)

## Implementation

The implementation is fairly close to the paper, but uses `std::sort` as base case for sorting less than 1024 elements.  As-is the code technically requires a C++14 compiler, even though `g++` is happy to compile it with `-std=c++11`.  The requirement stems from the use of a variable declaration in the `find_bucket` function, which is marked `constexpr`.  You can simply replace `constexpr` with `inline` to make it valid C++11.
