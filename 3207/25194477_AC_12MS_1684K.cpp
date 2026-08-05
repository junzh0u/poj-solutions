// POJ 3207 - Ikki's Story IV - Panda's Trick
//
// n points sit on a circle (0..n-1) and m chords must each be drawn either
// entirely inside or entirely outside the circle, with no two links touching
// except on the boundary.  Decide whether such an assignment exists.
//
// Each chord is one boolean (inside / outside), so this is 2-SAT.  Two chords
// can share a side iff they do not interleave; the outside of a circle is
// itself a disk on the sphere, so the obstruction is the same on both sides.
// With each chord normalised to (a, b), a < b, chords i and j interleave iff
// EXACTLY ONE of A[j], B[j] lies strictly inside the open arc (A[i], B[i]).
// That is the crossing test — not "the intervals overlap": nested chords
// (both endpoints inside) and disjoint ones (neither inside) are both fine,
// only the "one in, one out" case forces the two chords apart.  Chords that
// merely share an endpoint never conflict either; the statement rules those
// out ("every point will have at most one link"), but the guards below make
// the crossing test independent of that promise.
//
// Each interleaving pair contributes x_i != x_j, i.e. the clauses
// (!x_i | !x_j) and (x_i | x_j), which become four implication edges.
// Variable v is node 2v (true / inside) and 2v+1 (false / outside);
// the instance is satisfiable iff no variable has both its literals in one
// strongly connected component.  Tarjan is written iteratively so the
// recursion depth of a 1000-node implication graph cannot blow the stack.
//
// m <= 500, so O(m^2) pair testing is 125k pairs and at most 500k edges —
// trivial well inside the 1s / 128MB budget.
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXV = 1005;   /* 2 * m, m <= 500 */
const int MAXE = 500 * 500 * 2 + 10;

int head[MAXV], nxt[MAXE], to[MAXE], ecnt;

static void addEdge(int u, int v) {
    to[ecnt] = v; nxt[ecnt] = head[u]; head[u] = ecnt++;
}

/* iterative Tarjan */
int dfn[MAXV], low[MAXV], comp[MAXV], stk[MAXV], stkTop;
bool onStk[MAXV];
int idx, ncomp;
int callU[MAXV], callE[MAXV];

static void tarjan(int root) {
    int sp = 0;
    callU[0] = root; callE[0] = head[root];
    dfn[root] = low[root] = ++idx;
    stk[stkTop++] = root; onStk[root] = true;
    while (sp >= 0) {
        int u = callU[sp];
        int e = callE[sp];
        if (e != -1) {
            callE[sp] = nxt[e];
            int v = to[e];
            if (dfn[v] == 0) {
                dfn[v] = low[v] = ++idx;
                stk[stkTop++] = v; onStk[v] = true;
                ++sp;
                callU[sp] = v; callE[sp] = head[v];
            } else if (onStk[v]) {
                if (dfn[v] < low[u]) low[u] = dfn[v];
            }
        } else {
            if (low[u] == dfn[u]) {
                ++ncomp;
                for (;;) {
                    int w = stk[--stkTop];
                    onStk[w] = false;
                    comp[w] = ncomp;
                    if (w == u) break;
                }
            }
            --sp;
            if (sp >= 0) {
                int p = callU[sp];
                if (low[u] < low[p]) low[p] = low[u];
            }
        }
    }
}

int A[505], B[505];

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        (void)n;
        int i, j;
        for (i = 0; i < m; ++i) {
            int a, b;
            if (scanf("%d %d", &a, &b) != 2) { a = b = 0; }
            if (a > b) { int t = a; a = b; b = t; }
            A[i] = a; B[i] = b;
        }
        int V = 2 * m;
        ecnt = 0;
        for (i = 0; i < V; ++i) {
            head[i] = -1; dfn[i] = 0; low[i] = 0; comp[i] = 0; onStk[i] = false;
        }
        for (i = 0; i < m; ++i) {
            for (j = i + 1; j < m; ++j) {
                /* chord i = (A[i], B[i]) with A[i] < B[i];
                   they interleave iff exactly one of A[j], B[j] lies
                   strictly inside the open arc (A[i], B[i]) */
                if (A[i] == B[i] || A[j] == B[j]) continue;      /* degenerate */
                if (A[j] == A[i] || A[j] == B[i] ||
                    B[j] == A[i] || B[j] == B[i]) continue;      /* shared endpoint */
                int c = (A[j] > A[i] && A[j] < B[i]) ? 1 : 0;
                int d = (B[j] > A[i] && B[j] < B[i]) ? 1 : 0;
                if (c != d) {
                    /* x_i != x_j */
                    addEdge(2 * i, 2 * j + 1);
                    addEdge(2 * j, 2 * i + 1);
                    addEdge(2 * i + 1, 2 * j);
                    addEdge(2 * j + 1, 2 * i);
                }
            }
        }
        idx = 0; ncomp = 0; stkTop = 0;
        for (i = 0; i < V; ++i) if (dfn[i] == 0) tarjan(i);
        bool ok = true;
        for (i = 0; i < m; ++i) if (comp[2 * i] == comp[2 * i + 1]) { ok = false; break; }
        if (ok) printf("panda is telling the truth...\n");
        else printf("the evil panda is lying again\n");
    }
    return 0;
}
