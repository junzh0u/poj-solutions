// POJ 3042 - Grazing on the Run
// Model: claude-sonnet-5
// Approach: Bessie always eats a contiguous (in sorted order) prefix/suffix
// pair of clumps around her start L, since eating is instantaneous when she
// passes a clump. So the state after some moves is: the set of eaten clumps
// forms an interval [i clumps taken on the left, j clumps taken on the
// right] of the points sorted by distance from L, and she is currently
// standing at whichever end (left or right) she extended to last.
// dp[i][j][0/1] holds a *reframed* accumulator: total staleness equals
// sum over every unit of travel of (number of clumps eaten at-or-after that
// unit of travel), because a clump's staleness is exactly the distance
// traveled before it is reached. So each new segment of length D, taken
// when i+j clumps have already been eaten out of n, adds D*(n-i-j) to the
// accumulator (D contributes once to the new clump and once more to each
// of the n-i-j-1 remaining clumps still to be eaten later). Extending the
// same direction again is a short local hop; extending the opposite
// direction after having gone out to the far side is a full sweep back
// across the whole currently-eaten span. Answer = dp[k][m][0 or 1] once
// all n points are covered (k = points <= L, m = points > L).
// No statement ambiguity found; board mentions an array-size-1000 RE trap
// (need size n+1) and an indexing off-by-one trap, both avoided here by
// sizing arrays to n+2 and validating against the sample by hand before
// ever touching real data.
#include <cstdio>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF = (ll)4e18;

static ll dp[1002][1002][2];
static ll leftpos[1002], rightpos[1002];

int main() {
    int n; ll L;
    if (scanf("%d %lld", &n, &L) != 2) return 0;
    static ll pos[1002];
    for (int i = 0; i < n; ++i) scanf("%lld", &pos[i]);
    sort(pos, pos + n);

    int k = 0; // count of points <= L
    while (k < n && pos[k] <= L) ++k;
    int m = n - k;

    leftpos[0] = L;
    for (int t = 1; t <= k; ++t) leftpos[t] = pos[k - t]; // nearest first (descending position)
    rightpos[0] = L;
    for (int t = 1; t <= m; ++t) rightpos[t] = pos[k + t - 1]; // nearest first (ascending position)

    dp[0][0][0] = 0;
    dp[0][0][1] = 0;

    for (int i = 0; i <= k; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0 && j == 0) continue;
            if (i > 0) {
                ll segSame = leftpos[i - 1] - leftpos[i];
                ll segCross = rightpos[j] - leftpos[i];
                ll coef = (ll)(n - (i - 1 + j));
                ll best = INF;
                if (dp[i - 1][j][0] < INF) best = min(best, dp[i - 1][j][0] + segSame * coef);
                if (dp[i - 1][j][1] < INF) best = min(best, dp[i - 1][j][1] + segCross * coef);
                dp[i][j][0] = best;
            } else {
                dp[i][j][0] = INF;
            }
            if (j > 0) {
                ll segSame = rightpos[j] - rightpos[j - 1];
                ll segCross = rightpos[j] - leftpos[i];
                ll coef = (ll)(n - (i + j - 1));
                ll best = INF;
                if (dp[i][j - 1][1] < INF) best = min(best, dp[i][j - 1][1] + segSame * coef);
                if (dp[i][j - 1][0] < INF) best = min(best, dp[i][j - 1][0] + segCross * coef);
                dp[i][j][1] = best;
            } else {
                dp[i][j][1] = INF;
            }
        }
    }

    ll ans = min(dp[k][m][0], dp[k][m][1]);
    printf("%lld\n", ans);
    return 0;
}
