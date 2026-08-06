// POJ 2186 - Popular Cows
//
// "A thinks B is popular" is a directed edge A -> B, and popularity is
// transitive, so cow v is popular with everybody iff every other vertex
// reaches v in the transitive closure.
//
// Condense the digraph into its strongly connected components: within one
// SCC every cow reaches every other, so the whole component is popular or
// none of it is. The condensation is a DAG, and a vertex is reachable from
// all others exactly when it lies in the unique sink component (out-degree
// 0). A DAG always has at least one sink; if it has two, neither reaches
// the other, so nobody is universally popular and the answer is 0.
// Otherwise the answer is the size of that single sink SCC.
//
// SCCs come from Tarjan's algorithm written iteratively with an explicit
// call stack: N is 10,000 and a chain input would recurse 10,000 deep,
// which risks a stack overflow on the judge even though it survives
// locally. Edges are kept in a separate array so the condensation's
// out-degrees can be counted in one pass afterwards.
//
// Note: N = 1 answers 1 - the lone cow is vacuously popular with every
// "other" cow. Self-loops and duplicate edges appear in the input and are
// harmless: both collapse to intra-component edges that are skipped when
// counting out-degrees.

#include <cstdio>

const int MAXN = 10005;
const int MAXM = 50005;

int head[MAXN], nxt[MAXM], dst[MAXM], ecnt = 0;
int dfn[MAXN], low[MAXN], comp[MAXN], sz[MAXN];
int stk[MAXN], instk[MAXN], top = 0;
int callv[MAXN], calle[MAXN];
int outdeg[MAXN];
int idx = 0, ncomp = 0;
int eu[MAXM], ev[MAXM];

static void addEdge(int u, int v) {
    dst[ecnt] = v;
    nxt[ecnt] = head[u];
    head[u] = ecnt++;
}

static void tarjan(int root) {
    int sp = 0;
    dfn[root] = low[root] = ++idx;
    stk[top++] = root;
    instk[root] = 1;
    callv[0] = root;
    calle[0] = head[root];
    while (sp >= 0) {
        int u = callv[sp];
        int e = calle[sp];
        if (e != -1) {
            calle[sp] = nxt[e];
            int v = dst[e];
            if (!dfn[v]) {
                dfn[v] = low[v] = ++idx;
                stk[top++] = v;
                instk[v] = 1;
                ++sp;
                callv[sp] = v;
                calle[sp] = head[v];
            } else if (instk[v]) {
                if (dfn[v] < low[u]) low[u] = dfn[v];
            }
        } else {
            if (low[u] == dfn[u]) {
                ++ncomp;
                for (;;) {
                    int w = stk[--top];
                    instk[w] = 0;
                    comp[w] = ncomp;
                    ++sz[ncomp];
                    if (w == u) break;
                }
            }
            --sp;
            if (sp >= 0) {
                int p = callv[sp];
                if (low[u] < low[p]) low[p] = low[u];
            }
        }
    }
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 1; i <= n; ++i) head[i] = -1;
    for (int i = 0; i < m; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        eu[i] = a;
        ev[i] = b;
        addEdge(a, b);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i);
    for (int i = 0; i < m; ++i) {
        int cu = comp[eu[i]], cv = comp[ev[i]];
        if (cu != cv) ++outdeg[cu];
    }
    int sink = 0, cnt = 0;
    for (int c = 1; c <= ncomp; ++c)
        if (outdeg[c] == 0) { ++cnt; sink = c; }
    printf("%d\n", cnt == 1 ? sz[sink] : 0);
    return 0;
}
