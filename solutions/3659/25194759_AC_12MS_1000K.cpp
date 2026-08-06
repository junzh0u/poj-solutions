// POJ 3659 - Cell Phone Network
// Model: gpt-5.6-terra
// Tree DP for the minimum vertex dominating set.  Each vertex is selected,
// covered by a selected child, or left for its parent to cover.  The last
// state is forbidden at the root, which distinguishes domination from vertex
// cover.
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

const int INF = 1000000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    if (!(cin >> n)) return 0;

    vector< vector<int> > graph(n + 1);
    int i;
    for (i = 1; i < n; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    vector<int> parent(n + 1, 0);
    vector<int> order;
    order.reserve(n);
    order.push_back(1);
    for (i = 0; i < (int)order.size(); ++i) {
        int v = order[i];
        int j;
        for (j = 0; j < (int)graph[v].size(); ++j) {
            int u = graph[v][j];
            if (u != parent[v]) {
                parent[u] = v;
                order.push_back(u);
            }
        }
    }

    vector<int> selected(n + 1, 0);
    vector<int> covered_by_child(n + 1, 0);
    vector<int> needs_parent(n + 1, 0);
    for (i = n - 1; i >= 0; --i) {
        int v = order[i];
        int choose = 1;
        int leave_for_parent = 0;
        int covered = 0;
        int extra = INF;
        int j;
        for (j = 0; j < (int)graph[v].size(); ++j) {
            int u = graph[v][j];
            if (parent[u] != v) continue;
            choose += min(selected[u], min(covered_by_child[u], needs_parent[u]));
            if (leave_for_parent >= INF || covered_by_child[u] >= INF)
                leave_for_parent = INF;
            else
                leave_for_parent += covered_by_child[u];
            covered += min(selected[u], covered_by_child[u]);
            extra = min(extra, selected[u] - min(selected[u], covered_by_child[u]));
        }
        selected[v] = choose;
        needs_parent[v] = leave_for_parent;
        covered_by_child[v] = (extra == INF ? INF : covered + extra);
    }

    cout << min(selected[1], covered_by_child[1]) << '\n';
    return 0;
}
