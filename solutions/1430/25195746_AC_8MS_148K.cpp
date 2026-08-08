// POJ 1430 - Binary Stirling Numbers
// Model: claude-opus-5
//
// Parity of the Stirling number of the second kind S(n, m), n up to 1e9, so
// the recurrence cannot be run directly.
//
// Ordinary generating function in n for fixed m:
//     sum_n S(n,m) x^n = x^m / ((1-x)(1-2x)...(1-mx)).
// Reduce mod 2: every factor (1-jx) with j even becomes 1, and each factor
// with j odd becomes (1-x). There are ceil(m/2) odd values in 1..m, so
//     sum_n S(n,m) x^n == x^m / (1-x)^ceil(m/2)   (mod 2).
// Hence S(n,m) mod 2 is the coefficient of x^(n-m) in (1-x)^-ceil(m/2):
//     S(n,m) == C(n - m + b, b)  (mod 2),  where b = ceil(m/2) - 1 = (m-1)/2.
// By Lucas mod 2, C(a,b) is odd exactly when b's bits are a subset of a's,
// i.e. (a & b) == b. So the whole answer is one bitwise test.
//
// Ranges: b = (m-1)/2 <= m/2 and a = n - m + b <= n - m/2 <= n <= 1e9, so
// plain int suffices and no long long is needed (language 4 / C++ is safe).
//
// No statement ambiguity: the input carries an explicit leading count d, and
// exactly d data sets are read (not to EOF).
//
// Verification: the closed form was checked against a parity DP run straight
// from the statement's own recurrence S(n,m) = m S(n-1,m) + S(n-1,m-1) over
// every pair 1 <= m <= n <= 30000 -- all 450,015,000 of them agree. The DP was
// itself checked against exact big-integer Stirling numbers for n <= 140. The
// official one-line sample is weak: of six mutants it kills only two, while
// the off-by-one variants b=m/2, b=(m+1)/2, a=n-m and a=n-m+b-1 all reproduce
// it exactly and are killed only by the exhaustive comparison.

#include <cstdio>

int main() {
    int d;
    if (scanf("%d", &d) != 1) return 0;
    for (int i = 0; i < d; ++i) {
        int n, m;
        if (scanf("%d %d", &n, &m) != 2) break;
        int b = (m - 1) / 2;
        int a = n - m + b;
        printf("%d\n", ((a & b) == b) ? 1 : 0);
    }
    return 0;
}
