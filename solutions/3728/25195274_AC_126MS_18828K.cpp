// POJ 3728 - The merchant
// Model: claude-opus-5
//
// Tree with N <= 50000 cities, each with a price; Q <= 50000 queries ask, for
// the directed simple path u -> v, the maximum of price[sell] - price[buy]
// where the buy city comes strictly before the sell city along that path
// (0 when no positive profit exists).  The path is directed: query "2 3" and
// query "3 2" have different answers in the official sample.
//
// Approach: binary lifting over the tree rooted at 1.  For each node x and
// each k, the table entry [k][x] summarises the vertical chain of 2^k nodes
// x, parent(x), ..., anc(x, 2^k - 1) with four values:
//   mx / mn  - max / min price on the chain
//   up       - best profit travelling the chain deep -> shallow
//   dn       - best profit travelling the chain shallow -> deep
// Two chains A (deeper) and B (shallower) merge in O(1):
//   up = max(up A, up B, mx B - mn A),  dn = max(dn A, dn B, mx A - mn B).
// A query splits at l = LCA(u, v) into A = u..l (walked upward) and
// B = v..l (walked upward, but traversed downward on the real path), so
//   answer = max(0, up A, dn B, mx B - mn A).
// The cross term mx B - mn A is the part naive solutions drop: buying on the
// ascending half and selling on the descending half.  l belongs to both
// chains, but a buy and sell both at l yields 0, so the overlap is harmless.
//
// The tree can be a path of 50000 nodes, so the rooting pass is an iterative
// BFS rather than a recursive DFS.  All values fit comfortably in int
// (prices <= 50000); the sentinel node 0 uses +-1e8 so that unused table
// entries cannot overflow.

#include <cstdio>

const int MAXN = 50005;
const int LOG = 17;
const int NEG = -100000000;
const int POS = 100000000;

static int n, q;
static int w[MAXN];
static int head[MAXN], nxt[2 * MAXN], dst[2 * MAXN];
static int par[LOG][MAXN];
static int mxT[LOG][MAXN], mnT[LOG][MAXN], upT[LOG][MAXN], dnT[LOG][MAXN];
static int dep[MAXN], bfs[MAXN];
static int ecnt = 0;

static inline int imax(int a, int b) { return a > b ? a : b; }
static inline int imin(int a, int b) { return a < b ? a : b; }

/* ---- fast input ---- */
static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;
static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}
static inline int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x * sgn;
}

static void addEdge(int a, int b) {
    dst[ecnt] = b; nxt[ecnt] = head[a]; head[a] = ecnt++;
}

struct Info { int mx, mn, up, dn; };

/* chain from x up to ancestor a, both inclusive, stored deep -> shallow */
static Info climb(int x, int a) {
    int len = dep[x] - dep[a] + 1;
    Info r;
    r.mx = NEG; r.mn = POS; r.up = 0; r.dn = 0;
    bool first = true;
    for (int k = 0; k < LOG; ++k) {
        if (((len >> k) & 1) == 0) continue;
        if (first) {
            r.mx = mxT[k][x]; r.mn = mnT[k][x];
            r.up = upT[k][x]; r.dn = dnT[k][x];
            first = false;
        } else {
            /* r is the deeper part, table entry [k][x] the shallower part */
            int nmx = imax(r.mx, mxT[k][x]);
            int nmn = imin(r.mn, mnT[k][x]);
            int nup = imax(imax(r.up, upT[k][x]), mxT[k][x] - r.mn);
            int ndn = imax(imax(r.dn, dnT[k][x]), r.mx - mnT[k][x]);
            r.mx = nmx; r.mn = nmn; r.up = nup; r.dn = ndn;
        }
        x = par[k][x];
    }
    return r;
}

static int lca(int u, int v) {
    if (dep[u] < dep[v]) { int t = u; u = v; v = t; }
    int d = dep[u] - dep[v];
    for (int k = 0; k < LOG; ++k)
        if ((d >> k) & 1) u = par[k][u];
    if (u == v) return u;
    for (int k = LOG - 1; k >= 0; --k)
        if (par[k][u] != par[k][v]) { u = par[k][u]; v = par[k][v]; }
    return par[0][u];
}

int main() {
    n = readInt();
    for (int i = 1; i <= n; ++i) w[i] = readInt();
    for (int i = 0; i <= n; ++i) head[i] = -1;
    for (int i = 0; i < n - 1; ++i) {
        int a = readInt(), b = readInt();
        addEdge(a, b); addEdge(b, a);
    }

    /* iterative BFS: no recursion, depth can reach 50000 */
    int qh = 0, qt = 0;
    bfs[qt++] = 1; dep[1] = 0; par[0][1] = 0;
    while (qh < qt) {
        int u = bfs[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = dst[e];
            if (v == par[0][u]) continue;
            par[0][v] = u;
            dep[v] = dep[u] + 1;
            bfs[qt++] = v;
        }
    }

    mxT[0][0] = NEG; mnT[0][0] = POS; upT[0][0] = 0; dnT[0][0] = 0;
    for (int k = 0; k < LOG; ++k) par[k][0] = 0;
    for (int i = 1; i <= n; ++i) {
        mxT[0][i] = mnT[0][i] = w[i];
        upT[0][i] = dnT[0][i] = 0;
    }
    for (int k = 1; k < LOG; ++k) {
        mxT[k][0] = NEG; mnT[k][0] = POS; upT[k][0] = 0; dnT[k][0] = 0;
        for (int i = 1; i <= n; ++i) {
            int m = par[k - 1][i];
            mxT[k][i] = imax(mxT[k - 1][i], mxT[k - 1][m]);
            mnT[k][i] = imin(mnT[k - 1][i], mnT[k - 1][m]);
            upT[k][i] = imax(imax(upT[k - 1][i], upT[k - 1][m]),
                             mxT[k - 1][m] - mnT[k - 1][i]);
            dnT[k][i] = imax(imax(dnT[k - 1][i], dnT[k - 1][m]),
                             mxT[k - 1][i] - mnT[k - 1][m]);
            par[k][i] = par[k - 1][m];
        }
    }

    q = readInt();
    for (int i = 0; i < q; ++i) {
        int u = readInt(), v = readInt();
        int l = lca(u, v);
        Info A = climb(u, l);
        Info B = climb(v, l);
        int ans = 0;
        if (A.up > ans) ans = A.up;
        if (B.dn > ans) ans = B.dn;
        if (B.mx - A.mn > ans) ans = B.mx - A.mn;
        printf("%d\n", ans);
    }
    return 0;
}
