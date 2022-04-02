#include <iostream>
#include <chrono>
#include "collatz.h"

int main() {
    uint_fast64_t n_begin = 10'000'000'000;
    uint_fast64_t n_end =  100'000'000'000;

    runner_par(n_begin, n_end);

    return 0;
}