// POJ 2749 - Building roads
// Model: claude-opus-5
//
// Every barn hangs off S1 or S2, and S1-S2 is one edge, so the network is a
// tree and the barn-to-barn distances are fixed by the assignment alone:
//   both on S1        d1[i] + d1[j]
//   both on S2        d2[i] + d2[j]
//   i on S1, j on S2  d1[i] + D + d2[j]          (D = |S1 S2|)
// Binary search the answer X and decide feasibility with 2-SAT: variable
// x_i = "barn i hangs off S1".  For an ordered pair i != j the three shapes
// above give exactly three kinds of forbidden combination
//   A(i,j): d1[i]+d1[j] > X       forbids (S1,S1)
//   B(i,j): d2[i]+d2[j] > X       forbids (S2,S2)
//   E(i,j): d1[i]+D+d2[j] > X     forbids (i on S1, j on S2)
// and each forbidden combination is one 2-clause.  Hate pairs add x_i xor x_j,
// friend pairs add x_i <-> x_j; those do not depend on X so they are the only
// edges stored explicitly.
//
// The distance clauses are left *implicit* instead of materialised: there are
// up to 8*C(500,2) = 1e6 of them and rebuilding that graph ~24 times is what
// makes the straightforward version time out here.  Tarjan enumerates the
// out-neighbours of node u = (barn i, side p) by one linear scan over d1/d2
// with two precomputed thresholds -- A/B is "d[j] > X - d[i]" and E is
// "d[j] > X - D - d[i]" -- so a whole feasibility test costs 2n*n threshold
// tests (5e5) with no graph construction at all.  Satisfiable iff no i has
// comp[x_i] == comp[!x_i].
//
// Upper bound for the search: with X = 2*max_i max(d1[i],d2[i]) + D every
// distance clause is vacuous, so infeasibility there means the hate/friend
// system alone is contradictory -> -1.  A looser bound only wastes iterations
// and a tighter one can cut off the true answer.

#include <cstdio>
#include <cstring>

const int MAXN = 505;
const int MAXV = 2 * MAXN;
const int MAXE = 8 * 2005 + 10;

int n, na, nb;
int d1[MAXN], d2[MAXN], D;

int ehead[MAXV], enxt[MAXE], eto[MAXE], ecnt;

static void addEdge(int u, int v) {
    eto[ecnt] = v;
    enxt[ecnt] = ehead[u];
    ehead[u] = ecnt++;
}

int X;
int dfn[MAXV], low[MAXV], comp[MAXV], stk[MAXV];
bool ins[MAXV];
int top, idx, scnt;

#define VISIT(vv)                                       \
    {                                                   \
        int _v = (vv);                                  \
        if (!dfn[_v]) {                                 \
            tarjan(_v);                                 \
            if (low[_v] < low[u]) low[u] = low[_v];     \
        } else if (ins[_v]) {                           \
            if (dfn[_v] < low[u]) low[u] = dfn[_v];     \
        }                                               \
    }

/* node 2i = "barn i on S1", node 2i+1 = "barn i on S2" */
static void tarjan(int u) {
    dfn[u] = low[u] = ++idx;
    stk[top++] = u;
    ins[u] = true;

    const int i = u >> 1;
    const int p = u & 1;
    const int self = p ? d2[i] : d1[i];
    const int t1 = X - self;      /* same-side clause A (p==0) or B (p==1) */
    const int t2 = X - D - self;  /* cross clause E */
    const int *arrA = p ? d2 : d1;
    const int *arrB = p ? d1 : d2;
    const int pa = 1 - p;
    const int pb = p;

    for (int j = 0; j < n; ++j) {
        if (j == i) continue;
        if (arrA[j] > t1) VISIT(2 * j + pa)
        if (arrB[j] > t2) VISIT(2 * j + pb)
    }
    for (int e = ehead[u]; e != -1; e = enxt[e]) VISIT(eto[e])

    if (low[u] == dfn[u]) {
        int v;
        do {
            v = stk[--top];
            ins[v] = false;
            comp[v] = scnt;
        } while (v != u);
        ++scnt;
    }
}

static bool feasible(int limit) {
    X = limit;
    int m = 2 * n;
    memset(dfn, 0, sizeof(int) * m);
    memset(ins, 0, sizeof(bool) * m);
    top = idx = scnt = 0;
    for (int u = 0; u < m; ++u)
        if (!dfn[u]) tarjan(u);
    for (int i = 0; i < n; ++i)
        if (comp[2 * i] == comp[2 * i + 1]) return false;
    return true;
}

int main() {
    while (scanf("%d %d %d", &n, &na, &nb) == 3) {
        int sx1, sy1, sx2, sy2;
        scanf("%d %d %d %d", &sx1, &sy1, &sx2, &sy2);
        D = (sx1 > sx2 ? sx1 - sx2 : sx2 - sx1) + (sy1 > sy2 ? sy1 - sy2 : sy2 - sy1);
        int maxd = 0;
        for (int i = 0; i < n; ++i) {
            int x, y;
            scanf("%d %d", &x, &y);
            int ax = x - sx1, ay = y - sy1;
            if (ax < 0) ax = -ax;
            if (ay < 0) ay = -ay;
            d1[i] = ax + ay;
            int bx = x - sx2, by = y - sy2;
            if (bx < 0) bx = -bx;
            if (by < 0) by = -by;
            d2[i] = bx + by;
            if (d1[i] > maxd) maxd = d1[i];
            if (d2[i] > maxd) maxd = d2[i];
        }
        ecnt = 0;
        for (int u = 0; u < 2 * n; ++u) ehead[u] = -1;
        for (int k = 0; k < na; ++k) {
            int a, b;
            scanf("%d %d", &a, &b);
            --a; --b;
            /* hate: different points -- (a v b) and (~a v ~b) */
            addEdge(2 * a + 1, 2 * b);
            addEdge(2 * b + 1, 2 * a);
            addEdge(2 * a, 2 * b + 1);
            addEdge(2 * b, 2 * a + 1);
        }
        for (int k = 0; k < nb; ++k) {
            int a, b;
            scanf("%d %d", &a, &b);
            --a; --b;
            /* friends: same point */
            addEdge(2 * a, 2 * b);
            addEdge(2 * b, 2 * a);
            addEdge(2 * a + 1, 2 * b + 1);
            addEdge(2 * b + 1, 2 * a + 1);
        }

        int hi = 2 * maxd + D;
        if (!feasible(hi)) {
            printf("-1\n");
            continue;
        }
        int lo = 0;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (feasible(mid)) hi = mid; else lo = mid + 1;
        }
        printf("%d\n", lo);
    }
    return 0;
}
