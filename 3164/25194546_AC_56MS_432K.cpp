// POJ 3164 - Command Network
//
// Minimum arborescence (directed minimum spanning tree) rooted at node 1,
// solved with Chu-Liu/Edmonds. Note this is NOT an MST: the wires are
// unidirectional, so the answer is the cheapest set of edges that lets node 1
// reach every other node, and Kruskal/Prim give the wrong number here.
//
// Edge weights are the Euclidean distances between the two endpoints, so they
// are reals and the total is printed to two decimals.
//
// The algorithm, per round:
//   1. Give every non-root node its cheapest incoming edge. If some non-root
//      node has none, no arborescence exists.
//   2. If those choices are acyclic they already form the arborescence: stop.
//   3. Otherwise every cycle is contracted to a single node. The accumulated
//      answer already paid inw[v] for every v, so each surviving edge into v
//      is rebated by inw[v] -- the cost of entering the cycle from outside is
//      only the *difference* between the new edge and the cycle edge it
//      displaces. Edges whose endpoints land in the same contracted node are
//      dropped by the u != v test. Repeat on the smaller graph.
// Each round removes at least one node, so there are at most N rounds and the
// whole thing is O(N*M) = 100 * 10^4 per test case.
//
// Statement notes:
//   - "M <= 104" on the problem page is 10^4; the superscript is lost in the
//     rendered text.
//   - The graph need not be reachable from node 1 at all; that case prints
//     'poor snoopy'. A BFS from node 1 settles it up front. Edmonds would also
//     detect it (an unreachable cycle contracts into a node with no incoming
//     edge), but the explicit check is clearer and costs nothing.
//   - Input has parallel edges and self loops; self loops are dropped and
//     parallel edges resolve naturally to the cheapest.
//   - Several test cases per file, read to EOF.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 105;
const int MAXM = 10005;
const double INF = 1e18;

struct Edge { int u, v; double w; };

Edge ed[MAXM];
Edge work[MAXM];
double inw[MAXN * 2];
int pre[MAXN * 2], id_[MAXN * 2], vis[MAXN * 2];

double px[MAXN], py[MAXN];

int head[MAXN], nxt[MAXM], to_[MAXM];
int q[MAXN];
bool seen[MAXN];

/* Chu-Liu / Edmonds minimum arborescence rooted at `root`.
   Returns -1 if no arborescence exists. Destroys `work`. */
double zhuliu(int root, int n, int m) {
    double res = 0.0;
    while (true) {
        int i;
        for (i = 0; i < n; i++) inw[i] = INF;
        for (i = 0; i < m; i++) {
            int u = work[i].u, v = work[i].v;
            if (u != v && work[i].w < inw[v]) { inw[v] = work[i].w; pre[v] = u; }
        }
        for (i = 0; i < n; i++)
            if (i != root && inw[i] >= INF) return -1.0;

        int cnt = 0;
        for (i = 0; i < n; i++) { id_[i] = -1; vis[i] = -1; }
        inw[root] = 0.0;
        for (i = 0; i < n; i++) {
            res += inw[i];
            int v = i;
            while (vis[v] != i && id_[v] == -1 && v != root) { vis[v] = i; v = pre[v]; }
            if (v != root && id_[v] == -1) {
                int u;
                for (u = pre[v]; u != v; u = pre[u]) id_[u] = cnt;
                id_[v] = cnt++;
            }
        }
        if (cnt == 0) break;                       /* no cycle: done */
        for (i = 0; i < n; i++) if (id_[i] == -1) id_[i] = cnt++;
        for (i = 0; i < m; i++) {
            int v = work[i].v;
            work[i].u = id_[work[i].u];
            work[i].v = id_[work[i].v];
            if (work[i].u != work[i].v) work[i].w -= inw[v];
        }
        n = cnt;
        root = id_[root];
    }
    return res;
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        int i;
        for (i = 0; i < n; i++) scanf("%lf %lf", &px[i], &py[i]);
        for (i = 0; i < n; i++) head[i] = -1;
        int mm = 0;
        for (i = 0; i < m; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            --a; --b;
            if (a < 0 || a >= n || b < 0 || b >= n) continue;
            if (a == b) continue;                  /* self loop is useless */
            double dx = px[a] - px[b], dy = py[a] - py[b];
            ed[mm].u = a; ed[mm].v = b; ed[mm].w = sqrt(dx * dx + dy * dy);
            to_[mm] = b; nxt[mm] = head[a]; head[a] = mm;
            mm++;
        }
        /* reachability from node 0 */
        for (i = 0; i < n; i++) seen[i] = false;
        int qh = 0, qt = 0;
        q[qt++] = 0; seen[0] = true;
        while (qh < qt) {
            int u = q[qh++];
            for (int e = head[u]; e != -1; e = nxt[e])
                if (!seen[to_[e]]) { seen[to_[e]] = true; q[qt++] = to_[e]; }
        }
        bool ok = true;
        for (i = 0; i < n; i++) if (!seen[i]) ok = false;
        if (!ok) { printf("poor snoopy\n"); continue; }

        for (i = 0; i < mm; i++) work[i] = ed[i];
        double r = zhuliu(0, n, mm);
        if (r < 0) printf("poor snoopy\n");
        else printf("%.2f\n", r);
    }
    return 0;
}
