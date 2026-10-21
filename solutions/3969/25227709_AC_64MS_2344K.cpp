// POJ 3969 - K-Graph Oddity
// Model: gpt-6-astra
// Choose the least odd k >= maximum degree. Some vertex has degree < k:
// otherwise n odd vertices of odd degree k have an odd total degree.
// Root a spanning tree there and color in reverse discovery order. Every
// nonroot has an uncolored parent, and the root has fewer than k neighbors,
// so the first unused color is always at most k. O(n+m) time and space.
// The input is one graph; output any valid coloring, not a chromatic minimum.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n, m;
    if (scanf("%d%d", &n, &m) != 2) return 0;
    vector< vector<int> > adj(n);
    for (int i = 0; i < m; ++i) {
        int a, b; scanf("%d%d", &a, &b); --a; --b;
        adj[a].push_back(b); adj[b].push_back(a);
    }
    int k = 0;
    for (int v = 0; v < n; ++v) k = max(k, (int)adj[v].size());
    if (k % 2 == 0) ++k;
    int root = 0;
    while ((int)adj[root].size() == k) ++root;
    vector<int> order, seen(n, 0), color(n, 0), used(k + 2, -1);
    order.push_back(root); seen[root] = 1;
    for (int i = 0; i < n; ++i) {
        int v = order[i];
        for (int j = 0; j < (int)adj[v].size(); ++j) {
            int u = adj[v][j];
            if (!seen[u]) { seen[u] = 1; order.push_back(u); }
        }
    }
    for (int i = n - 1; i >= 0; --i) {
        int v = order[i];
        for (int j = 0; j < (int)adj[v].size(); ++j)
            used[color[adj[v][j]]] = v;
        int c = 1;
        while (used[c] == v) ++c;
        color[v] = c;
    }
    printf("%d\n", k);
    for (int v = 0; v < n; ++v) printf("%d\n", color[v]);
    return 0;
}
