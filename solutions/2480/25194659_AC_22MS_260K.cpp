// POJ 2480 - Longge's problem
// Model: claude-sonnet-5
// Approach: sum_{i=1}^{N} gcd(i,N) = sum_{d|N} d*phi(N/d), which is
// multiplicative. For a prime power p^e the sub-sum equals
// p^e + e*p^(e-1)*(p-1) (derived by expanding phi(p^m)=p^m-p^(m-1)).
// So factorize N by trial division up to sqrt(N) (N < 2^31, so
// sqrt(N) < 46341), and multiply the per-prime-power contributions
// together. Input is multiple test cases, one N per line, until EOF.
// No ambiguity in the statement; sample (N=2 -> 3, N=6 -> 15) matches
// the formula directly.
#include <cstdio>

int main() {
    long long n;
    while (scanf("%lld", &n) == 1) {
        long long m = n;
        long long ans = 1;
        for (long long p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                int e = 0;
                long long pe = 1;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                    pe *= p;
                }
                long long contrib = pe + (long long)e * (pe / p) * (p - 1);
                ans *= contrib;
            }
        }
        if (m > 1) {
            // remaining prime factor to the power 1
            long long contrib = m + 1 * 1 * (m - 1);
            ans *= contrib;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
