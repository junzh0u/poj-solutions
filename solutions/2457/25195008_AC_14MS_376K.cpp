// POJ 2457 - Part Acquisition
// Model: claude-sonnet-5
// Approach: model each item type 1..K as a graph node; each planet gives a
// directed edge a_i -> b_i (trade away a_i, receive b_i). The cows start
// holding item 1 and want item K, so the minimum number of trades is the
// shortest path (in edges) from node 1 to node K in this directed graph,
// found with a plain BFS (unweighted, up to 1000 nodes / 50000 edges).
// Output is one more than that edge count (the count of objects held,
// i.e. the number of nodes on the path) followed by the path itself, or
// -1 if K is unreachable. Special Judge: any minimum-length path is
// accepted, only the length is unique -- discuss board confirms this and
// separately flags two pitfalls: edges are directed (not to be treated as
// undirected), and duplicate (a_i, b_i) pairs across different planets are
// common and must not break BFS (they just add redundant edges, harmless
// here since BFS only needs first-visit distances).
#include <cstdio>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    vector<vector<int> > adj(k + 1);
    for (int i = 0; i < n; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        adj[a].push_back(b);
    }

    vector<int> dist(k + 1, -1);
    vector<int> parent(k + 1, -1);
    queue<int> q;
    dist[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == k) break;
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    if (dist[k] == -1) {
        printf("-1\n");
        return 0;
    }

    printf("%d\n", dist[k] + 1);
    vector<int> path;
    for (int cur = k; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    for (int i = (int)path.size() - 1; i >= 0; --i) {
        printf("%d\n", path[i]);
    }
    return 0;
}
