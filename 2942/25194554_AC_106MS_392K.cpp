// POJ 2942 - Knights of the Round Table
//
// A seating is a set of knights placed around a table so that no two neighbours
// hate each other, with an odd number (>= 3) of them.  Build the *compatibility*
// graph G = complement of the enmity graph: a seating is exactly a simple cycle
// of G of odd length, and since a simple graph has no cycles of length 1 or 2,
// "odd cycle" already implies length >= 3.  So a knight survives iff some simple
// odd cycle of G passes through him, and the answer is the number of knights for
// which none does.
//
// Every simple cycle lies inside a single biconnected component (a BCC is an
// edge-partition, and two edges are in the same BCC iff a cycle contains both),
// so the question is local to each BCC.  Within one BCC the criterion collapses:
// if a 2-connected subgraph contains an odd cycle at all, then *every* one of its
// vertices lies on an odd cycle.  Proof: let C be an odd cycle in the block and v
// a vertex of the block off C.  2-connectivity gives two vertex-disjoint paths
// from v to C (fan lemma), meeting C at distinct a, b.  Those split C into two
// arcs whose lengths differ in parity (C is odd), and combining each arc with the
// two paths gives two cycles through v whose lengths differ in parity - one of
// them is odd.  A block contains an odd cycle iff it is not bipartite, so:
//
//   mark every vertex of every non-bipartite biconnected component of G;
//   the answer is the count of unmarked knights.
//
// Implementation: enmity stored as a bitset per knight, the complement expanded
// into a CSR adjacency (n*(n-1) <= 999000 entries).  Tarjan BCC is written
// iteratively with an explicit vertex stack plus the usual edge stack; each block
// popped off the edge stack is tested for bipartiteness by a parity union-find
// over just that block's edges (lazily reset with a stamp, so the total cost over
// all blocks stays linear in the number of edges).  Note the bipartiteness test
// must be per block, not per connected component: a connected component can mix
// a non-bipartite block with bipartite ones, and only the former's vertices can
// be seated.
//
// Statement notes: the m lines may repeat a pair and may name a knight as his own
// enemy; both are harmless and simply ignored.  n = 1 (or any knight isolated in
// G) yields an expelled knight, matching "one person cannot sit around a table".
//
// Accepted: 106MS, 392K, C++ (run 25194554).
#include <cstdio>
#include <cstring>

static const int MAXN = 1005;
static const int MAXADJ = 1000005;   /* n*(n-1) <= 999000 directed entries */
static const int MAXES = 500005;     /* n*(n-1)/2 <= 499500 undirected edges */

/* ---------- fast input ---------- */
static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;
static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}
static inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sgn;
    return true;
}

/* ---------- graph state ---------- */
static unsigned int hate[MAXN][(MAXN + 31) / 32];
static int start_[MAXN + 1];
static int adjv[MAXADJ];

static int disc[MAXN], low[MAXN], par[MAXN], iter_[MAXN], stk[MAXN];
static int es_a[MAXES], es_b[MAXES];
static int esTop;
static bool okv[MAXN];

/* ---------- parity DSU, lazily reset by stamp ---------- */
static int dsuPar[MAXN], dsuRel[MAXN], dsuRank[MAXN], stampArr[MAXN];
static int curStamp = 0;
static int fRoot;

static inline void ensureV(int x) {
    if (stampArr[x] != curStamp) {
        stampArr[x] = curStamp;
        dsuPar[x] = x;
        dsuRel[x] = 0;
        dsuRank[x] = 0;
    }
}

static int findRel(int x) {
    int root = x, r = 0;
    while (dsuPar[root] != root) { r ^= dsuRel[root]; root = dsuPar[root]; }
    int cur = x, curRel = r;
    while (dsuPar[cur] != cur) {
        int nxt = dsuPar[cur];
        int nxtRel = curRel ^ dsuRel[cur];
        dsuPar[cur] = root;
        dsuRel[cur] = curRel;
        cur = nxt;
        curRel = nxtRel;
    }
    fRoot = root;
    return r;
}

