// POJ 1847 - Tram
//
// Each intersection i lists its K_i outgoing rails. The switch there is already
// pointing at the FIRST intersection listed, so taking that rail is free; taking
// any of the others costs exactly one manual switch change. The cost of a route
// is therefore the number of edges used that were not the first-listed one — not
// the number of rails travelled.
//
// So this is a shortest path on a directed graph with weights in {0, 1}: edge
// i -> adj[i][0] has weight 0, i -> adj[i][j>0] has weight 1. Solved with 0-1 BFS
// (a deque: relax along a 0-edge and push front, along a 1-edge and push back),
// which is O(V + E) — trivially inside the limits for N <= 100.
//
// Statement details worth pinning down:
//   * A == B answers 0 (dist[A] starts at 0, no switch touched).
//   * B unreachable from A answers -1.
//   * A listing itself as a target, and duplicate targets, are both legal and
//     harmless here; the relaxation just keeps the cheapest.
//   * K_i may be 0 (a dead end).
// Verified against a Floyd-Warshall reference on 800 random small graphs.
#include <cstdio>
#include <deque>
#include <vector>

using namespace std;

static const int INF = 1000000000;

int main() {
    int n, a, b;
    while (scanf("%d %d %d", &n, &a, &b) == 3) {
        vector< vector<int> > adj(n + 1);   /* target */
        vector< vector<int> > w(n + 1);     /* cost */
        for (int i = 1; i <= n; ++i) {
            int k;
            if (scanf("%d", &k) != 1) return 0;
            for (int j = 0; j < k; ++j) {
                int t;
                scanf("%d", &t);
                adj[i].push_back(t);
                w[i].push_back(j == 0 ? 0 : 1);
            }
        }
        vector<int> dist(n + 1, INF);
        deque<int> dq;
        dist[a] = 0;
        dq.push_back(a);
        while (!dq.empty()) {
            int u = dq.front();
            dq.pop_front();
            for (size_t e = 0; e < adj[u].size(); ++e) {
                int v = adj[u][e];
                if (v < 1 || v > n) continue;
                int nd = dist[u] + w[u][e];
                if (nd < dist[v]) {
                    dist[v] = nd;
                    if (w[u][e] == 0) dq.push_front(v);
                    else dq.push_back(v);
                }
            }
        }
        printf("%d\n", dist[b] >= INF ? -1 : dist[b]);
    }
    return 0;
}
