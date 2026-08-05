// POJ 2516 - Minimum Cost
//
// M supply places each stock K kinds of goods; N shopkeepers each order
// K kinds; the transport cost of one unit of kind k from place j to
// shop i is given by the k-th of K N*M matrices. Minimize total cost,
// or print -1 if some order cannot be met.
//
// The K kinds never interact: a unit of kind k can only come out of the
// kind-k stock and can only satisfy kind-k demand, and the cost of
// moving it depends on nothing else. So this is not one big
// transportation problem but K independent ones, and the answers simply
// add. Solving them jointly would need a much larger graph for no gain.
//
// Each kind is a min-cost max-flow on a four-layer graph:
//   source -> place j   with capacity = stock of kind k at j, cost 0
//   place j -> shop i   with capacity INF, cost = matrix_k[i][j]
//   shop i -> sink      with capacity = shop i's order of kind k, cost 0
// Successive shortest augmenting paths found by SPFA (Bellman-Ford), as
// in 2135 -- residual arcs carry negative cost, so Dijkstra is out
// without potentials. If the max flow for a kind falls short of that
// kind's total demand, the whole test case is infeasible and the answer
// is -1; the remaining matrices must still be consumed from stdin, so
// the loop keeps reading and only stops accumulating.
//
// Ambiguity worth noting: the matrix is indexed row = shopkeeper,
// column = supply place (the statement says N * M with i the row), the
// opposite of the graph's natural direction, so the arc built from
// entry (i, j) runs place j -> shop i. Also, a kind with zero total
// demand is trivially satisfiable even when every place stocks zero of
// it -- feasibility is per kind, demand vs. supply, not a global check.
//
// N, M, K < 50, amounts in [0,3], costs in (0,100): at most 147 units of
// flow per kind over ~2500 arcs, times 49 kinds. Twenty test cases at
// the stated maxima run in 0.7s locally against a 4s limit.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MAXV = 105;
const int MAXE = 2 * (50 * 50 + 50 + 50) + 10;
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

/* returns total cost, sets *flowOut to the amount pushed */
int mcmf(int s, int t, int *flowOut) {
    int total = 0, flow = 0;
    while (spfa(s, t)) {
        int aug = INF;
        for (int v = t; v != s; v = to_[prevEdge[v] ^ 1])
            if (cap[prevEdge[v]] < aug) aug = cap[prevEdge[v]];
        for (int v = t; v != s; v = to_[prevEdge[v] ^ 1]) {
            cap[prevEdge[v]] -= aug;
            cap[prevEdge[v] ^ 1] += aug;
        }
        total += aug * dist_[t];
        flow += aug;
    }
    *flowOut = flow;
    return total;
}

int need[55][55];   /* need[i][k] shopkeeper i wants k-th goods */
int have_[55][55];  /* have_[j][k] supply place j stores k-th goods */

int main() {
    int n, m, k;
    while (scanf("%d %d %d", &n, &m, &k) == 3 && (n || m || k)) {
        for (int i = 0; i < n; i++)
            for (int t = 0; t < k; t++) scanf("%d", &need[i][t]);
        for (int j = 0; j < m; j++)
            for (int t = 0; t < k; t++) scanf("%d", &have_[j][t]);

        int answer = 0;
        bool ok = true;
        for (int t = 0; t < k; t++) {
            /* source = 0, supplies 1..m, shops m+1..m+n, sink m+n+1 */
            int s = 0, snk = m + n + 1;
            initGraph(snk + 1);
            int totalNeed = 0;
            for (int j = 0; j < m; j++)
                if (have_[j][t] > 0) addEdge(s, 1 + j, have_[j][t], 0);
            for (int i = 0; i < n; i++) {
                if (need[i][t] > 0) addEdge(m + 1 + i, snk, need[i][t], 0);
                totalNeed += need[i][t];
            }
            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++) {
                    int c;
                    scanf("%d", &c);
                    addEdge(1 + j, m + 1 + i, INF, c);
                }
            int flow = 0;
            int cost = mcmf(s, snk, &flow);
            if (flow < totalNeed) ok = false;
            else answer += cost;
        }
        if (ok) printf("%d\n", answer);
        else printf("-1\n");
    }
    return 0;
}
