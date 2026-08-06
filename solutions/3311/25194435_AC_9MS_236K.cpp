// POJ 3311 - Hie with the Pie
//
// One driver leaves the pizzeria (location 0), delivers to all n <= 10
// locations, and returns. The given matrix is the time of a *direct* hop
// i -> j; the statement explicitly allows passing through other locations
// (or the pizzeria) more than once, and warns the times need not be
// symmetric and need not obey the triangle inequality.
//
// So the matrix is not a metric and cannot be fed to a tour search as is:
// first run Floyd-Warshall over all n+1 nodes to replace every entry by the
// true shortest travel time, which is exactly what "you may pass through
// other locations" means. On that closure the answer is an ordinary
// travelling-salesman cycle through 0, computed with the standard bitmask
// DP: dp[mask][v] = cheapest way to start at the pizzeria, deliver the set
// `mask` of orders and end at v. It is seeded with dp[{v}][v] = d[0][v],
// extended one unvisited order at a time, and the answer is
// min_v dp[full][v] + d[v][0] -- the tour must return to the pizzeria.
// O(2^n * n^2) per case, trivially inside the 2s limit.
//
// Input is a stream of cases terminated by n = 0. n >= 1 always, so the
// full mask is never empty.

#include <cstdio>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int m = n + 1;
        static int d[11][11];
        int i, j, k;
        for (i = 0; i < m; ++i)
            for (j = 0; j < m; ++j)
                scanf("%d", &d[i][j]);
        /* shortest-path closure: revisiting locations is allowed */
        for (k = 0; k < m; ++k)
            for (i = 0; i < m; ++i)
                for (j = 0; j < m; ++j)
                    if (d[i][k] + d[k][j] < d[i][j])
                        d[i][j] = d[i][k] + d[k][j];

        const int INF = 1000000000;
        int full = 1 << n;
        static int dp[1024][10];
        int mask, v;
        for (mask = 0; mask < full; ++mask)
            for (v = 0; v < n; ++v)
                dp[mask][v] = INF;
        for (v = 0; v < n; ++v)
            dp[1 << v][v] = d[0][v + 1];
        for (mask = 1; mask < full; ++mask) {
            for (v = 0; v < n; ++v) {
                if (!(mask & (1 << v))) continue;
                int cur = dp[mask][v];
                if (cur >= INF) continue;
                for (int w = 0; w < n; ++w) {
                    if (mask & (1 << w)) continue;
                    int nm = mask | (1 << w);
                    int cand = cur + d[v + 1][w + 1];
                    if (cand < dp[nm][w]) dp[nm][w] = cand;
                }
            }
        }
        int best = INF;
        for (v = 0; v < n; ++v) {
            int t = dp[full - 1][v] + d[v + 1][0];
            if (t < best) best = t;
        }
        printf("%d\n", best);
    }
    return 0;
}
