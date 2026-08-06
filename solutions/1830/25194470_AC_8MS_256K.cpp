// POJ 1830 - 开关问题 (Switch Problem)
//
// N switches (0 < N < 29) start in a given state and must reach a target
// state. Operating switch i flips switch i itself, plus every switch j
// listed in a pair "i j". Each switch may be operated at most once, and
// order does not matter, so a plan is just a subset of switches: x_i in
// {0,1}. Count the plans that reach the target.
//
// This is a linear system over GF(2). Let a[j][i] = 1 iff operating i
// flips j — that is the identity (a switch always flips itself) OR-ed
// with the listed pairs. The equation for switch j is
//     XOR_i a[j][i] * x_i = start[j] XOR target[j].
// Gaussian elimination over GF(2), each row held as one unsigned int
// with bits 0..n-1 the coefficients and bit n the right-hand side, so a
// row operation is a single XOR. After full reduction, any row past the
// rank that is still nonzero has all-zero coefficients and a 1 on the
// right — the system is inconsistent, print the impossible message.
// Otherwise every one of the n-rank free variables can be chosen freely,
// giving 2^(n-rank) plans. n <= 28 keeps that inside a 32-bit int.
//
// Statement ambiguities resolved:
//  - "operating a switch changes the state of the switches linked to it"
//    also changes the operated switch itself; without that implicit
//    diagonal the first sample would not give 4.
//  - The pair "I J" is directed: operating I affects J, so it sets the
//    coefficient of x_I in equation J, not the other way round.
//  - Switches with no listed links stay determined (identity row), so an
//    unconstrained-looking input is not automatically free.
//  - Duplicate pairs are set, not toggled (|=), since they state that a
//    link exists rather than adding a second flip.
#include <cstdio>

int main() {
    int K;
    if (scanf("%d", &K) != 1) return 0;
    while (K-- > 0) {
        int n;
        scanf("%d", &n);
        static int st[64], ed[64];
        int i;
        for (i = 0; i < n; ++i) scanf("%d", &st[i]);
        for (i = 0; i < n; ++i) scanf("%d", &ed[i]);
        /* row[r] : bits 0..n-1 are coefficients, bit n is RHS */
        static unsigned int row[64];
        for (i = 0; i < n; ++i)
            row[i] = (1u << i) | (((unsigned int)(st[i] ^ ed[i])) << n);
        int a, b;
        while (scanf("%d %d", &a, &b) == 2 && (a != 0 || b != 0)) {
            /* operating switch a flips switch b : coefficient of x_a in eq b */
            row[b - 1] |= (1u << (a - 1));
        }
        int rank = 0;
        bool ok = true;
        for (int col = 0; col < n; ++col) {
            int piv = -1;
            for (i = rank; i < n; ++i)
                if (row[i] >> col & 1u) { piv = i; break; }
            if (piv < 0) continue;
            unsigned int t = row[piv]; row[piv] = row[rank]; row[rank] = t;
            for (i = 0; i < n; ++i)
                if (i != rank && (row[i] >> col & 1u)) row[i] ^= row[rank];
            ++rank;
        }
        for (i = rank; i < n; ++i)
            if (row[i] != 0) { ok = false; break; }
        if (!ok) {
            printf("Oh,it's impossible~!!\n");
        } else {
            printf("%d\n", 1 << (n - rank));
        }
    }
    return 0;
}
