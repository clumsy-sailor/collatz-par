#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <omp.h>
#include "collatz.h"

uint_fast64_t collatz_next(uint_fast64_t n) {
    return n % 2 == 0 ? n / 2 : 3 * n + 1;
}

uint_fast64_t collatz_seq_length(uint_fast64_t n) {
    uint_fast64_t seq_length = 0;
    while (n != 1) {
        n = collatz_next(n);
        ++seq_length;
    }
    return seq_length;
}

// Input:
// n_begin: start of the interval where the longest contiguous sequence is being searched.
// n_end: end of the interval where the longest contiguous sequence is being searched.
// Output:
// n_min: smallest number, within interval, from where the longest contiguous sequence starts.
// longest_contig_seq_length: length of the longest contiguous sequence starts.
void collatz_longest_contig_seq(uint_fast64_t n_begin, uint_fast64_t n_end,
                                uint_fast64_t& n_min, uint_fast64_t& longest_contig_seq_length) {
    n_min = 0;
    longest_contig_seq_length = 0;
    uint_fast64_t contig_seq_length = 1; 
    bool inside_subseq = false;
    uint_fast64_t length1 = 1;
    uint_fast64_t length2;
    for (uint_fast64_t n = n_begin; n < n_end; ++n) {
        length2 = collatz_seq_length(n);
        if (length2 == length1) { // Start of contiguous sequence identified.
            inside_subseq = true;
            ++contig_seq_length;
            length1 = length2;
            continue;
        }
        else {
            if (inside_subseq) { // End of contiguous sequence identified.
                inside_subseq = false;
                if (contig_seq_length > longest_contig_seq_length) {
                    longest_contig_seq_length = contig_seq_length;
                    n_min = n - contig_seq_length;
                }
                contig_seq_length = 1;
            }
        }
        length1 = length2;
    }
}

Pair collatz_longest_contig_seq_par(uint_fast64_t n_begin, uint_fast64_t n_end) {
    Pair result{0, 0};
    collatz_longest_contig_seq(n_begin, n_end, result.n_min, result.longest_contig_seq_length);
    return result;
}

void runner_par(uint_fast64_t n_begin, uint_fast64_t n_end) {
    Pair result{0, 0};
    auto t_start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        Pair thread_result;
        uint_fast64_t id = uint_fast64_t(omp_get_thread_num());
        // Share evenly the workload among thread team.
        uint_fast64_t k = (n_end - n_begin) / omp_get_num_threads();
        thread_result = collatz_longest_contig_seq_par(n_begin + id * k, n_begin + (id + 1) * k);

        #pragma omp critical
        result = max_pair(result, thread_result);
    }
    auto t_stop = std::chrono::high_resolution_clock::now();
    auto et = t_stop - t_start;
    collatz_save(n_begin, n_end, result.n_min, result.longest_contig_seq_length, et);
    std::cout << "OK\n";
}

Pair max_pair(Pair res1, Pair res2) {
    return (res1.longest_contig_seq_length >= res2.longest_contig_seq_length) ? res1 : res2;
}

void collatz_save(uint_fast64_t n_begin,
                  uint_fast64_t n_end,
                  uint_fast64_t n_min,
                  uint_fast64_t longest_contig_seq_length,
                  const std::chrono::nanoseconds& et) {
    auto time_now = std::chrono::system_clock::now();
    auto time_now_time_t = std::chrono::system_clock::to_time_t(time_now);
    auto et_msec = std::chrono::duration_cast<std::chrono::milliseconds>(et);
    std::stringstream ss;
    ss << "collatz_" << std::put_time(std::localtime(&time_now_time_t), "%Y-%m-%d_%H:%M:%S") << ".txt";
    std::ofstream of;
    of.open(ss.str());
    of.imbue(std::locale(""));
    of << n_begin << " ... " << n_end << '\n';
    of <<  n_min << '\t' << longest_contig_seq_length << '\n';
    of << et_msec.count() / 1000.0 << '\n';
    of.close();
}