// POJ 2773 - Happy 2006
//
// For a given m, print the K-th positive integer coprime to m (1 <= m <= 1e6,
// 1 <= K <= 1e8), one query per input line until EOF.
//
// Approach: count-and-binary-search rather than enumerating a period.
// Factor m by trial division (at most 7 distinct primes, since
// 2*3*5*7*11*13*17 = 510510 > 1e6), build the signed squarefree divisors of
// rad(m) with their Moebius sign, and get
//     count(x) = #{ 1 <= i <= x : gcd(i,m) = 1 } = sum_d mu(d) * floor(x/d)
// by inclusion-exclusion over at most 2^7 = 128 divisors. The answer is the
// smallest x with count(x) >= K, found by binary search; such an x is coprime
// to m by construction, so no boundary correction is needed -- this sidesteps
// the usual off-by-one of the "divide K by phi(m), index into one period"
// formulation, where a remainder of 0 means the last element of the previous
// block.
//
// Upper bound for the search: with phi = count(m), x = (K/phi + 1)*m satisfies
// count(x) = (K/phi + 1)*phi >= K.
//
// Ranges: m = 1 is legal, has no prime factors, and makes count(x) = x, so the
// answer is simply K. The largest answer overall is about 5.6e8 (worst ratio
// m/phi(m) ~ 5.2 at m = 510510), which still fits in a 32-bit int, so I/O uses
// %d and only the intermediate counting is done in long long -- this also
// avoids POJ's %lld-vs-%I64d compiler split.
//
// Accepted: Run ID 25194488, G++, 9MS, 256K, 1437B.
#include <cstdio>

int primes[8];
int np;
long long divs[256];
int ndiv;

/* number of integers in [1,x] coprime to m */
static long long countCoprime(long long x)
{
    long long s = 0;
    int i;
    for (i = 0; i < ndiv; ++i) {
        long long d = divs[i] < 0 ? -divs[i] : divs[i];
        long long q = x / d;
        if (divs[i] < 0) s -= q; else s += q;
    }
    return s;
}

int main()
{
    int m, k;
    while (scanf("%d %d", &m, &k) == 2) {
        long long K = k;
        int t = m, i, j;
        np = 0;
        for (i = 2; (long long)i * i <= t; ++i) {
            if (t % i == 0) {
                primes[np++] = i;
                while (t % i == 0) t /= i;
            }
        }
        if (t > 1) primes[np++] = t;

        /* signed squarefree divisors: sign = mobius */
        divs[0] = 1;
        ndiv = 1;
        for (i = 0; i < np; ++i) {
            int cur = ndiv;
            for (j = 0; j < cur; ++j) {
                long long d = divs[j] < 0 ? -divs[j] : divs[j];
                long long nd = d * primes[i];
                divs[ndiv++] = divs[j] < 0 ? nd : -nd;
            }
        }

        long long phi = countCoprime(m);
        long long lo = 1, hi = (K / phi + 1) * (long long)m;
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (countCoprime(mid) >= K) hi = mid; else lo = mid + 1;
        }
        printf("%d\n", (int)lo);
    }
    return 0;
}
