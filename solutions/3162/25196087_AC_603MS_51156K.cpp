// POJ 3162 - Walking Race
// Model: claude-opus-5
//
// Day i's walk is the longest simple path starting at check-point i, i.e. the
// eccentricity ecc[i] of node i in the tree. The answer is the length of the
// longest run of CONSECUTIVE days (consecutive node indices) whose ecc values
// have max - min <= M.
//
// ecc: the farthest node from any node is an endpoint of some diameter, so three
// BFS passes suffice -- from node 1 to find diameter endpoint a, from a to find
// endpoint b, from b -- and ecc[i] = max(dist_a[i], dist_b[i]).  Weights are
// non-negative, which is what makes that property hold.
// Window: two monotone deques (max and min over the window) with a left pointer
// that jumps past whichever front index is smaller whenever the spread exceeds M.
// Overall O(N).
//
// Statement leaves d_i's range unspecified, so distances are kept in long long:
// with N = 10^6 a path whose weights exceed ~2000 already overflows 32 bits.
// That is why this goes out under G++ rather than C++.
//
// Constant factor matters here (the discuss board is full of TLE reports that
// went away when vector<> adjacency was replaced by flat arrays): the graph is a
// CSR built in place, input is a fread scanner, and recursion is avoided entirely
// so a 10^6-node path cannot blow the stack.

#include <cstdio>

static const int MAXN = 1100005;
static const int MAXE = 2 * MAXN;

static int par[MAXN], wpar[MAXN];
static int st[MAXN + 2];
static int adjTo[MAXE], adjW[MAXE];
static long long da[MAXN], db[MAXN];
static int que[MAXN];
static int qmax[MAXN], qmin[MAXN];

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

static int n;

static void bfs(int s, long long *d) {
    for (int i = 1; i <= n; ++i) d[i] = -1;
    int head = 0, tail = 0;
    d[s] = 0;
    que[tail++] = s;
    while (head < tail) {
        int u = que[head++];
        long long du = d[u];
        int e = st[u], en = st[u + 1];
        for (; e < en; ++e) {
            int v = adjTo[e];
            if (d[v] < 0) {
                d[v] = du + adjW[e];
                que[tail++] = v;
            }
        }
    }
}

int main() {
    int m;
    if (!readInt(n)) return 0;
    readInt(m);
    /* Edge i (i = 1..N-1) joins node i+1 to f_i, so f_i is exactly the parent of
       i+1 when the tree is rooted at 1 -- but nothing says f_i < i+1, so build a
       real undirected adjacency rather than assuming an ordered parent array. */
    for (int v = 2; v <= n; ++v) {
        readInt(par[v]);
        readInt(wpar[v]);
    }

    for (int v = 0; v <= n + 1; ++v) st[v] = 0;
    for (int v = 2; v <= n; ++v) { ++st[v]; ++st[par[v]]; }
    int s = 0;
    for (int v = 1; v <= n; ++v) { s += st[v]; st[v] = s; }  /* st[v] = block end */
    for (int v = 2; v <= n; ++v) {
        int u = par[v], w = wpar[v];
        adjTo[--st[v]] = u; adjW[st[v]] = w;
        adjTo[--st[u]] = v; adjW[st[u]] = w;
    }
    st[n + 1] = s;  /* after the decrements st[v] is the block start of v */

    bfs(1, da);
    int a = 1;
    for (int i = 1; i <= n; ++i) if (da[i] > da[a]) a = i;
    bfs(a, da);
    int b = 1;
    for (int i = 1; i <= n; ++i) if (da[i] > da[b]) b = i;
    bfs(b, db);
    for (int i = 1; i <= n; ++i) if (db[i] > da[i]) da[i] = db[i];

    long long *f = da;
    int hx = 0, tx = 0, hn = 0, tn = 0;
    int l = 1, ans = 0;
    for (int r = 1; r <= n; ++r) {
        while (tx > hx && f[qmax[tx - 1]] <= f[r]) --tx;
        qmax[tx++] = r;
        while (tn > hn && f[qmin[tn - 1]] >= f[r]) --tn;
        qmin[tn++] = r;
        while (f[qmax[hx]] - f[qmin[hn]] > (long long)m) {
            if (qmax[hx] < qmin[hn]) l = qmax[hx++] + 1;
            else l = qmin[hn++] + 1;
        }
        if (r - l + 1 > ans) ans = r - l + 1;
    }
    printf("%d\n", ans);
    return 0;
}
