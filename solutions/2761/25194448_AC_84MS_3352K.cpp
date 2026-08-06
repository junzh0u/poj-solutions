// POJ 2761 - Feed the dogs
//
// k-th smallest value in a range, answered offline.
//
// The statement's key promise is that "any feeding interval will not contain
// another completely" -- the queried intervals may overlap but never nest.
// Sorting them by left endpoint therefore also sorts them by right endpoint,
// so a single sliding window can walk over all queries in order: both pointers
// only ever move forward, for a total of O(n) index moves rather than the
// persistent structure a general k-th-in-range query would need.
//
// The window's multiset lives in a Fenwick tree indexed by compressed value
// (values are arbitrary ints and may repeat, so ranks come from sort+unique
// and duplicates simply pile up counts on the same index). The k-th smallest
// is then the smallest prefix whose sum reaches k, found by binary lifting
// over the tree in O(log n) -- no extra descent structure required.
//
// Robustness note: the window is grown on both sides before it is shrunk on
// either, so the code still answers correctly if the input ever violates the
// non-nesting promise (it would just lose the linear bound on pointer moves).
// kth() also clamps when k exceeds the window size, which the statement never
// defines; a well-formed input never triggers it.
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

static const int MAXN = 100005;
static const int MAXM = 50005;

int n, m;
int a[MAXN];      /* raw pretty values */
int rnk[MAXN];    /* 1-based rank of a[i] among distinct sorted values */
int vals[MAXN];   /* distinct sorted values */
int nv;           /* number of distinct values */

int bit[MAXN];
int LOG;          /* highest power of two <= nv */

inline void upd(int i, int d) {
    for (; i <= nv; i += i & (-i)) bit[i] += d;
}

/* smallest index p with prefix_sum(p) >= k, via binary lifting */
inline int kth(int k) {
    int pos = 0;
    for (int pw = LOG; pw; pw >>= 1) {
        if (pos + pw <= nv && bit[pos + pw] < k) {
            pos += pw;
            k -= bit[pos];
        }
    }
    if (pos >= nv) return nv; /* guard: k larger than the window size */
    return pos + 1;
}

struct Query {
    int l, r, k, id;
};

Query q[MAXM];
int ans[MAXM];

static bool cmpQ(const Query &x, const Query &y) {
    if (x.l != y.l) return x.l < y.l;
    return x.r < y.r;
}

/* fast input: values may be negative */
static char buf[1 << 16];
static int bufLen = 0, bufPos = 0;

inline int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return buf[bufPos++];
}

inline bool readInt(int &out) {
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

int main() {
    while (readInt(n)) {
        if (!readInt(m)) break;
        int i;
        for (i = 1; i <= n; ++i) readInt(a[i]);
        for (i = 1; i <= n; ++i) vals[i - 1] = a[i];
        sort(vals, vals + n);
        nv = (int)(unique(vals, vals + n) - vals);
        for (i = 1; i <= n; ++i)
            rnk[i] = (int)(lower_bound(vals, vals + nv, a[i]) - vals) + 1;

        for (i = 0; i < m; ++i) {
            readInt(q[i].l);
            readInt(q[i].r);
            readInt(q[i].k);
            q[i].id = i;
        }
        sort(q, q + m, cmpQ);

        for (i = 0; i <= nv; ++i) bit[i] = 0;
        LOG = 1;
        while ((LOG << 1) <= nv) LOG <<= 1;

        int curL = 1, curR = 0;
        for (i = 0; i < m; ++i) {
            int l = q[i].l, r = q[i].r;
            /* grow first, then shrink: the window stays consistent even if
               the input violates the "no interval contains another" promise */
            while (curL > l) { --curL; upd(rnk[curL], 1); }
            while (curR < r) { ++curR; upd(rnk[curR], 1); }
            while (curL < l) { upd(rnk[curL], -1); ++curL; }
            while (curR > r) { upd(rnk[curR], -1); --curR; }
            ans[q[i].id] = vals[kth(q[i].k) - 1];
        }
        for (i = 0; i < m; ++i) printf("%d\n", ans[i]);

        /* clear the tree for a possible next test case */
        for (i = curL; i <= curR; ++i) upd(rnk[i], -1);
    }
    return 0;
}
