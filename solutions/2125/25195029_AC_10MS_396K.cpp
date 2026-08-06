// POJ 2125 - Destroying The Graph
// Model: claude-opus-5
//
// Each arc (u,v) is destroyed iff Bob plays "u -" (remove arcs leaving u) or
// "v +" (remove arcs entering v).  So the task is a minimum-weight vertex cover
// on the bipartite graph whose left nodes are the "u -" moves (weight W[u]^-)
// and right nodes the "v +" moves (weight W[v]^+), one edge per arc.
//
// Min-weight bipartite vertex cover = min s-t cut (Koenig / LP duality):
//   S -> L_u  capacity W[u]^-
//   R_v -> T  capacity W[v]^+
//   L_u -> R_v capacity INF, one per distinct arc
// The max-flow value is the answer.  The cover is read off the residual graph:
// with Z = { nodes reachable from S in the residual }, the cut edges are
// S -> L_u for u not in Z and R_v -> T for v in Z, so the moves are
//   "u -" for every left node NOT reachable, "v +" for every right node reachable.
// (No INF edge can cross the cut: if L_u is reachable so is R_v, so the set is a
// genuine vertex cover, and its weight equals the max-flow value.)
//
// Orientation trap: input line 2 is W^+ (paid to remove arcs INCOMING into i,
// printed as "i +"), line 3 is W^- (arcs OUTGOING from i, printed as "i -").
// The provided sample costs 5 under BOTH readings, so it is no evidence at all
// that the two lines were not swapped; the discriminating case is e.g.
//   2 1 / 10 5 / 1 10 / 1 2  ->  answer 1, move "1 -"  (swapped reading gives 5).
//
// Loops and parallel arcs are allowed; parallel arcs are collapsed since the
// L->R capacity is infinite.  Total cost <= 100*2*10^6 fits in int.  Arrays are
// sized well past the stated N<=100 / M<=5000 limits.
#include <cstdio>
#include <cstring>

const int MAXN = 305;          /* generous: statement says N <= 100 */
const int MAXV = 2 * MAXN + 5;
const int MAXE = 2 * (MAXN * MAXN + 2 * MAXN + 10); /* directed arcs, both ways */
const int INF = 1000000000;

int head[MAXV], nxt[MAXE], to[MAXE], cap[MAXE];
int ecnt;
int nv, S, T;

void init_graph(int n) {
    nv = n;
    ecnt = 0;
    for (int i = 0; i < nv; i++) head[i] = -1;
}

void add_edge(int u, int v, int c) {
    to[ecnt] = v; cap[ecnt] = c; nxt[ecnt] = head[u]; head[u] = ecnt++;
    to[ecnt] = u; cap[ecnt] = 0; nxt[ecnt] = head[v]; head[v] = ecnt++;
}

int level_[MAXV], iter_[MAXV], que_[MAXV];

bool bfs() {
    for (int i = 0; i < nv; i++) level_[i] = -1;
    int qh = 0, qt = 0;
    level_[S] = 0; que_[qt++] = S;
    while (qh < qt) {
        int u = que_[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (cap[e] > 0 && level_[to[e]] < 0) {
                level_[to[e]] = level_[u] + 1;
                que_[qt++] = to[e];
            }
        }
    }
    return level_[T] >= 0;
}

int dfs(int u, int f) {
    if (u == T) return f;
    for (int &e = iter_[u]; e != -1; e = nxt[e]) {
        int v = to[e];
        if (cap[e] > 0 && level_[v] == level_[u] + 1) {
            int d = dfs(v, f < cap[e] ? f : cap[e]);
            if (d > 0) { cap[e] -= d; cap[e ^ 1] += d; return d; }
        }
    }
    return 0;
}

int max_flow() {
    int flow = 0;
    while (bfs()) {
        for (int i = 0; i < nv; i++) iter_[i] = head[i];
        int f;
        while ((f = dfs(S, INF)) > 0) flow += f;
    }
    return flow;
}

bool reach[MAXV];

void residual_reach() {
    for (int i = 0; i < nv; i++) reach[i] = false;
    int qh = 0, qt = 0;
    reach[S] = true; que_[qt++] = S;
    while (qh < qt) {
        int u = que_[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (cap[e] > 0 && !reach[to[e]]) {
                reach[to[e]] = true;
                que_[qt++] = to[e];
            }
        }
    }
}

int wplus[MAXN], wminus[MAXN];
bool arc[MAXN][MAXN];

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        for (int i = 1; i <= n; i++) scanf("%d", &wplus[i]);
        for (int i = 1; i <= n; i++) scanf("%d", &wminus[i]);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) arc[i][j] = false;
        for (int i = 0; i < m; i++) {
            int u, v;
            if (scanf("%d %d", &u, &v) != 2) break;
            if (u >= 1 && u <= n && v >= 1 && v <= n) arc[u][v] = true;
        }

        S = 0; T = 2 * n + 1;
        init_graph(2 * n + 2);
        for (int u = 1; u <= n; u++) add_edge(S, u, wminus[u]);
        for (int v = 1; v <= n; v++) add_edge(n + v, T, wplus[v]);
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++)
                if (arc[u][v]) add_edge(u, n + v, INF);

        int ans = max_flow();
        residual_reach();

        int k = 0;
        for (int u = 1; u <= n; u++) if (!reach[u]) k++;
        for (int v = 1; v <= n; v++) if (reach[n + v]) k++;

        printf("%d\n%d\n", ans, k);
        for (int u = 1; u <= n; u++) if (!reach[u]) printf("%d -\n", u);
        for (int v = 1; v <= n; v++) if (reach[n + v]) printf("%d +\n", v);
    }
    return 0;
}
