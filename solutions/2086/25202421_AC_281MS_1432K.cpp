// POJ 2086 - Land Division Tax
// Model: claude-sonnet-5
//
// N lots arranged on a circle. Repeatedly split a contiguous piece into two
// contiguous sub-pieces until every lot is its own piece; each split costs
// F * max(areaOfPart1, areaOfPart2). Minimize total tax.
//
// This is the classic circular interval-DP: double the array to length 2N,
// let dp[i][j] (j-i < N) be the minimum tax to reduce the contiguous run of
// lots i..j to singletons via the same splitting rule. dp[i][i] = 0 and
// dp[i][j] = min over split k of dp[i][k] + dp[k+1][j] + F*max(sum(i,k),
// sum(k+1,j)). The very first division of the whole circle corresponds to
// picking where to "cut it open" into a length-N interval, so the answer is
// min over i in [1,N] of dp[i][i+N-1].
//
// F has exactly two decimal digits, so it is read and scaled to an integer
// number of cents (F100). All tax accumulation is then done in exact integer
// (long long) arithmetic representing tax*100, avoiding any floating point
// rounding across the up to ~199 additions per test case; only the final
// print divides by 100.
//
// Statement ambiguity: none found beyond the worked example in the text,
// which was used to validate the DP by hand before coding. The discuss board
// (message 10148 etc.) confirms the intended solution is plain O(N^3) DP and
// warns that Knuth's quadrangle-inequality speedup does NOT apply here (the
// cost function does not satisfy the quadrangle inequality), so no such
// optimization was attempted.

#include <cstdio>
#include <algorithm>
using namespace std;

static const int MAXN = 200;
static long long dp[2 * MAXN + 5][2 * MAXN + 5];
static long long S[2 * MAXN + 5];
static int X[2 * MAXN + 5];

int main() {
    int n;
    double f;
    while (scanf("%d %lf", &n, &f) == 2) {
        if (n == 0 && f == 0.0) break;
        long long f100 = (long long)(f * 100.0 + 0.5);
        for (int i = 1; i <= n; i++) scanf("%d", &X[i]);
        for (int i = 1; i <= n; i++) X[i + n] = X[i];
        int m = 2 * n;
        S[0] = 0;
        for (int i = 1; i <= m; i++) S[i] = S[i - 1] + X[i];

        for (int i = 1; i <= m; i++) dp[i][i] = 0;

        for (int len = 2; len <= n; len++) {
            for (int i = 1; i + len - 1 <= m; i++) {
                int j = i + len - 1;
                long long best = -1;
                for (int k = i; k < j; k++) {
                    long long left = S[k] - S[i - 1];
                    long long right = S[j] - S[k];
                    long long mx = left > right ? left : right;
                    long long cand = dp[i][k] + dp[k + 1][j] + f100 * mx;
                    if (best < 0 || cand < best) best = cand;
                }
                dp[i][j] = best;
            }
        }

        long long ans = -1;
        if (n == 1) {
            ans = 0;
        } else {
            for (int i = 1; i <= n; i++) {
                long long v = dp[i][i + n - 1];
                if (ans < 0 || v < ans) ans = v;
            }
        }

        printf("%lld.%02lld\n", ans / 100, ans % 100);
    }
    return 0;
}
