#pragma once

#include <cstdint>
#include <chrono>

struct Pair {
    uint_fast64_t n_min;
    uint_fast64_t longest_contig_seq_length;
};

uint_fast64_t collatz_next(uint_fast64_t n);

uint_fast64_t collatz_seq_length(uint_fast64_t n);

void collatz_longest_contig_seq(uint_fast64_t n_begin,
                                uint_fast64_t n_end,
                                uint_fast64_t& n_min,
                                uint_fast64_t& longest_contig_seq_length);

Pair collatz_longest_contig_seq_pair(uint_fast64_t n_begin, uint_fast64_t n_end);

void runner_par1(uint_fast64_t n_begin, uint_fast64_t n_end);
void runner_par2(uint_fast64_t n_begin, uint_fast64_t n_end);

Pair max_pair(Pair res1, Pair res2);

void collatz_save(uint_fast64_t n_begin,
                  uint_fast64_t n_end,
                  uint_fast64_t n_min,
                  uint_fast64_t longest_contig_seq_length,
                  const std::chrono::nanoseconds& et);

