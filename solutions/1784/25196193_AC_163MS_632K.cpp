// POJ 1784 - Huffman's Greed
// Model: claude-sonnet-5
// Approach: classic optimal binary search tree DP. Read n keys' frequencies
// p1..pn (internal-node hit probabilities, as raw integer frequencies) then
// q0..qn (miss/gap frequencies), per the statement's stated input order.
// Interleave them into a single array B[0..2n] = q0,p1,q1,p2,q2,...,pn,qn so
// that the subtree weight w(i,j) = qi + sum_{k=i+1..j}(pk+qk) becomes a
// contiguous range sum B[2i..2j], computed via prefix sums in O(1). Then the
// textbook O(n^3) interval DP dp[i][j] = min_{i<r<=j} dp[i][r-1]+dp[r][j] +
// w(i,j) finds the optimal tree cost over keys K_{i+1..j}; dp is done purely
// in integer frequency units, so dp[0][n] already equals cost*s exactly
// (verified against sample: n=2, freqs 20 15 15 25 25 -> 160). n<=200 makes
// n^3=8e6 operations, comfortably under the 1s limit even without the
// standard Knuth root-monotonicity speedup.
// Ambiguity: statement doesn't spell out the frequency order explicitly
// beyond "in this order"; confirmed p1..pn then q0..qn by matching the
// first sample case by hand.
#include <cstdio>
#include <cstring>
using namespace std;

static long long B[405];
static long long S[405]; // prefix sums, S[m] = sum_{t=0}^{m} B[t]
static long long dp[201][201];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        long long p[201];
        long long q[201];
        for (int i = 1; i <= n; ++i) scanf("%lld", &p[i]);
        for (int i = 0; i <= n; ++i) scanf("%lld", &q[i]);

        // Interleave: B[2k] = q[k] for k=0..n, B[2k-1] = p[k] for k=1..n
        int m = 2 * n; // last index
        for (int k = 0; k <= n; ++k) B[2 * k] = q[k];
        for (int k = 1; k <= n; ++k) B[2 * k - 1] = p[k];
        S[0] = B[0];
        for (int t = 1; t <= m; ++t) S[t] = S[t - 1] + B[t];

        // w(i,j) for 0<=i<=j<=n : sum B[2i..2j]
        // = S[2j] - (i==0 ? 0 : S[2i-1])

        for (int i = 0; i <= n; ++i) dp[i][i] = 0;

        for (int len = 1; len <= n; ++len) {
            for (int i = 0; i + len <= n; ++i) {
                int j = i + len;
                long long w = S[2 * j] - (i == 0 ? 0 : S[2 * i - 1]);
                long long best = -1;
                for (int r = i + 1; r <= j; ++r) {
                    long long cost = dp[i][r - 1] + dp[r][j];
                    if (best == -1 || cost < best) best = cost;
                }
                dp[i][j] = best + w;
            }
        }

        printf("%lld\n", dp[0][n]);
    }
    return 0;
}
