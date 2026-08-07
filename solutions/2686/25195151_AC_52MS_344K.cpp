// POJ 2686 - Traveling by Stagecoach
// Model: claude-opus-5
//
// Bitmask DP over the set of already-spent tickets.
//
// A ride consumes exactly one ticket and moves along one road, taking
// z / t[i] time, so a route from a to b is a walk of k edges together with an
// injective assignment of k of the n tickets to those edges. State
// (mask, v) = "standing in city v having spent exactly the tickets in mask";
// dp[mask][v] is the minimum time to reach it. Every transition sets one more
// bit, so mask strictly increases and the state graph is a DAG: relaxing masks
// in increasing numeric order (mask | bit > mask) needs no priority queue.
// Answer = min over all masks of dp[mask][b], "Impossible" if none is finite.
// Cost: O(2^n * m * n * deg) <= 2^8 * 30 * 8 * 30, trivial.
//
// n <= 8 tickets is what makes the exponential state legal; a plain shortest
// path is wrong because the cost of an edge depends on which ticket is left.
//
// Output: printf("%.3f") deliberately, not "%.3lf" -- the discuss board
// reports repeated WA under "%.3lf" on this judge, and this is submitted under
// language 4 (C++), which the board also names as accepting where G++ (0) has
// given WA. %.3f rounds to within 5e-4, inside the allowed 0.001 error.
//
// Datasets repeat until the line "0 0 0 0 0"; p may be 0, in which case no
// road lines follow.

#include <cstdio>
#include <vector>

using namespace std;

const double INF = 1e18;

int main() {
    int n, m, p, a, b;
    while (scanf("%d %d %d %d %d", &n, &m, &p, &a, &b) == 5) {
        if (n == 0 && m == 0 && p == 0 && a == 0 && b == 0) break;

        vector<int> t(n);
        for (int i = 0; i < n; ++i) scanf("%d", &t[i]);

        // adjacency lists, cities numbered 1..m
        vector<vector<pair<int, int> > > adj(m + 1);
        for (int i = 0; i < p; ++i) {
            int x, y, z;
            scanf("%d %d %d", &x, &y, &z);
            adj[x].push_back(make_pair(y, z));
            adj[y].push_back(make_pair(x, z));
        }

        int full = 1 << n;
        vector<vector<double> > dp(full, vector<double>(m + 1, INF));
        dp[0][a] = 0.0;

        double best = INF;
        for (int mask = 0; mask < full; ++mask) {
            for (int v = 1; v <= m; ++v) {
                double cur = dp[mask][v];
                if (cur >= INF) continue;
                if (v == b && cur < best) best = cur;
                for (int i = 0; i < n; ++i) {
                    if (mask & (1 << i)) continue;
                    int nmask = mask | (1 << i);
                    for (size_t e = 0; e < adj[v].size(); ++e) {
                        int u = adj[v][e].first;
                        double nt = cur + (double)adj[v][e].second / (double)t[i];
                        if (nt < dp[nmask][u]) dp[nmask][u] = nt;
                    }
                }
            }
        }

        if (best >= INF) printf("Impossible\n");
        else printf("%.3f\n", best);
    }
    return 0;
}
