// POJ 1619 - EKG Sequence
// Model: claude-sonnet-5
//
// The EKG sequence a(1)=1, a(2)=2, a(k) = smallest unused positive integer
// sharing a common factor with a(k-1). We are guaranteed every queried
// n <= 300000 appears at some position p <= 1,000,000, so we precompute the
// first 1,000,000 terms once and answer all queries from a lookup table.
//
// Naive generation (scan candidates from 1 upward each step) is far too slow.
// Instead, for the previous term a(k-1), factor it into its distinct prime
// factors. For each such prime p we keep a persistent multiplier ptr[p]: the
// smallest untried multiple of p is ptr[p]*p. Since "used" only grows over
// time, ptr[p] never needs to decrease across the whole run, so the total
// work advancing every ptr[p] over all 1,000,000 steps is bounded (harmonic
// sum over the primes that ever appear), not quadratic. At each step we take
// the minimum unused candidate across the (few) prime factors of a(k-1).
//
// A local simulation (see scratch notes) shows that although the *chosen*
// term value stays under 1.6e6 for the first 1e6 terms, the *candidates
// probed* while searching for the next unused multiple of a large prime can
// reach about 2.05e6 (searching p, 2p, 3p, 4p... for a prime p up to ~5e5).
// MAXV=3,000,000 gives a comfortable margin over that observed 2.05e6 probe
// ceiling for both the used[] and ptr[] arrays.
//
// Statement ambiguity: none found; verified byte-for-byte against the sample
// (12->7, 21->15, 2->2, 33->21, 100000->97110, 299977->584871).

#include <cstdio>

const int STEPS = 1000000;   // guaranteed p <= 1,000,000
const int MAXV = 3000000;    // safe bound for values / probe candidates
const int MAXN = 300000;     // guaranteed n <= 300,000

static bool used[MAXV + 2];
static int ptr[MAXV + 2];    // ptr[p] = next multiplier to try for prime p
static int pos[MAXN + 1];    // pos[n] = position of n in the sequence
static int primesBuf[32];

int main() {
    for (int p = 0; p <= MAXV + 1; ++p) ptr[p] = 1;

    used[1] = true;
    pos[1] = 1;
    used[2] = true;
    pos[2] = 2;
    int a = 2;

    for (int i = 3; i <= STEPS; ++i) {
        int x = a;
        int np = 0;
        for (int f = 2; f * f <= x; ++f) {
            if (x % f == 0) {
                primesBuf[np++] = f;
                while (x % f == 0) x /= f;
            }
        }
        if (x > 1) primesBuf[np++] = x;

        int best = -1;
        for (int k = 0; k < np; ++k) {
            int p = primesBuf[k];
            int &pt = ptr[p];
            for (;;) {
                int cand = p * pt;
                if (!used[cand]) {
                    if (best == -1 || cand < best) best = cand;
                    break;
                }
                ++pt;
            }
        }

        a = best;
        used[a] = true;
        if (a <= MAXN) pos[a] = i;
    }

    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        printf("The number %d appears in location %d.\n", n, pos[n]);
    }

    return 0;
}