/* one biconnected component = edges es[l..r); non-bipartite => every vertex of
   it lies on an odd cycle */
static void processBCC(int l, int r) {
    ++curStamp;
    bool odd = false;
    for (int i = l; i < r; i++) {
        int a = es_a[i], b = es_b[i];
        ensureV(a); ensureV(b);
        int ra = findRel(a); int rootA = fRoot;
        int rb = findRel(b); int rootB = fRoot;
        if (rootA == rootB) {
            if (ra == rb) odd = true;
        } else {
            int rel = 1 ^ ra ^ rb;
            if (dsuRank[rootA] > dsuRank[rootB]) {
                dsuPar[rootB] = rootA;
                dsuRel[rootB] = rel;
            } else {
                dsuPar[rootA] = rootB;
                dsuRel[rootA] = rel;
                if (dsuRank[rootA] == dsuRank[rootB]) dsuRank[rootB]++;
            }
        }
    }
    if (odd) {
        for (int i = l; i < r; i++) { okv[es_a[i]] = true; okv[es_b[i]] = true; }
    }
}

int main() {
    int n, m;
    while (readInt(n) && readInt(m)) {
        if (n == 0 && m == 0) break;
        int W = (n + 32) / 32;
        for (int i = 1; i <= n; i++) memset(hate[i], 0, (size_t)W * sizeof(unsigned int));
        for (int i = 0; i < m; i++) {
            int a, b;
            if (!readInt(a)) a = 0;
            if (!readInt(b)) b = 0;
            if (a >= 1 && a <= n && b >= 1 && b <= n && a != b) {
                hate[a][b >> 5] |= 1u << (b & 31);
                hate[b][a >> 5] |= 1u << (a & 31);
            }
        }
        /* complement graph in CSR form */
        int pos = 0;
        for (int u = 1; u <= n; u++) {
            start_[u] = pos;
            const unsigned int *hu = hate[u];
            for (int v = 1; v <= n; v++) {
                if (v == u) continue;
                if (!(hu[v >> 5] & (1u << (v & 31)))) adjv[pos++] = v;
            }
        }
        start_[n + 1] = pos;

        for (int i = 1; i <= n; i++) { disc[i] = 0; okv[i] = false; }
        int timer = 0;
        esTop = 0;
        for (int s = 1; s <= n; s++) {
            if (disc[s]) continue;
            int top = 0;
            disc[s] = low[s] = ++timer;
            iter_[s] = start_[s];
            par[s] = 0;
            stk[top++] = s;
            while (top > 0) {
                int u = stk[top - 1];
                if (iter_[u] < start_[u + 1]) {
                    int v = adjv[iter_[u]++];
                    if (v == par[u]) continue;
                    if (!disc[v]) {
                        es_a[esTop] = u; es_b[esTop] = v; esTop++;
                        par[v] = u;
                        disc[v] = low[v] = ++timer;
                        iter_[v] = start_[v];
                        stk[top++] = v;
                    } else if (disc[v] < disc[u]) {
                        es_a[esTop] = u; es_b[esTop] = v; esTop++;
                        if (disc[v] < low[u]) low[u] = disc[v];
                    }
                } else {
                    top--;
                    if (top > 0) {
                        int p = stk[top - 1];
                        if (low[u] < low[p]) low[p] = low[u];
                        if (low[u] >= disc[p]) {
                            int i = esTop - 1;
                            while (!(es_a[i] == p && es_b[i] == u)) i--;
                            processBCC(i, esTop);
                            esTop = i;
                        }
                    }
                }
            }
        }
        int expelled = 0;
        for (int i = 1; i <= n; i++) if (!okv[i]) expelled++;
        printf("%d\n", expelled);
    }
    return 0;
}
