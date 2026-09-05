// POJ 2088 - Long Night of Museums
// Model: claude-sonnet-5
//
// Approach: bitmask DP (Held-Karp style). dp[mask][j] = minimum total
// time (museum-visit time plus travel time) needed to visit exactly the
// set of museums in `mask`, ending the tour at museum j. Transitions add
// one more museum k not yet in mask, paying travelTime[j][k] + visitTime[k].
// Any dp value that would exceed the 420-minute budget (6pm to 1am is
// 7 hours) is pruned to "unreachable", since extending it further can
// only make it worse. The answer for each test case is the largest
// popcount(mask) over all masks with a finite dp value for some ending
// museum (0 when no single museum's own visit time fits the budget).
//
// The tour need not start or end at any particular museum and does not
// need to return to the start; travel times are given directly for every
// ordered pair of museums, so there's never a benefit to "passing
// through" an unvisited museum on the way to another one (a trap the
// discuss board raises against naive solvers, but it doesn't apply here
// since the travel matrix already gives direct point-to-point times).
//
// popcount is computed with a small precomputed table rather than a
// compiler builtin, to stay portable across POJ's compilers.

#include <cstdio>

const int MAXN = 20;
const int BUDGET = 420;
const short INF = 32000;
const int MAXMASK = 1 << MAXN;

static short dp[MAXMASK][MAXN];
static int travelTime[MAXN][MAXN];
static int visitTime[MAXN];
static int popcnt[MAXMASK];

int main() {
    popcnt[0] = 0;
    for (int m = 1; m < MAXMASK; m++) {
        popcnt[m] = popcnt[m >> 1] + (m & 1);
    }

    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) {
            scanf("%d", &visitTime[i]);
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%d", &travelTime[i][j]);
            }
        }

        int totalMasks = 1 << n;
        for (int mask = 0; mask < totalMasks; mask++) {
            for (int j = 0; j < n; j++) {
                dp[mask][j] = INF;
            }
        }

        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (visitTime[i] <= BUDGET) {
                dp[1 << i][i] = (short)visitTime[i];
                if (ans < 1) ans = 1;
            }
        }

        for (int mask = 1; mask < totalMasks; mask++) {
            for (int last = 0; last < n; last++) {
                if (!(mask & (1 << last))) continue;
                short cur = dp[mask][last];
                if (cur >= INF) continue;
                for (int nxt = 0; nxt < n; nxt++) {
                    if (mask & (1 << nxt)) continue;
                    int newCost = cur + travelTime[last][nxt] + visitTime[nxt];
                    if (newCost <= BUDGET) {
                        int newMask = mask | (1 << nxt);
                        if (newCost < dp[newMask][nxt]) {
                            dp[newMask][nxt] = (short)newCost;
                            int cnt = popcnt[newMask];
                            if (cnt > ans) ans = cnt;
                        }
                    }
                }
            }
        }

        printf("%d\n", ans);
    }
    return 0;
}
