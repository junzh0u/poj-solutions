// POJ 2831 - Can We Build This One?
// Model: gpt-5.6-terra
// Build one minimum spanning tree.  The greatest edge on its path between two
// endpoints is the minimum possible bottleneck; a reduced edge can occur in
// some MST exactly when that bottleneck is at least its new cost.

#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const { return w < other.w; }
};

struct DSU {
    int p[1001];
    void init(int n) { for (int i = 1; i <= n; ++i) p[i] = i; }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool join(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        p[b] = a;
        return true;
    }
};

struct Adj { int to, w; };

int up[11][1001], best[11][1001], depth[1001];
vector<Adj> tree[1001];

void dfs(int u, int parent, int weight) {
    up[0][u] = parent;
    best[0][u] = weight;
    for (size_t i = 0; i < tree[u].size(); ++i) {
        int v = tree[u][i].to;
        if (v == parent) continue;
        depth[v] = depth[u] + 1;
        dfs(v, u, tree[u][i].w);
    }
}

int pathMax(int a, int b) {
    int ans = 0;
    if (depth[a] < depth[b]) swap(a, b);
    for (int k = 10; k >= 0; --k)
        if (depth[a] - (1 << k) >= depth[b]) {
            ans = max(ans, best[k][a]);
            a = up[k][a];
        }
    if (a == b) return ans;
    for (int k = 10; k >= 0; --k)
        if (up[k][a] != up[k][b]) {
            ans = max(ans, best[k][a]);
            ans = max(ans, best[k][b]);
            a = up[k][a]; b = up[k][b];
        }
    return max(ans, max(best[0][a], best[0][b]));
}

int main() {
    int n, m, q;
    while (scanf("%d%d%d", &n, &m, &q) == 3) {
        vector<Edge> edges(m), sorted;
        for (int i = 0; i < m; ++i) scanf("%d%d%d", &edges[i].u, &edges[i].v, &edges[i].w);
        sorted = edges;
        sort(sorted.begin(), sorted.end());
        for (int i = 1; i <= n; ++i) tree[i].clear();
        DSU dsu;
        dsu.init(n);
        int used = 0;
        for (size_t i = 0; i < sorted.size() && used < n - 1; ++i)
            if (dsu.join(sorted[i].u, sorted[i].v)) {
                Adj a = {sorted[i].v, sorted[i].w};
                Adj b = {sorted[i].u, sorted[i].w};
                tree[sorted[i].u].push_back(a);
                tree[sorted[i].v].push_back(b);
                ++used;
            }
        depth[1] = 0;
        dfs(1, 0, 0);
        for (int k = 1; k <= 10; ++k)
            for (int v = 1; v <= n; ++v) {
                up[k][v] = up[k - 1][up[k - 1][v]];
                best[k][v] = max(best[k - 1][v], best[k - 1][up[k - 1][v]]);
            }
        while (q--) {
            int i, x;
            scanf("%d%d", &i, &x);
            --i;
            puts(pathMax(edges[i].u, edges[i].v) >= x ? "Yes" : "No");
        }
    }
    return 0;
}
