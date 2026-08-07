// POJ 2201 - Cartesian Tree
// Model: claude-opus-5
//
// Build the (unique) treap over n pairs (k, a): a BST on the main key k whose
// auxiliary keys a satisfy the min-heap condition, then report parent/left/right
// for every node in input order.
//
// Approach: sort the nodes by k ascending and insert them in that order, keeping
// the tree's rightmost spine on a stack. A new node w always ends up on the
// rightmost spine, because its key exceeds every key inserted so far. Pop every
// spine node whose auxiliary key is greater than a[w]; the last one popped is a
// whole subtree of smaller keys that must sit below w, so it becomes w's left
// child, and the new spine top (if any) adopts w as its right child. Each node is
// pushed and popped at most once, so the construction is O(n) after the O(n log n)
// sort -- the discuss board's TLE reports come from rescanning the rightmost path
// per insertion instead, which degrades to O(n^2) on a sorted-a input.
//
// Ambiguity: the statement asks to "detect that it is not possible" and the output
// spec allows NO, but it also guarantees all k distinct and all a distinct. Under
// that guarantee the cartesian tree always exists and is unique (induction: the
// unique minimum-a node must be the root, and the BST condition then forces the
// split by k into two independent subproblems), so NO is unreachable and this
// program always prints YES. The discuss board agrees no test triggers NO.
//
// The problem page advertises a per-case time limit, so the input is read to EOF
// and each data set handled independently. Hand-rolled buffered I/O: the output is
// up to 50 000 lines per case, which is where the board's other TLEs come from.

#include <cstdio>
#include <algorithm>

static const int MAXN = 50005;

struct Node {
    int k, a, id;
};

static bool byKey(const Node& x, const Node& y) { return x.k < y.k; }

static Node nd[MAXN];
static int par[MAXN], lch[MAXN], rch[MAXN];
static int stkId[MAXN], stkA[MAXN];

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static int readInt(int* out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return 0;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    *out = v * sgn;
    return 1;
}

static char obuf[1 << 16];
static int opos = 0;

static void flushOut() {
    fwrite(obuf, 1, opos, stdout);
    opos = 0;
}

static void putCh(char c) {
    if (opos == (int)sizeof(obuf)) flushOut();
    obuf[opos++] = c;
}

static void putInt(int v) {
    char t[12];
    int n = 0;
    if (v == 0) { putCh('0'); return; }
    while (v > 0) { t[n++] = (char)('0' + v % 10); v /= 10; }
    while (n > 0) putCh(t[--n]);
}

int main() {
    int n;
    while (readInt(&n)) {
        int i;
        for (i = 0; i < n; ++i) {
            readInt(&nd[i].k);
            readInt(&nd[i].a);
            nd[i].id = i + 1;
        }
        for (i = 1; i <= n; ++i) { par[i] = 0; lch[i] = 0; rch[i] = 0; }
        std::sort(nd, nd + n, byKey);

        int top = 0;
        for (i = 0; i < n; ++i) {
            int cur = nd[i].id;
            int last = 0;
            while (top > 0 && stkA[top - 1] > nd[i].a) last = stkId[--top];
            if (top > 0) {
                rch[stkId[top - 1]] = cur;
                par[cur] = stkId[top - 1];
            }
            if (last) {
                lch[cur] = last;
                par[last] = cur;
            }
            stkId[top] = cur;
            stkA[top] = nd[i].a;
            ++top;
        }

        putCh('Y'); putCh('E'); putCh('S'); putCh('\n');
        for (i = 1; i <= n; ++i) {
            putInt(par[i]); putCh(' ');
            putInt(lch[i]); putCh(' ');
            putInt(rch[i]); putCh('\n');
        }
    }
    flushOut();
    return 0;
}
