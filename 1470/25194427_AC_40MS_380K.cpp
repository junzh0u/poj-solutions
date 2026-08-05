// POJ 1470 - Closest Common Ancestors
//
// For each dataset: read a rooted tree on n <= 900 vertices, then a list of
// pairs; for every pair count the LCA, and print "vertex:count" in ascending
// vertex order for the vertices that were an LCA at least once.
//
// Parsing is the real difficulty. The statement wraps everything in
// punctuation ("v:(k) c1 c2 ...", "(u v)"), says white-space may be used
// freely, and the vertex lines may arrive in any order. But the input holds
// nothing except non-negative integers, so a tokenizer that skips every
// non-digit character and then reads a run of digits is both complete and
// format-agnostic: ':', '(', ')', spaces, tabs and newlines are all just
// separators. No scanf pattern is needed, and "5:(3)1 4 2" with no spaces at
// all parses identically to the spaced form.
//
// Algorithm: build the child adjacency, find the root as the one vertex that
// is nobody's successor, BFS (not recursion - a chain can be 899 deep) to get
// parent and depth, then binary lifting with LOG=10 for O(log n) LCA. Queries
// are answered independently, so a repeated pair is counted once per
// occurrence, which is what the sample requires. Datasets repeat until EOF.
//
// Ambiguities resolved: a node is its own ancestor, so (u u) answers u and a
// pair where one node is an ancestor of the other answers that ancestor;
// vertices never used as an LCA are omitted from the output entirely (the
// sample prints only 2 and 5, not 1, 3, 4); nothing separates one dataset's
// output from the next.
#include <cstdio>
#include <cstring>

static const int MAXN = 905;
static const int LOG = 10;

static int head_[MAXN], nxt_[MAXN], to_[MAXN];
static int par_[MAXN], dep_[MAXN];
static int up_[LOG][MAXN];
static int cnt_[MAXN];
static bool isChild_[MAXN];
static int order_[MAXN];

static char buf[1 << 16];
static int blen = 0, bpos = 0;

static int gc() {
    if (bpos == blen) {
        blen = (int)fread(buf, 1, sizeof(buf), stdin);
        bpos = 0;
        if (blen <= 0) return -1;
    }
    return (unsigned char)buf[bpos++];
}

/* The input is nothing but non-negative integers separated by arbitrary
   punctuation and whitespace, so skipping every non-digit is a complete
   and format-agnostic tokenizer. */
static bool readInt(int &x) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = gc();
    }
    return true;
}

static int lca(int u, int v) {
    if (dep_[u] < dep_[v]) { int t = u; u = v; v = t; }
    int d = dep_[u] - dep_[v];
    for (int j = 0; j < LOG; j++)
        if ((d >> j) & 1) u = up_[j][u];
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; j--)
        if (up_[j][u] != up_[j][v]) { u = up_[j][u]; v = up_[j][v]; }
    return up_[0][u];
}

int main() {
    int n;
    while (readInt(n)) {
        if (n < 1) n = 0;
        if (n > MAXN - 1) n = MAXN - 1;
        int i;
        for (i = 0; i <= n; i++) {
            head_[i] = -1;
            isChild_[i] = false;
            cnt_[i] = 0;
            par_[i] = 0;
            dep_[i] = 0;
        }
        int ec = 0;
        for (i = 0; i < n; i++) {
            int v, k;
            if (!readInt(v)) return 0;
            if (!readInt(k)) return 0;
            for (int j = 0; j < k; j++) {
                int s;
                if (!readInt(s)) return 0;
                if (v < 1 || v > n || s < 1 || s > n) continue;
                to_[ec] = s;
                nxt_[ec] = head_[v];
                head_[v] = ec;
                ec++;
                isChild_[s] = true;
            }
        }
        int root = 1;
        for (i = 1; i <= n; i++)
            if (!isChild_[i]) { root = i; break; }

        int qh = 0, qt = 0;
        order_[qt++] = root;
        par_[root] = root;
        dep_[root] = 0;
        while (qh < qt) {
            int u = order_[qh++];
            for (int e = head_[u]; e != -1; e = nxt_[e]) {
                int v = to_[e];
                par_[v] = u;
                dep_[v] = dep_[u] + 1;
                order_[qt++] = v;
            }
        }

        for (i = 0; i <= n; i++) up_[0][i] = par_[i];
        up_[0][0] = 0;
        for (int j = 1; j < LOG; j++)
            for (i = 0; i <= n; i++) up_[j][i] = up_[j - 1][up_[j - 1][i]];

        int m;
        if (!readInt(m)) m = 0;
        for (i = 0; i < m; i++) {
            int u, v;
            if (!readInt(u)) break;
            if (!readInt(v)) break;
            if (u < 1 || u > n || v < 1 || v > n) continue;
            cnt_[lca(u, v)]++;
        }

        for (i = 1; i <= n; i++)
            if (cnt_[i]) printf("%d:%d\n", i, cnt_[i]);
    }
    return 0;
}
