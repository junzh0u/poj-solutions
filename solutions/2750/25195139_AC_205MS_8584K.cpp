// POJ 2750 - Potted Flower
// Model: claude-opus-5
//
// N flowers on a circle, M point updates; after each update report the largest
// sum over a contiguous arc whose length is 1..N-1 (the full circle is banned --
// that ban is the trap the discuss board is full of).
//
// Segment tree over the linear array, each node holding sum plus the max and min
// prefix / suffix / subarray sums (all over non-empty ranges), merged the usual way.
//
// Handling the "not the whole circle" ban exactly, without a special case:
//   * A valid non-wrapping arc is a linear interval of length <= N-1, so it misses
//     some index, so it lies entirely inside [1, N-1] or entirely inside [2, N].
//     Hence maxProper = max(maxSub[1..N-1], maxSub[2..N]) -- querying those two
//     ranges instead of the root is what keeps the whole array out of the answer.
//   * A valid wrapping arc is the complement of a linear interval of length
//     1..N-1, so its best value is total - minProper, with minProper taken over
//     the same two ranges. The complement then also has length 1..N-1.
// Every valid arc is in exactly one of the two families, so the answer is
// max(maxProper, total - minProper) with no correction needed.
//
// Sums are bounded by 1e5 * 1000 = 1e8, so int suffices.

#include <cstdio>

const int MAXN = 100005;

struct Node {
    int sum;
    int maxPre, maxSuf, maxSub;
    int minPre, minSuf, minSub;
};

static Node tr[MAXN * 4];
static int a[MAXN];
static int n, m;

static Node makeLeaf(int v) {
    Node r;
    r.sum = v;
    r.maxPre = r.maxSuf = r.maxSub = v;
    r.minPre = r.minSuf = r.minSub = v;
    return r;
}

static Node merge(const Node &L, const Node &R) {
    Node r;
    r.sum = L.sum + R.sum;

    r.maxPre = L.maxPre > L.sum + R.maxPre ? L.maxPre : L.sum + R.maxPre;
    r.maxSuf = R.maxSuf > R.sum + L.maxSuf ? R.maxSuf : R.sum + L.maxSuf;
    r.maxSub = L.maxSub > R.maxSub ? L.maxSub : R.maxSub;
    int cross = L.maxSuf + R.maxPre;
    if (cross > r.maxSub) r.maxSub = cross;

    r.minPre = L.minPre < L.sum + R.minPre ? L.minPre : L.sum + R.minPre;
    r.minSuf = R.minSuf < R.sum + L.minSuf ? R.minSuf : R.sum + L.minSuf;
    r.minSub = L.minSub < R.minSub ? L.minSub : R.minSub;
    int crossMin = L.minSuf + R.minPre;
    if (crossMin < r.minSub) r.minSub = crossMin;

    return r;
}

static void build(int node, int l, int r) {
    if (l == r) { tr[node] = makeLeaf(a[l]); return; }
    int mid = (l + r) >> 1;
    build(node << 1, l, mid);
    build(node << 1 | 1, mid + 1, r);
    tr[node] = merge(tr[node << 1], tr[node << 1 | 1]);
}

static void update(int node, int l, int r, int pos, int val) {
    if (l == r) { tr[node] = makeLeaf(val); return; }
    int mid = (l + r) >> 1;
    if (pos <= mid) update(node << 1, l, mid, pos, val);
    else update(node << 1 | 1, mid + 1, r, pos, val);
    tr[node] = merge(tr[node << 1], tr[node << 1 | 1]);
}

static Node query(int node, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return tr[node];
    int mid = (l + r) >> 1;
    if (qr <= mid) return query(node << 1, l, mid, ql, qr);
    if (ql > mid) return query(node << 1 | 1, mid + 1, r, ql, qr);
    return merge(query(node << 1, l, mid, ql, qr),
                 query(node << 1 | 1, mid + 1, r, ql, qr));
}

static int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) {
        if (c == EOF) return 0;
        c = getchar();
    }
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sign;
}

int main() {
    n = readInt();
    int i;
    int total = 0;
    for (i = 1; i <= n; ++i) { a[i] = readInt(); total += a[i]; }
    build(1, 1, n);
    m = readInt();
    for (i = 0; i < m; ++i) {
        int p = readInt();
        int v = readInt();
        total += v - a[p];
        a[p] = v;
        update(1, 1, n, p, v);

        Node A = query(1, 1, n, 1, n - 1);
        Node B = query(1, 1, n, 2, n);
        int maxProper = A.maxSub > B.maxSub ? A.maxSub : B.maxSub;
        int minProper = A.minSub < B.minSub ? A.minSub : B.minSub;
        int wrap = total - minProper;
        int ans = maxProper > wrap ? maxProper : wrap;
        printf("%d\n", ans);
    }
    return 0;
}
