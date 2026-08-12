// POJ 3895 - Cycles of Lanes
// Model: gpt-5.6-terra
// Approach: Tarjan's edge-stack decomposition finds every biconnected component.
// Because every edge belongs to at most one simple cycle, each nontrivial
// biconnected component is precisely one simple cycle; its distinct vertices
// give that cycle's length. Bridges contribute nothing, so an acyclic graph
// prints 0 (as required by the discussion-board clarification).

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int to, id;
    Edge(int t, int i) : to(t), id(i) {}
};

vector< vector<Edge> > graph;
vector<int> disc, low, edgeStack;
vector< pair<int, int> > graphEdges;
int timer_, answer;

void dfs(int u, int parentEdge) {
    disc[u] = low[u] = ++timer_;
    for (size_t i = 0; i < graph[u].size(); ++i) {
        int v = graph[u][i].to;
        int id = graph[u][i].id;
        if (id == parentEdge) continue;
        if (!disc[v]) {
            edgeStack.push_back(id);
            dfs(v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] >= disc[u]) {
                vector<int> vertices;
                while (true) {
                    int e = edgeStack.back();
                    edgeStack.pop_back();
                    vertices.push_back(graphEdges[e].first);
                    vertices.push_back(graphEdges[e].second);
                    if (e == id) break;
                }
                sort(vertices.begin(), vertices.end());
                int cnt = 0;
                for (size_t j = 0; j < vertices.size(); ++j)
                    if (j == 0 || vertices[j] != vertices[j - 1]) ++cnt;
                if (cnt >= 3) answer = max(answer, cnt);
            }
        } else if (disc[v] < disc[u]) {
            edgeStack.push_back(id);
            low[u] = min(low[u], disc[v]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        cin >> n >> m;
        graph.assign(n + 1, vector<Edge>());
        graphEdges.clear();
        for (int i = 0; i < m; ++i) {
            int u, v;
            cin >> u >> v;
            graphEdges.push_back(make_pair(u, v));
            graph[u].push_back(Edge(v, i));
            graph[v].push_back(Edge(u, i));
        }
        disc.assign(n + 1, 0);
        low.assign(n + 1, 0);
        edgeStack.clear();
        timer_ = answer = 0;
        for (int i = 1; i <= n; ++i)
            if (!disc[i]) dfs(i, -1);
        cout << answer << '\n';
    }
    return 0;
}
