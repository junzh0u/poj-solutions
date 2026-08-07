// POJ 3511 - Fermat's Christmas Theorem
// Model: claude-opus-5
//
// For each query [L, U] print "L U x y": x = number of primes in the range,
// y = number of those primes expressible as a sum of two squares.
//
// Approach: sieve once up to 1,000,000 and build two prefix-count arrays --
// primes, and primes that are a sum of two squares.  By Fermat's theorem an
// odd prime is a sum of two squares iff p % 4 == 1; the prime 2 = 1^2 + 1^2
// is the extra case the theorem's "odd prime" wording leaves out.  Each query
// is then two array lookups.
//
// Statement traps (all confirmed against the discuss board):
//  * L and U may be negative; the input terminates only when BOTH are -1, so
//    a line like "-1 5" is a real query.  Clamp the range to [1, 1000000]
//    before indexing, but echo the ORIGINAL L and U in the output.
//  * There are no negative primes, so a negative L contributes nothing.
//  * 2 counts toward y even though the quoted theorem speaks of odd primes.

#include <cstdio>

const int MAXN = 1000000;

static bool composite[MAXN + 1];
static int cntPrime[MAXN + 1];
static int cntSumSq[MAXN + 1];

int main() {
    composite[0] = composite[1] = true;
    for (int i = 2; (long)i * i <= (long)MAXN; ++i) {
        if (!composite[i]) {
            for (int j = i * i; j <= MAXN; j += i) composite[j] = true;
        }
    }

    cntPrime[0] = 0;
    cntSumSq[0] = 0;
    for (int i = 1; i <= MAXN; ++i) {
        int p = composite[i] ? 0 : 1;
        int s = 0;
        if (p && (i == 2 || i % 4 == 1)) s = 1;
        cntPrime[i] = cntPrime[i - 1] + p;
        cntSumSq[i] = cntSumSq[i - 1] + s;
    }

    int L, U;
    while (scanf("%d %d", &L, &U) == 2) {
        if (L == -1 && U == -1) break;
        int lo = L, hi = U;
        if (lo < 1) lo = 1;
        if (hi > MAXN) hi = MAXN;
        int x = 0, y = 0;
        if (hi >= lo) {
            x = cntPrime[hi] - cntPrime[lo - 1];
            y = cntSumSq[hi] - cntSumSq[lo - 1];
        }
        printf("%d %d %d %d\n", L, U, x, y);
    }
    return 0;
}
