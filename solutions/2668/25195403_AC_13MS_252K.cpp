// POJ 2668 - Defending Castle
// Model: claude-opus-5
//
// The K-th throw does D/K damage rounded up, so the answer is
//   A(D,N) = sum_{k=1..N} ceil(D/k).
// For D >= 1, ceil(D/k) = floor((D-1)/k) + 1, which turns this into
//   A(D,N) = N + S(M, N),   S(M,L) = sum_{k=1..L} floor(M/k),   M = D-1.
// D = 1 gives M = 0 and the answer N, correct since ceil(1/k) = 1 for all k.
//
// S(M,L) is computed by the hyperbola split rather than by walking divisor
// blocks, because the block walk costs two divisions per block over ~2*sqrt(M)
// blocks while this costs one division per step over ~2*sqrt(M) steps -- half
// the work, and 64-bit division is the entire running time here.  With
// s = floor(sqrt(M)):
//   * L <= s: sum the L terms directly.
//   * L >  s: the terms k <= s are summed directly, and the tail is counted by
//     value instead of by index,
//         sum_{k=s+1..L} floor(M/k) = sum_{q=1..floor(M/(s+1))} (min(L, floor(M/q)) - s),
//     which is exact because every k <= s already has floor(M/k) >= floor(M/s)
//     >= floor(M/(s+1)) >= q, so #{k <= L : floor(M/k) >= q} = min(L, floor(M/q))
//     and exactly s of those k lie in [1, s].
//
// All integer, no floating point in the arithmetic (the sqrt seed is corrected
// by exact integer comparisons).  D, N <= 2e9 fit a 32-bit signed int; the
// total is bounded by D*(ln N + 1) + N < 5e10 and needs a 64-bit accumulator.
// Printed via std::cout because POJ's C++ compiler has no reliable %lld.
// Checked against the problem author's own board answer: D = N = 2000000000
// gives 45141689303.
//
// Input framing: multiple test cases, one line each, terminated by "0 0"
// (EOF also stops), one output line per case.

#include <cmath>
#include <cstdio>
#include <iostream>

// sum_{k=1..L} floor(M/k), for M >= 1 and L >= 1.
static long long divisor_sum(long long M, long long L) {
    long long s = (long long)std::sqrt((double)M);
    while (s > 1 && s * s > M) --s;
    while ((s + 1) * (s + 1) <= M) ++s;

    long long head = L < s ? L : s;
    long long r = 0;
    for (long long k = 1; k <= head; ++k) r += M / k;
    if (L <= s) return r;

    long long qmax = M / (s + 1);
    for (long long q = 1; q <= qmax; ++q) {
        long long t = M / q;
        if (t > L) t = L;
        r += t - s;
    }
    return r;
}

int main() {
    int d, n;
    while (scanf("%d %d", &d, &n) == 2) {
        if (d == 0 && n == 0) break;
        // D is promised positive; guard anyway so a stray 0 prints 0 rather
        // than falling through the M = D-1 < 0 case below.
        if (d == 0) { std::cout << 0 << "\n"; continue; }

        long long m = (long long)d - 1;
        long long total = (long long)n;
        if (m >= 1 && n >= 1) total += divisor_sum(m, (long long)n);

        std::cout << total << "\n";
    }
    return 0;
}
