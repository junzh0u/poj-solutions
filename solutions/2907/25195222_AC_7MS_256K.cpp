// POJ 2907 - Collecting Beepers
// Model: claude-opus-5
//
// Karel moves only along the axes, so the cost between two cells is their
// Manhattan distance.  The tour must start at Karel's position, visit every
// beeper, and return: a closed TSP over at most 11 nodes (start + <=10
// beepers), which a Held-Karp bitmask DP settles exactly.
//
// dp[mask][i] = cheapest walk that starts at node 0 (Karel), has visited
// exactly the beepers in mask, and currently stands on beeper i.  The answer
// is min over i of dp[full][i] + d(i, start); with zero beepers it is 0.
//
// The world size line is read and discarded -- the grid has no obstacles, so
// it constrains nothing.  Coordinates are 1-based but only differences matter.

#include <cstdio>
#include <cstdlib>

static int ax[12], ay[12];
static int d[12][12];
static int dp[1 << 10][10];

static int iabs(int v) { return v < 0 ? -v : v; }

int main() {
    int scenarios;
    if (scanf("%d", &scenarios) != 1) return 0;
    while (scenarios-- > 0) {
        int sx, sy, n;
        scanf("%d %d", &sx, &sy);      /* world size: unused */
        (void)sx; (void)sy;
        scanf("%d %d", &ax[0], &ay[0]);
        scanf("%d", &n);
        int i, j;
        for (i = 1; i <= n; ++i) scanf("%d %d", &ax[i], &ay[i]);

        for (i = 0; i <= n; ++i)
            for (j = 0; j <= n; ++j)
                d[i][j] = iabs(ax[i] - ax[j]) + iabs(ay[i] - ay[j]);

        if (n == 0) { printf("The shortest path has length 0\n"); continue; }

        const int full = 1 << n;
        const int INF = 1000000000;
        int mask;
        for (mask = 0; mask < full; ++mask)
            for (i = 0; i < n; ++i) dp[mask][i] = INF;
        for (i = 0; i < n; ++i) dp[1 << i][i] = d[0][i + 1];

        for (mask = 1; mask < full; ++mask) {
            for (i = 0; i < n; ++i) {
                if (!(mask & (1 << i))) continue;
                int cur = dp[mask][i];
                if (cur >= INF) continue;
                for (j = 0; j < n; ++j) {
                    if (mask & (1 << j)) continue;
                    int nm = mask | (1 << j);
                    int cand = cur + d[i + 1][j + 1];
                    if (cand < dp[nm][j]) dp[nm][j] = cand;
                }
            }
        }

        int best = INF;
        for (i = 0; i < n; ++i) {
            int cand = dp[full - 1][i] + d[i + 1][0];
            if (cand < best) best = cand;
        }
        printf("The shortest path has length %d\n", best);
    }
    return 0;
}
