// POJ 3921 - Destroying the bus stations
// Model: gpt-5.6-terra
// Branch on an internal station of a currently shortest route of at most k
// roads. Every feasible destruction set must contain one such station.
// A greedy packing of internally disjoint offending routes gives a safe
// lower bound for pruning.

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>

using namespace std;

const int MAXN = 55;
const int INF = 1000000;

int n, m, limit;
vector<int> graph[MAXN];
bool removedStation[MAXN];
int best;

int shortestPath(const bool blocked[], vector<int> *path) {
    int distance[MAXN], parent[MAXN];
    queue<int> q;
    int i;
    for (i = 1; i <= n; ++i) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (distance[u] >= limit) continue;
        for (i = 0; i < (int)graph[u].size(); ++i) {
            int v = graph[u][i];
            if (blocked[v] || distance[v] != INF) continue;
            distance[v] = distance[u] + 1;
            parent[v] = u;
            q.push(v);
        }
    }
    if (distance[n] == INF) return INF;
    if (path != 0) {
        path->clear();
        for (i = n; i != -1; i = parent[i]) path->push_back(i);
        reverse(path->begin(), path->end());
    }
    return distance[n];
}

int packingLowerBound() {
    bool blocked[MAXN];
    int count = 0, i;
    vector<int> path;
    memcpy(blocked, removedStation, sizeof(blocked));
    while (shortestPath(blocked, &path) <= limit) {
        ++count;
        for (i = 1; i + 1 < (int)path.size(); ++i) blocked[path[i]] = true;
    }
    return count;
}

void search(int used) {
    vector<int> path;
    int i, lower;
    if (used >= best) return;
    if (shortestPath(removedStation, &path) > limit) {
        best = used;
        return;
    }
    lower = packingLowerBound();
    if (used + lower >= best) return;

    // Trying high-degree stations first normally finds a strong upper bound
    // early, without affecting the exhaustive branching argument.
    vector<int> choices;
    for (i = 1; i + 1 < (int)path.size(); ++i) choices.push_back(path[i]);
    for (i = 0; i < (int)choices.size(); ++i) {
        int j;
        for (j = i + 1; j < (int)choices.size(); ++j) {
            if (graph[choices[j]].size() > graph[choices[i]].size()) {
                int tmp = choices[i];
                choices[i] = choices[j];
                choices[j] = tmp;
            }
        }
    }
    for (i = 0; i < (int)choices.size(); ++i) {
        int v = choices[i];
        removedStation[v] = true;
        search(used + 1);
        removedStation[v] = false;
    }
}

int main() {
    while (scanf("%d%d%d", &n, &m, &limit) == 3 && (n || m || limit)) {
        int i, s, f;
        for (i = 1; i <= n; ++i) graph[i].clear();
        for (i = 0; i < m; ++i) {
            scanf("%d%d", &s, &f);
            graph[s].push_back(f);
        }
        memset(removedStation, 0, sizeof(removedStation));
        best = n - 2;
        if (shortestPath(removedStation, 0) > limit) best = 0;
        else search(0);
        printf("%d\n", best);
    }
    return 0;
}
