// POJ 2735 - Reliable Nets
// Model: claude-sonnet-5
// Approach: branch-and-bound over the (<=20) candidate edges, sorted by
// ascending cost.  At each edge we branch include/exclude, pruning when the
// running cost already reaches the best known answer, and also pruning when
// some vertex cannot reach degree >= 2 even using every remaining edge (a
// vertex of degree < 2 always yields a bridge / disconnection).  At a leaf
// (all edges decided) the chosen multigraph is tested for full connectivity
// and for having zero bridges, using an edge-id based Tarjan bridge check so
// that two parallel edges between the same pair of buildings correctly make
// each other non-bridges.  A "minimal reliable net" is exactly a connected,
// bridgeless (2-edge-connected) spanning subgraph, which is precisely
// "surviving the loss of any single line".
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

int n, m;
struct Edge { int u, v, c; };
Edge edges[25];
int remInc[26][16]; // remInc[i][v] = # of edges[i..m-1] incident to vertex v
bool included[25];
int deg[16];
int best;

// adjacency for leaf check: list of (neighbor, edgeId) per vertex
int adjHead[16];
int adjNext[50];
int adjTo[50];
int adjEid[50];
int adjCnt;

void addAdj(int u, int v, int eid) {
    adjTo[adjCnt] = v; adjEid[adjCnt] = eid; adjNext[adjCnt] = adjHead[u]; adjHead[u] = adjCnt++;
}

int disc[16], low[16], timer_;
bool visited[16];
bool hasBridge;

void dfsBridge(int u, int parentEid) {
    visited[u] = true;
    disc[u] = low[u] = timer_++;
    for (int e = adjHead[u]; e != -1; e = adjNext[e]) {
        if (adjEid[e] == parentEid) continue; // skip the exact parent edge instance
        int v = adjTo[e];
        if (!visited[v]) {
            dfsBridge(v, adjEid[e]);
            low[u] = min(low[u], low[v]);
            if (low[v] > disc[u]) hasBridge = true;
        } else {
            low[u] = min(low[u], disc[v]);
        }
    }
}

bool checkReliable() {
    if (n == 1) return true;
    memset(adjHead, -1, sizeof(adjHead));
    adjCnt = 0;
    for (int i = 0; i < m; i++) if (included[i]) {
        addAdj(edges[i].u, edges[i].v, i);
        addAdj(edges[i].v, edges[i].u, i);
    }
    memset(visited, 0, sizeof(visited));
    timer_ = 0;
    hasBridge = false;
    dfsBridge(1, -1);
    for (int v = 1; v <= n; v++) if (!visited[v]) return false; // not connected
    if (hasBridge) return false;
    return true;
}

int curCost;

bool cmpEdge(const Edge &a, const Edge &b) { return a.c < b.c; }

void solve(int i) {
    if (curCost >= best) return;
    for (int v = 1; v <= n; v++) {
        if (deg[v] + remInc[i][v] < 2) return; // infeasible
    }
    if (i == m) {
        if (checkReliable()) best = curCost;
        return;
    }
    // try include first (edges sorted ascending, gives good bound quickly)
    included[i] = true;
    deg[edges[i].u]++; deg[edges[i].v]++;
    curCost += edges[i].c;
    solve(i + 1);
    curCost -= edges[i].c;
    deg[edges[i].u]--; deg[edges[i].v]--;
    included[i] = false;

    solve(i + 1);
}

int main() {
    int tc = 0;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        tc++;
        for (int i = 0; i < m; i++) {
            scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].c);
        }
        sort(edges, edges + m, cmpEdge);
        for (int v = 0; v <= n; v++) remInc[m][v] = 0;
        for (int i = m - 1; i >= 0; i--) {
            for (int v = 0; v <= n; v++) remInc[i][v] = remInc[i + 1][v];
            remInc[i][edges[i].u]++;
            remInc[i][edges[i].v]++;
        }
        memset(deg, 0, sizeof(deg));
        memset(included, 0, sizeof(included));
        curCost = 0;
        best = 1 << 30;
        if (n == 1) {
            best = 0;
        } else {
            solve(0);
        }
        if (best == (1 << 30)) {
            printf("There is no reliable net possible for test case %d.\n", tc);
        } else {
            printf("The minimal cost for test case %d is %d.\n", tc, best);
        }
    }
    return 0;
}
