#include <iostream>
#include <chrono>
#include "collatz.h"

int main() {
    uint_fast64_t n_begin = 1'000'000'000;
    uint_fast64_t n_end =  10'000'000'000;

    //runner_par1(n_begin, n_end);
    runner_par2(n_begin, n_end);

    return 0;
}