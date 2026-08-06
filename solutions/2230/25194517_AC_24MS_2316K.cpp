// POJ 2230 - Watchcow
//
// Walk every one of the M bidirectional trails exactly twice, once in each
// direction, starting and ending at field 1.
//
// Replacing each undirected edge {u,v} with the two opposite arcs u->v and
// v->u turns the requirement into "use every arc of this digraph exactly
// once" — an Eulerian circuit. The construction gives every vertex equal
// in-degree and out-degree for free, and the graph is connected (the
// statement guarantees a path exists), so a circuit always exists and no
// feasibility check is needed.
//
// Hierholzer's algorithm builds it. The traversal is iterative — at the
// stated maxima the circuit is 2M+1 = 100001 vertices long and recursion
// would risk blowing the stack. The per-vertex "current arc" pointer
// (iterp[]) is advanced destructively as arcs are consumed, so each arc is
// looked at once and the whole run is O(N+M) rather than quadratic on a
// dense multigraph. Vertices are appended to the circuit only when they run
// out of unused arcs; the circuit is emitted in reverse of that order.
//
// Ambiguity: a pair of fields may be joined by several trails, so the graph
// is a multigraph and adjacency must be an arc list, not an adjacency
// matrix or a set. The judge is a Special Judge, so any valid tour passes —
// the output need not match the sample.
#include <cstdio>

static const int MAXN = 10005;
static const int MAXE = 100005;

static int head[MAXN];
static int iterp[MAXN];
static int to_[MAXE];
static int nxt[MAXE];
static int ecnt = 0;

static int stk[MAXE + 5];
static int circuit[MAXE + 5];

static void add(int u, int v) {
    to_[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt++;
}

static char obuf[MAXE * 7];

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 1; i <= n; ++i) head[i] = -1;
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) break;
        add(u, v);
        add(v, u);
    }
    for (int i = 1; i <= n; ++i) iterp[i] = head[i];

    int sp = 0, cc = 0;
    stk[sp++] = 1;
    while (sp > 0) {
        int v = stk[sp - 1];
        int e = iterp[v];
        if (e != -1) {
            iterp[v] = nxt[e];
            stk[sp++] = to_[e];
        } else {
            circuit[cc++] = v;
            --sp;
        }
    }

    int p = 0;
    for (int i = cc - 1; i >= 0; --i) {
        int x = circuit[i];
        char tmp[8];
        int t = 0;
        if (x == 0) tmp[t++] = '0';
        while (x > 0) { tmp[t++] = (char)('0' + x % 10); x /= 10; }
        while (t > 0) obuf[p++] = tmp[--t];
        obuf[p++] = '\n';
    }
    fwrite(obuf, 1, (size_t)p, stdout);
    return 0;
}
