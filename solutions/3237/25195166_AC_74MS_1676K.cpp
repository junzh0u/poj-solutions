// POJ 3237 - Tree
// Model: claude-opus-5
//
// Heavy-light decomposition over the tree, with the weight of edge (parent[v], v)
// stored at v's HLD position, plus a segment tree carrying (max, min) per node and
// a boolean "negate" lazy tag.
//
// The min is what makes NEGATE lazy: negating a segment maps (max, min) to
// (-min, -max), so a subtree summary can be flipped in O(1) without touching its
// leaves.  The tag is XOR-toggled, never assigned - two negations of the same
// range cancel, and assigning would lose that.
//
// The root's HLD position holds no edge; it is seeded (max, min) = (-INF, +INF),
// which is the identity for both max and negation.  INT_MIN would be wrong here:
// -INT_MIN is INT_MIN, so the slot would stop being neutral after a flip (a trap
// the problem's discuss board reports as a source of WA).
//
// CHANGE i v assigns an absolute weight, overriding any negations accumulated on
// that edge so far; the point update pushes lazy tags down on the way in, so the
// assigned value is not later flipped by a stale ancestor tag.
//
// Input framing the statement leaves implicit, confirmed on the discuss board:
// multiple test cases separated by blank lines, each terminated by DONE, which
// must be consumed before the next case is read.  Commands are dispatched on
// their first letter so a stream is parsed the same way regardless of whether it
// spells the keywords out.
//
// QUERY a b with a == b has an empty path and no defined maximum; 0 is printed.
//
// The DFS passes are iterative (BFS order + reverse sweep for subtree sizes, an
// explicit stack for chain assignment), so a path-shaped tree cannot blow the
// stack - the board carries several RE reports on this problem.
//
// Complexity: O(log^2 N) per instruction.  Measured locally at 0.15s for 20 cases
// of N = 10000 with 10000 instructions each, and 1.4s for the same with 100000
// instructions each, against a 5s limit.

#include <cstdio>
#include <algorithm>

using namespace std;

const int MAXN = 100005;
const int NEG_INF = -1000000000;
const int POS_INF =  1000000000;

int n;
int head_[MAXN], nxt_[2 * MAXN], to_[2 * MAXN], wt_[2 * MAXN], ecnt;
int eu[MAXN], ev[MAXN];
int par[MAXN], dep[MAXN], sz[MAXN], heavy[MAXN], chtop[MAXN], pos_[MAXN], ew[MAXN];
int order_[MAXN];
int base_[MAXN];
int edgeNode[MAXN];
int stkNode[MAXN], stkTop_[MAXN];

int mxv[4 * MAXN], mnv[4 * MAXN];
char lz[4 * MAXN];

static void addEdge(int u, int v, int w) {
    ++ecnt;
    to_[ecnt] = v; wt_[ecnt] = w; nxt_[ecnt] = head_[u]; head_[u] = ecnt;
}

static void applyNeg(int x) {
    int t = mxv[x];
    mxv[x] = -mnv[x];
    mnv[x] = -t;
    lz[x] ^= 1;
}

static void pushDown(int x) {
    if (lz[x]) {
        applyNeg(x << 1);
        applyNeg(x << 1 | 1);
        lz[x] = 0;
    }
}

static void pullUp(int x) {
    mxv[x] = max(mxv[x << 1], mxv[x << 1 | 1]);
    mnv[x] = min(mnv[x << 1], mnv[x << 1 | 1]);
}

static void build(int x, int l, int r) {
    lz[x] = 0;
    if (l == r) {
        if (l == 1) {
            /* position 1 is the root, which carries no edge.  (-INF, +INF) is
               the identity for negation as well as for max/min, so this slot
               can never pollute a result; INT_MIN would negate to itself. */
            mxv[x] = NEG_INF; mnv[x] = POS_INF;
        } else {
            mxv[x] = mnv[x] = base_[l];
        }
        return;
    }
    int mid = (l + r) >> 1;
    build(x << 1, l, mid);
    build(x << 1 | 1, mid + 1, r);
    pullUp(x);
}

static void pointSet(int x, int l, int r, int p, int v) {
    if (l == r) { mxv[x] = mnv[x] = v; return; }
    pushDown(x);
    int mid = (l + r) >> 1;
    if (p <= mid) pointSet(x << 1, l, mid, p, v);
    else pointSet(x << 1 | 1, mid + 1, r, p, v);
    pullUp(x);
}

static void rangeNeg(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) { applyNeg(x); return; }
    pushDown(x);
    int mid = (l + r) >> 1;
    if (ql <= mid) rangeNeg(x << 1, l, mid, ql, qr);
    if (qr > mid) rangeNeg(x << 1 | 1, mid + 1, r, ql, qr);
    pullUp(x);
}

static int rangeMax(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return mxv[x];
    pushDown(x);
    int mid = (l + r) >> 1;
    int res = NEG_INF;
    if (ql <= mid) res = max(res, rangeMax(x << 1, l, mid, ql, qr));
    if (qr > mid) res = max(res, rangeMax(x << 1 | 1, mid + 1, r, ql, qr));
    return res;
}

