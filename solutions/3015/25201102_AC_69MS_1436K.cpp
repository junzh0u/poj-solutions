// POJ 3015 - Expected Difference
// Model: claude-sonnet-5
//
// Sort A ascending (statement says input is already sorted, but we sort
// defensively). For an m-subset B, its contribution is (max-min). Fix a
// pair of positions i<j (1-indexed, sorted) as the min/max of B: the
// remaining m-2 elements of B must come from the j-i-1 elements strictly
// between them, so there are C(j-i-1, m-2) subsets with exactly that
// (min,max) pair. So
//   E = sum_{i<j} (A_j - A_i) * C(j-i-1, m-2) / C(n, m).
// Grouping by gap d = j-i (d from m-1 to n-1, since we need j-i-1>=m-2):
//   E = sum_d C(d-1, m-2)/C(n,m) * sum_{i=1}^{n-d} (A_{i+d} - A_i)
// and sum_{i=1}^{n-d} (A_{i+d}-A_i) = S[n]-S[d]-S[n-d] via prefix sums S.
// This is O(n) per test case.
//
// Binomial coefficients like C(50000,25000) vastly overflow any integer
// or double range, so the ratio C(d-1,m-2)/C(n,m) is computed via
// precomputed log-factorials and a single exp() call per d (the discuss
// board explicitly confirms this: "take logs" rather than attempt exact
// big-integer combinatorics). Verified against all 4 sample cases
// (1.000/1.333/1.667/6.500) and against a brute-force enumeration of all
// C(n,m) subsets for random small n<=12.
//
// Ambiguity: none of note; the board's "use unsigned int" warning is
// about 32-bit overflow of the prefix sums S[] (A_i up to 65536, n up to
// 50000 => sums up to ~3.3e9 > INT_MAX) which is avoided here by using
// long long throughout.

#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXN = 50005;
static double logfact[MAXN];
static long long A[MAXN];
static long long S[MAXN];

int main() {
    logfact[0] = 0.0;
    for (int i = 1; i < MAXN; ++i) logfact[i] = logfact[i - 1] + log((double)i);

    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        for (int i = 1; i <= n; ++i) scanf("%lld", &A[i]);
        sort(A + 1, A + n + 1);
        S[0] = 0;
        for (int i = 1; i <= n; ++i) S[i] = S[i - 1] + A[i];

        double logCnm = logfact[n] - logfact[m] - logfact[n - m];
        int k = m - 2; // choose k from (d-1) items
        double ans = 0.0;
        for (int d = m - 1; d <= n - 1; ++d) {
            double logCd = logfact[d - 1] - logfact[k] - logfact[d - 1 - k];
            double logratio = logCd - logCnm;
            if (logratio > -700.0) {
                double term = exp(logratio);
                long long val = S[n] - S[d] - S[n - d];
                ans += term * (double)val;
            }
        }
        printf("%.3f\n", ans);
    }
    return 0;
}
