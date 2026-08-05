// POJ 3422 - Kaka's Matrix Travels
//
// Kaka makes K trips from the top-left to bottom-right cell of an N x N
// grid, moving only right or down, and sums every cell he passes over --
// but a cell already visited on an earlier trip has been zeroed, so only
// its first visit (across all K trips) ever contributes. Maximize the
// total over K trips (K may be 0).
//
// This is "K best paths with shared value collected once", modeled as a
// min-cost flow of value K from source to sink. Split each cell into an
// in-node and an out-node joined by two parallel arcs: one of capacity 1
// carrying cost -value[i][j] (the only arc that can ever be used, since
// paying for a cell happens at most once), and one of capacity K-1
// carrying cost 0 (lets later trips pass through for free once the value
// has already been claimed, or route through a cell without claiming it
// at all). Movement arcs out(i,j) -> in(i,j+1) and out(i,j) -> in(i+1,j)
// carry capacity K and cost 0. Costs are negated so that maximizing SUM
// becomes minimizing cost; successive shortest augmenting paths are found
// with SPFA (Bellman-Ford) since residual arcs carry negative cost.
//
// The grid being a DAG doesn't make a simple topological longest-path
// argument correct on its own: once flow is pushed, residual back-arcs
// point from a down/right cell back toward the source, and the standard
// successive-shortest-path argument (not a topological one) is what
// guarantees optimality here.
//
// Since routing through a cell beyond the first visit is free, K is
// effectively capped by how many trips can add value at all; capacity K
// on every arc lets the flow always reach exactly K units even when K
// exceeds what's useful (the excess trips just add 0). K = 0 is handled
// directly since building a 0-flow network is pointless.
//
// N <= 50 => at most 2*N*N = 5000 nodes and ~20000 directed arcs; K <= 10
// augmentations of SPFA is trivial well within the 1000ms limit (33ms on
// the judge).
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MAXN = 50;
const int MAXV = 2 * MAXN * MAXN + 2;
const int MAXE = 2 * (4 * MAXN * MAXN + 4);
const int INF = 0x3f3f3f3f;

int head[MAXV], nxt[MAXE], to_[MAXE], cap[MAXE], cst[MAXE], ecnt;
int nv;

void initGraph(int v) {
    nv = v;
    ecnt = 0;
    for (int i = 0; i < v; i++) head[i] = -1;
}

void addEdge(int u, int v, int c, int w) {
    to_[ecnt] = v; cap[ecnt] = c; cst[ecnt] = w; nxt[ecnt] = head[u]; head[u] = ecnt++;
    to_[ecnt] = u; cap[ecnt] = 0; cst[ecnt] = -w; nxt[ecnt] = head[v]; head[v] = ecnt++;
}

int dist_[MAXV], prevEdge[MAXV];
bool inq[MAXV];

bool spfa(int s, int t) {
    for (int i = 0; i < nv; i++) { dist_[i] = INF; inq[i] = false; prevEdge[i] = -1; }
    dist_[s] = 0;
    queue<int> q;
    q.push(s); inq[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop(); inq[u] = false;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (cap[e] > 0 && dist_[u] + cst[e] < dist_[to_[e]]) {
                dist_[to_[e]] = dist_[u] + cst[e];
                prevEdge[to_[e]] = e;
                if (!inq[to_[e]]) { inq[to_[e]] = true; q.push(to_[e]); }
            }
        }
    }
    return dist_[t] < INF;
}

int mcmf(int s, int t, int need) {
    int total = 0, flow = 0;
    while (flow < need && spfa(s, t)) {
        int aug = need - flow;
        for (int v = t; v != s; v = to_[prevEdge[v] ^ 1])
            if (cap[prevEdge[v]] < aug) aug = cap[prevEdge[v]];
        for (int v = t; v != s; v = to_[prevEdge[v] ^ 1]) {
            cap[prevEdge[v]] -= aug;
            cap[prevEdge[v] ^ 1] += aug;
        }
        total += aug * dist_[t];
        flow += aug;
    }
    return total;
}

int a[MAXN][MAXN];

int inNode(int i, int j, int n) { return 2 * (i * n + j); }
int outNode(int i, int j, int n) { return 2 * (i * n + j) + 1; }

int main() {
    int n, k;
    while (scanf("%d %d", &n, &k) == 2) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) scanf("%d", &a[i][j]);

        if (k == 0) {
            printf("0\n");
            continue;
        }

        int src = 2 * n * n, snk = 2 * n * n + 1;
        initGraph(snk + 1);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                addEdge(inNode(i, j, n), outNode(i, j, n), 1, -a[i][j]);
                if (k - 1 > 0)
                    addEdge(inNode(i, j, n), outNode(i, j, n), k - 1, 0);
                if (j + 1 < n)
                    addEdge(outNode(i, j, n), inNode(i, j + 1, n), k, 0);
                if (i + 1 < n)
                    addEdge(outNode(i, j, n), inNode(i + 1, j, n), k, 0);
            }
        addEdge(src, inNode(0, 0, n), k, 0);
        addEdge(outNode(n - 1, n - 1, n), snk, k, 0);

        int cost = mcmf(src, snk, k);
        printf("%d\n", -cost);
    }
    return 0;
}
