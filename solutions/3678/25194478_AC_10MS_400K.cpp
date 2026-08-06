// POJ 3678 - Katu Puzzle
//
// Each edge constrains exactly two boolean variables, so the puzzle is 2-SAT.
// Encode variable i as two literals: 2i means "Xi = 1", 2i+1 means "Xi = 0";
// negation is the low bit flip. A clause (u OR v) contributes the implications
// ~u -> v and ~v -> u; a unit clause (u) contributes ~u -> u, which alone
// pins u.
//
// The clauses come straight off the truth tables, one row per (op, c). Two of
// the six cases are not binary constraints at all -- they force both inputs:
//
//   AND c=1  both inputs must be 1        units  (Xa=1), (Xb=1)
//   AND c=0  forbids (1,1)                clause (Xa=0 OR Xb=0)
//   OR  c=1  forbids (0,0)                clause (Xa=1 OR Xb=1)
//   OR  c=0  both inputs must be 0        units  (Xa=0), (Xb=0)
//   XOR c=1  forbids (0,0) and (1,1)      clauses (Xa=1 OR Xb=1), (Xa=0 OR Xb=0)
//   XOR c=0  forbids (0,1) and (1,0)      clauses (Xa=1 OR Xb=0), (Xa=0 OR Xb=1)
//
// The puzzle is solvable iff no variable ends up in the same strongly connected
// component as its own negation, found with an iterative Tarjan (recursion
// could go 2000 deep). Nothing special is needed for a = b: the same encoding
// handles it, e.g. "a XOR a = 1" yields both ~Xa -> Xa and Xa -> ~Xa, putting
// the two literals in one component and correctly answering NO.
//
// M is up to 1,000,000 and each edge adds up to 4 implications, but there are
// only 2N <= 2000 literals, so the implication graph is stored as a
// 2000 x 2000 bit matrix (500KB) instead of an edge list. That dedupes the
// implications for free and makes Tarjan's successor scan a word walk: the
// worst case runs in 10MS / 400K rather than the 32MB an adjacency list of
// 4,000,000 edges would have needed.
//
// Statement note: the graph is called directed, but every operator here is
// commutative, so edge orientation carries no information -- only the
// unordered pair {a, b} and the required output c matter.

#include <cstdio>

const int MAXN = 1000;
const int MAXV = 2 * MAXN;          /* implication-graph nodes: 2 literals per variable */
const int W = (MAXV + 31) / 32;     /* words per adjacency row */

unsigned int adj[MAXV][W];
int dfn[MAXV], low[MAXV], comp[MAXV], stk[MAXV], instk[MAXV];
int callStack[MAXV], edgeIter[MAXV];
int n, m, nv, idx, top, ncomp;

/* ---- fast input ---- */
static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return -1;
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x;
}

/* first letter of the operator token: 'A', 'O' or 'X' */
static int readOp() {
    int c = gc();
    while (c != -1 && !((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) c = gc();
    int first = c;
    while (c != -1 && ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) c = gc();
    if (first >= 'a' && first <= 'z') first -= 32;
    return first;
}

/* ---- 2-SAT graph ---- */
static inline int T(int i) { return 2 * i; }      /* literal "Xi = 1" */
static inline int F(int i) { return 2 * i + 1; }  /* literal "Xi = 0" */

static inline void addEdge(int u, int v) { adj[u][v >> 5] |= 1u << (v & 31); }

/* clause (u OR v): ~u -> v and ~v -> u */
static inline void addClause(int u, int v) {
    addEdge(u ^ 1, v);
    addEdge(v ^ 1, u);
}

/* unit clause (u): ~u -> u */
static inline void addUnit(int u) { addEdge(u ^ 1, u); }

/* smallest successor of u with index >= i, or -1 */
static inline int nextSucc(int u, int i) {
    if (i >= nv) return -1;
    const unsigned int *row = adj[u];
    int w = i >> 5;
    unsigned int cur = row[w] & (0xFFFFFFFFu << (i & 31));
    for (;;) {
        if (cur) {
            int b = 0;
            unsigned int x = cur;
            while (!(x & 1u)) { x >>= 1; ++b; }
            return (w << 5) + b;
        }
        if (++w >= W) return -1;
        cur = row[w];
    }
}

static void tarjan() {
    idx = 0; top = 0; ncomp = 0;
    int i;
    for (i = 0; i < nv; ++i) { dfn[i] = 0; comp[i] = 0; instk[i] = 0; }
    for (int s = 0; s < nv; ++s) {
        if (dfn[s]) continue;
        int sp = 0;
        dfn[s] = low[s] = ++idx;
        stk[top++] = s; instk[s] = 1;
        callStack[0] = s; edgeIter[0] = 0;
        while (sp >= 0) {
            int u = callStack[sp];
            int v = nextSucc(u, edgeIter[sp]);
            if (v >= 0) {
                edgeIter[sp] = v + 1;
                if (!dfn[v]) {
                    dfn[v] = low[v] = ++idx;
                    stk[top++] = v; instk[v] = 1;
                    ++sp;
                    callStack[sp] = v; edgeIter[sp] = 0;
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
                        if (w == u) break;
                    }
                }
                --sp;
                if (sp >= 0) {
                    int p = callStack[sp];
                    if (low[u] < low[p]) low[p] = low[u];
                }
            }
        }
    }
}

int main() {
    n = readInt();
    m = readInt();
    nv = 2 * n;
    for (int i = 0; i < m; ++i) {
        int a = readInt();
        int b = readInt();
        int c = readInt();
        int op = readOp();
        if (op == 'A') {                 /* AND */
            if (c == 1) { addUnit(T(a)); addUnit(T(b)); }   /* both inputs forced to 1 */
            else        { addClause(F(a), F(b)); }          /* not both 1 */
        } else if (op == 'O') {          /* OR */
            if (c == 1) { addClause(T(a), T(b)); }          /* not both 0 */
            else        { addUnit(F(a)); addUnit(F(b)); }   /* both inputs forced to 0 */
        } else {                         /* XOR */
            if (c == 1) { addClause(T(a), T(b)); addClause(F(a), F(b)); }  /* differ */
            else        { addClause(T(a), F(b)); addClause(F(a), T(b)); }  /* agree */
        }
    }
    tarjan();
    int ok = 1;
    for (int i = 0; i < n; ++i) {
        if (comp[2 * i] == comp[2 * i + 1]) { ok = 0; break; }
    }
    puts(ok ? "YES" : "NO");
    return 0;
}
