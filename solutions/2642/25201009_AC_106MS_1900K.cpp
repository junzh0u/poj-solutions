// POJ 2642 - The Brick Stops Here
// Model: claude-sonnet-5
// Approach: All bricks weigh the same (1000g), so the mixture of M distinct
// brick types has copper concentration equal to (sum of their per-brick
// copper content) / M. A client's request "concentration between CMin and
// CMax" is therefore exactly "sum of chosen concentrations in
// [M*CMin, M*CMax]" over exactly M distinct brick types, minimizing total
// price. This is a 0/1 knapsack with two dimensions: number of items picked
// (0..20, since M<=20) and sum of concentration (0..999*20=19980). We build
// the knapsack ONCE over all N bricks (dp[k][s] = min price to pick exactly
// k distinct brick types with concentration sum s), independent of the C
// queries, then answer each query by scanning dp[M][M*CMin .. M*CMax] for
// the minimum. Recomputing the knapsack per query (an approach several
// people on the discuss board tried) blows the time limit; precomputing
// once keeps total work about N*20*19980 ~ 8*10^7, comfortably fast.
// No statement ambiguity found; sample matches directly.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXK = 20;
const int MAXS = 999 * MAXK; // 19980
const int INF = 1000000000;

int dp[MAXK + 1][MAXS + 1];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    static int conc[205], price[205];
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &conc[i], &price[i]);
    }

    for (int k = 0; k <= MAXK; ++k)
        for (int s = 0; s <= MAXS; ++s)
            dp[k][s] = INF;
    dp[0][0] = 0;

    int curMaxK = 0; // highest k achieved so far
    for (int i = 0; i < n; ++i) {
        int c = conc[i], p = price[i];
        int kLimit = min(curMaxK, MAXK - 1);
        for (int k = kLimit; k >= 0; --k) {
            int sLimit = MAXS - c;
            for (int s = sLimit; s >= 0; --s) {
                if (dp[k][s] < INF) {
                    int ns = s + c;
                    int nv = dp[k][s] + p;
                    if (nv < dp[k + 1][ns]) dp[k + 1][ns] = nv;
                }
            }
        }
        if (curMaxK < MAXK) curMaxK++;
    }

    int c;
    scanf("%d", &c);
    while (c--) {
        int m, cmin, cmax;
        scanf("%d %d %d", &m, &cmin, &cmax);
        int lo = m * cmin;
        int hi = m * cmax;
        if (lo < 0) lo = 0;
        if (hi > MAXS) hi = MAXS;
        int best = INF;
        if (m <= MAXK) {
            for (int s = lo; s <= hi; ++s) {
                if (dp[m][s] < best) best = dp[m][s];
            }
        }
        if (best >= INF) {
            printf("impossible\n");
        } else {
            printf("%d\n", best);
        }
    }

    return 0;
}
