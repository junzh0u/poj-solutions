// POJ 3713 - Transferring Sylla
// Model: gpt-5.6-terra
// For every possible removed city, run Tarjan's articulation-point test on
// the remaining graph.  It is 3-vertex-connected exactly when every such
// graph is connected and has no articulation point.
#include <cstdio>
#include <vector>
using namespace std;

class Checker {
    int n;
    const vector< vector<int> >& graph;
    int banned, timer, seen;
    vector<int> dfn, low;
    bool bad;

    void dfs(int u, int parent) {
        int children = 0;
        int i;
        dfn[u] = low[u] = ++timer;
        ++seen;
        for (i = 0; i < (int)graph[u].size(); ++i) {
            int v = graph[u][i];
            if (v == banned || v == parent) continue;
            if (!dfn[v]) {
                ++children;
                dfs(v, u);
                if (low[v] < low[u]) low[u] = low[v];
                if (parent != -1 && low[v] >= dfn[u]) bad = true;
            } else if (dfn[v] < low[u]) {
                low[u] = dfn[v];
            }
        }
        if (parent == -1 && children > 1) bad = true;
    }

public:
    Checker(int cities, const vector< vector<int> >& tunnels)
        : n(cities), graph(tunnels) {}

    bool secureAfterRemoving(int city) {
        int start = 0;
        banned = city;
        while (start == banned) ++start;
        timer = seen = 0;
        bad = false;
        dfn.assign(n, 0);
        low.assign(n, 0);
        dfs(start, -1);
        return !bad && seen == n - 1;
    }
};

int main() {
    int n, m;
    while (scanf("%d%d", &n, &m) == 2 && (n || m)) {
        vector< vector<int> > graph(n);
        int i;
        for (i = 0; i < m; ++i) {
            int a, b;
            scanf("%d%d", &a, &b);
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
        bool ok = n >= 4;
        Checker checker(n, graph);
        for (i = 0; i < n && ok; ++i)
            if (!checker.secureAfterRemoving(i)) ok = false;
        puts(ok ? "YES" : "NO");
    }
    return 0;
}
