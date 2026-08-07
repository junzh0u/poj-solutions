// POJ 3068 - "Shortest" pair of paths
// Model: claude-sonnet-5
// Approach: min-cost flow of 2 units from depot 0 to depot N-1. Each
// intermediate depot is split into an in-node and an out-node joined by a
// capacity-1 cost-0 edge, forcing the two chemical paths to be vertex-disjoint
// except at the endpoints (which allow co-location per the statement). Each
// original shipping method becomes its own capacity-1 edge -- parallel edges
// between the same pair of depots are kept distinct (not merged), since two
// different methods may each carry one chemical. Successive shortest
// augmenting paths (SPFA/Bellman-Ford, since residual reverse edges can be
// negative cost) find the two disjoint paths minimizing total cost; if fewer
// than 2 units of flow can be pushed, the shipment is impossible.
// Ambiguity: none in the statement itself, but the discuss board flags a
// common implementation bug (message 350089) -- storing edges in an
// adjacency matrix keyed by (u,v) silently collapses parallel edges between
// the same pair, corrupting which edge's capacity actually gets decremented.
// Using an explicit edge-list/adjacency-list (as below) avoids this.
#include <cstdio>
#include <cstring>
#include <vector>
#include <deque>
using namespace std;

const int MAXV = 140;
const int INF = 1 << 29;

struct Edge { int to, cap, cost; };
vector<Edge> edges;
vector<int> g[MAXV];

void addEdge(int u, int v, int cap, int cost) {
    g[u].push_back((int)edges.size());
    Edge e1; e1.to = v; e1.cap = cap; e1.cost = cost; edges.push_back(e1);
    g[v].push_back((int)edges.size());
    Edge e2; e2.to = u; e2.cap = 0; e2.cost = -cost; edges.push_back(e2);
}

int dist_[MAXV], prevE[MAXV];
bool inq[MAXV];

bool spfa(int S, int T, int n, int &flow, int &cost) {
    for (int i = 0; i < n; i++) { dist_[i] = INF; inq[i] = false; prevE[i] = -1; }
    deque<int> q;
    dist_[S] = 0; q.push_back(S); inq[S] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop_front(); inq[u] = false;
        for (size_t k = 0; k < g[u].size(); k++) {
            int id = g[u][k];
            const Edge &e = edges[id];
            if (e.cap > 0 && dist_[u] + e.cost < dist_[e.to]) {
                dist_[e.to] = dist_[u] + e.cost;
                prevE[e.to] = id;
                if (!inq[e.to]) {
                    inq[e.to] = true;
                    q.push_back(e.to);
                }
            }
        }
    }
    if (dist_[T] >= INF) return false;
    int aug = INF;
    int v = T;
    while (v != S) {
        int id = prevE[v];
        if (edges[id].cap < aug) aug = edges[id].cap;
        v = edges[id ^ 1].to;
    }
    v = T;
    while (v != S) {
        int id = prevE[v];
        edges[id].cap -= aug;
        edges[id ^ 1].cap += aug;
        v = edges[id ^ 1].to;
    }
    flow += aug;
    cost += aug * dist_[T];
    return true;
}

int main() {
    int n, m;
    int instance = 0;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        instance++;
        edges.clear();
        for (int i = 0; i < 2 * n + 2; i++) g[i].clear();

        int S = 0, T = n - 1;
        // outId(v): split node for intermediate vertices only
        for (int v = 1; v <= n - 2; v++) {
            addEdge(v, v + n, 1, 0); // in(v) -> out(v)
        }
        for (int e = 0; e < m; e++) {
            int i, j, v;
            scanf("%d %d %d", &i, &j, &v);
            int u_out = (i == S || i == T) ? i : i + n;
            int v_in = j; // in(v) is always v itself
            addEdge(u_out, v_in, 1, v);
        }

        int flow = 0, cost = 0;
        bool ok = true;
        if (S == T) {
            flow = 2; cost = 0;
        } else {
            for (int k = 0; k < 2; k++) {
                if (!spfa(S, T, 2 * n + 2, flow, cost)) { ok = false; break; }
            }
        }
        if (ok && flow >= 2) {
            printf("Instance #%d: %d\n", instance, cost);
        } else {
            printf("Instance #%d: Not possible\n", instance);
        }
    }
    return 0;
}