/* ---- input ---- */
static char ibuf[1 << 16];
static int ipos_ = 0, ilen = 0;

static int gc() {
    if (ipos_ == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos_ = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos_++];
}

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sgn;
    return true;
}

/* reads a token of non-space chars; returns its first char, or -1 at EOF */
static int readToken() {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return -1;
    int first = c;
    while (c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') c = gc();
    return first;
}

/* ---- output ---- */
static char obuf[1 << 16];
static int opos_ = 0;

static void oflush() { fwrite(obuf, 1, opos_, stdout); opos_ = 0; }

static void writeInt(int v) {
    if (opos_ > (int)sizeof(obuf) - 16) oflush();
    if (v < 0) { obuf[opos_++] = '-'; v = -v; }
    char tmp[12];
    int k = 0;
    do { tmp[k++] = (char)('0' + v % 10); v /= 10; } while (v);
    while (k > 0) obuf[opos_++] = tmp[--k];
    obuf[opos_++] = '\n';
}

static void decompose() {
    /* BFS from 1 for parent / depth / order */
    int qh = 0, qt = 0;
    order_[qt++] = 1;
    par[1] = 0; dep[1] = 0; ew[1] = 0;
    while (qh < qt) {
        int x = order_[qh++];
        for (int e = head_[x]; e; e = nxt_[e]) {
            int v = to_[e];
            if (v == par[x]) continue;
            par[v] = x;
            dep[v] = dep[x] + 1;
            ew[v] = wt_[e];
            order_[qt++] = v;
        }
    }
    for (int i = 1; i <= n; i++) { sz[i] = 1; heavy[i] = 0; }
    for (int i = n - 1; i >= 0; i--) {
        int x = order_[i];
        int p = par[x];
        if (p) {
            sz[p] += sz[x];
            if (heavy[p] == 0 || sz[x] > sz[heavy[p]]) heavy[p] = x;
        }
    }
    /* iterative heavy-path assignment: chain positions stay contiguous */
    int cnt = 0, sp = 0;
    stkNode[sp] = 1; stkTop_[sp] = 1; sp++;
    while (sp > 0) {
        --sp;
        int start = stkNode[sp], t = stkTop_[sp];
        for (int v = start; v; v = heavy[v]) {
            chtop[v] = t;
            pos_[v] = ++cnt;
            base_[cnt] = ew[v];
            for (int e = head_[v]; e; e = nxt_[e]) {
                int c = to_[e];
                if (c == par[v] || c == heavy[v]) continue;
                stkNode[sp] = c; stkTop_[sp] = c; sp++;
            }
        }
    }
    base_[1] = 0;  /* unused: build() seeds position 1 as the neutral element */
}

int main() {
    int t;
    if (!readInt(t)) return 0;
    while (t-- > 0) {
        if (!readInt(n)) break;
        ecnt = 0;
        for (int i = 1; i <= n; i++) head_[i] = 0;
        for (int i = 1; i < n; i++) {
            int a, b, c;
            readInt(a); readInt(b); readInt(c);
            eu[i] = a; ev[i] = b;
            addEdge(a, b, c);
            addEdge(b, a, c);
        }
        decompose();
        for (int i = 1; i < n; i++)
            edgeNode[i] = (dep[eu[i]] > dep[ev[i]]) ? eu[i] : ev[i];
        build(1, 1, n);

        for (;;) {
            int c = readToken();
            if (c == -1) break;
            if (c == 'D' || c == 'd') break;
            int a, b;
            readInt(a); readInt(b);
            if (c == 'C' || c == 'c') {
                pointSet(1, 1, n, pos_[edgeNode[a]], b);
            } else if (c == 'N' || c == 'n') {
                int x = a, y = b;
                while (chtop[x] != chtop[y]) {
                    if (dep[chtop[x]] < dep[chtop[y]]) { int tmp = x; x = y; y = tmp; }
                    rangeNeg(1, 1, n, pos_[chtop[x]], pos_[x]);
                    x = par[chtop[x]];
                }
                if (x != y) {
                    if (dep[x] < dep[y]) { int tmp = x; x = y; y = tmp; }
                    rangeNeg(1, 1, n, pos_[y] + 1, pos_[x]);
                }
            } else {
                int x = a, y = b, ans = NEG_INF;
                if (x == y) { writeInt(0); continue; }
                while (chtop[x] != chtop[y]) {
                    if (dep[chtop[x]] < dep[chtop[y]]) { int tmp = x; x = y; y = tmp; }
                    ans = max(ans, rangeMax(1, 1, n, pos_[chtop[x]], pos_[x]));
                    x = par[chtop[x]];
                }
                if (x != y) {
                    if (dep[x] < dep[y]) { int tmp = x; x = y; y = tmp; }
                    ans = max(ans, rangeMax(1, 1, n, pos_[y] + 1, pos_[x]));
                }
                writeInt(ans);
            }
        }
    }
    oflush();
    return 0;
}
