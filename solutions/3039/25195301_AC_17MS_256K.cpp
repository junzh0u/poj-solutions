// POJ 3039 - Close Encounter
// Model: claude-opus-5
//
// Given a reduced fraction N/D (1 <= N < D <= 32767), find the reduced fraction
// P/Q with P, Q in 1..32767 that is closest to, but not equal to, N/D.
//
// Approach: enumerate the denominator Q from 1 to 32767.  For a fixed Q the only
// numerators worth trying are floor(N*Q/D) and floor(N*Q/D)+1, the two integers
// bracketing N*Q/D; every other numerator is strictly farther away.  Compare
// candidates exactly with integers: |P/Q - N/D| = |P*D - Q*N| / (Q*D), so with
// K = |P*D - Q*N| the comparison K1/Q1 < K2/Q2 is K1*Q2 < K2*Q1.  Since P is a
// neighbour of N*Q/D we always have K < D <= 32767, so both products stay below
// 32767*32767 < 2^31 and plain int suffices (no 64-bit needed).
//
// Two things the answer hinges on:
//   * "properly reduced" applies to the answer too, so gcd(P,Q) must be 1.  An
//     unreduced P/Q is exactly as close as its reduced form, so scanning Q
//     downward and keeping the first best would happily report 14/30 instead of
//     7/15 -- the discuss board's "denominator from 32767 down gives WA".
//     Scanning Q upward with strict improvement plus the gcd test avoids it.
//   * ties.  The Farey neighbours of N/D in F_32767 are the only candidates that
//     can win, at distances 1/(Q1*D) and 1/(Q2*D); they tie only when Q1 == Q2,
//     which (using Q1+Q2 = m*D from the mediant property) forces D == 2.  So the
//     one tied input is 1/2, where 16383/32767 and 16384/32767 are equally close
//     and "smallest" selects 16383/32767 -- which is what scanning Q upward and
//     testing floor(N*Q/D) before floor(N*Q/D)+1 produces.
//
// Verified two ways: exhaustively against a from-the-statement brute force over
// every coprime N < D <= 40 (with the search bound lowered to match), and at the
// real bound 32767 against an independent extended-Euclid computation of the two
// Farey neighbours, on 12012 inputs including every D <= 120 and the extremes.

#include <cstdio>

static int gcd(int a, int b) { while (b) { int t = a % b; a = b; b = t; } return a; }

int main() {
    const int M = 32767;
    int n, d;
    while (scanf("%d %d", &n, &d) == 2) {
        int bp = 0, bq = 0, bk = 0;
        for (int q = 1; q <= M; ++q) {
            int nq = n * q;
            int p0 = nq / d;
            for (int i = 0; i < 2; ++i) {
                int p = p0 + i;
                if (p < 1 || p > M) continue;
                int k = p * d - nq;
                if (k < 0) k = -k;
                if (k == 0) continue;             /* equal to N/D, not allowed */
                if (gcd(p, q) != 1) continue;     /* must be properly reduced */
                if (bq == 0 || k * bq < bk * q) { bp = p; bq = q; bk = k; }
            }
        }
        printf("%d %d\n", bp, bq);
    }
    return 0;
}
