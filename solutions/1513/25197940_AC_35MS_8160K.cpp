// POJ 1513 - Scheduling Lectures
// Model: claude-sonnet-5
//
// Approach: n topics (durations t_1..t_n, order fixed) must be packed into
// contiguous groups ("lectures"), each group's total duration <= L.
// First minimize the number of lectures, then among partitions achieving
// that minimum, minimize the sum of per-lecture dissatisfaction DI(free),
// where free = L - group_sum: DI=0 if free=0, DI=-C if 1<=free<=10,
// DI=(free-10)^2 otherwise.
//
// cntF[i] = minimum number of lectures to cover topics 1..i (DP over the
// sliding window of valid group starts, since t_i>=1 bounds each window to
// at most L predecessors and prefix sums are strictly increasing).
// cntB[i] = minimum number of lectures to cover topics i+1..n (symmetric,
// from the back). m = cntF[n] is the overall minimum lecture count.
//
// dp[i][j] = minimum total DI to cover topics 1..i using exactly j
// lectures, restricted to j in [cntF[i], m-cntB[i]] (any j outside that
// range cannot be part of an overall m-lecture optimum, which keeps the
// state space small in practice even though the naive bound is O(n*L*m)).
// Answer is dp[n][m].
//
// Statement ambiguity: none in the wording itself, but the discuss board
// (message 170740) documents an output-format trap not stated in the
// problem text: a blank line follows the "Case k:" line (visible only in
// the sample), and the blank separator appears only *between* cases, not
// after the last one. Handled accordingly below.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
static long long INF;

int n, L, C;
int t[MAXN];
long long S[MAXN];
int cntF[MAXN], cntB[MAXN];
static long long dp[MAXN][MAXN];

long long DI(int freeT) {
    if (freeT == 0) return 0;
    if (freeT <= 10) return -(long long)C;
    long long d = freeT - 10;
    return d * d;
}

int main() {
    INF = (long long)1e15;
    const int INF_I = 1000000000;
    int caseNo = 0;
    bool first = true;

    while (scanf("%d", &n) == 1 && n != 0) {
        scanf("%d %d", &L, &C);
        for (int i = 1; i <= n; i++) scanf("%d", &t[i]);

        S[0] = 0;
        for (int i = 1; i <= n; i++) S[i] = S[i - 1] + t[i];

        cntF[0] = 0;
        for (int i = 1; i <= n; i++) {
            cntF[i] = INF_I;
            for (int j = i - 1; j >= 0; j--) {
                long long sum = S[i] - S[j];
                if (sum > L) break;
                if (cntF[j] + 1 < cntF[i]) cntF[i] = cntF[j] + 1;
            }
        }

        cntB[n] = 0;
        for (int i = n - 1; i >= 0; i--) {
            cntB[i] = INF_I;
            for (int j = i + 1; j <= n; j++) {
                long long sum = S[j] - S[i];
                if (sum > L) break;
                if (cntB[j] + 1 < cntB[i]) cntB[i] = cntB[j] + 1;
            }
        }

        int m = cntF[n];

        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= m; j++)
                dp[i][j] = INF;
        dp[0][0] = 0;

        for (int i = 1; i <= n; i++) {
            int loJ = cntF[i];
            int hiJ = m - cntB[i];
            if (loJ > hiJ) continue;
            for (int j = loJ; j <= hiJ; j++) {
                long long best = INF;
                for (int p = i - 1; p >= 0; p--) {
                    long long sum = S[i] - S[p];
                    if (sum > L) break;
                    int jp = j - 1;
                    if (jp < 0) break;
                    if (dp[p][jp] >= INF) continue;
                    long long cost = dp[p][jp] + DI((int)(L - sum));
                    if (cost < best) best = cost;
                }
                dp[i][j] = best;
            }
        }

        caseNo++;
        if (!first) printf("\n");
        first = false;
        printf("Case %d:\n\n", caseNo);
        printf("Minimum number of lectures: %d\n", m);
        printf("Total dissatisfaction index: %lld\n", dp[n][m]);
    }
    return 0;
}
