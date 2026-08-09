// POJ 1770 - Special Experiment
// Model: gpt-5.6-terra
// Approach: Build the photon-transition forest, then compute its maximum
// weighted independent set with include/exclude tree dynamic programming.
// The statement's unique path condition rules out cycles; disconnected trees
// are handled independently.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, m;
int energy[205];
bool photon[1000001];
vector<int> graph[205];
bool seen[205];
int takeNode[205], skipNode[205];

void solveTree(int u, int parent) {
    int i, v;
    seen[u] = true;
    takeNode[u] = energy[u];
    skipNode[u] = 0;
    for (i = 0; i < (int)graph[u].size(); ++i) {
        v = graph[u][i];
        if (v == parent) continue;
        solveTree(v, u);
        takeNode[u] += skipNode[v];
        skipNode[u] += max(takeNode[v], skipNode[v]);
    }
}

int main() {
    int i, j, x;
    while (cin >> n >> m && (n || m)) {
        for (i = 0; i < n; ++i) {
            cin >> energy[i];
            graph[i].clear();
            seen[i] = false;
        }
        fill(photon, photon + 1000001, false);
        for (i = 0; i < m; ++i) {
            cin >> x;
            photon[x] = true;
        }
        for (i = 0; i < n; ++i)
            for (j = i + 1; j < n; ++j)
                if (photon[energy[j] - energy[i]]) {
                    graph[i].push_back(j);
                    graph[j].push_back(i);
                }
        int answer = 0;
        for (i = 0; i < n; ++i)
            if (!seen[i]) {
                solveTree(i, -1);
                answer += max(takeNode[i], skipNode[i]);
            }
        cout << answer << '\n';
    }
    return 0;
}
