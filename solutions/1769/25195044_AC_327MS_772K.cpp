// POJ 1769 - Minimizing maximizer
// Model: claude-sonnet-5
// Approach: By the 0-1 principle, test the pipeline with inputs that are all 0 except a
// single 1. Sorting a contiguous range [i,j] non-decreasing moves that lone 1 (if present
// in the range) to slot j and leaves everything else untouched. So a chosen subsequence of
// sorters, applied in original order, moves the token that started at slot p to some final
// slot c(p); the pipeline is correct for every input iff c(p) = n for every starting slot
// p = 1..n. c(.) is always order-preserving (non-decreasing in p), so equal-valued slots
// form contiguous groups that only ever merge (never split) as sorters are applied, and the
// requirement forces every slot to end up merged into one group with value n.
//
// Let dp[r] = minimum number of sorters, chosen respecting the original left-to-right
// order and using only sorters seen so far while scanning, needed to merge the prefix
// [1,r] into one group whose value is exactly r. dp[1] = 0 (slot 1 starts as its own group
// with value 1, no sorter needed). Scanning the given sorters (l,r) = (i_k,j_k) in their
// original order, applying this sorter merges every currently-existing group whose value
// lies in [l,r] into one new group of value r; in particular it extends a prefix state
// dp[k] (k in [l,r-1]) to dp[r] = dp[k] + 1, and no sorter with l > k could ever reach a
// prefix state stopped at k, since a group with value k < l is outside the sorter's range
// and is left untouched. That gives the "interval jump" recurrence
//   dp[r] = min( dp[r], 1 + min_{l<=k<=r-1} dp[k] )
// processed online, one sorter at a time in input order, with a segment tree over
// positions 1..n supporting range-min query and point-min update (each sorter needs one
// query over [l,r-1] and, if finite, one update at r). Answer is dp[n], which is always
// finite since the full original pipeline is stated to already be a correct Maximizer.
//
// Verified by hand against the sample: sorters (20,30)(1,10)(10,20)(20,30)(15,25)(30,40)
// give dp[10]=1, dp[20]=2, dp[30]=3 (via the second (20,30)), dp[25]=3, dp[40]=4 -> matches
// the sample answer of 4.
//
// No statement ambiguity beyond the standard 0-1-principle argument above, which the
// sample is consistent with. Hint says huge input, scanf recommended; using scanf.
#include <cstdio>
#include <algorithm>
using namespace std;

static const int INF = 1 << 30;
int n, m;
int seg[200005];

void build(int node, int l, int r) {
    if (l == r) {
        seg[node] = (l == 1) ? 0 : INF;
        return;
    }
    int mid = (l + r) / 2;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    seg[node] = min(seg[node * 2], seg[node * 2 + 1]);
}

int query(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql || ql > qr) return INF;
    if (ql <= l && r <= qr) return seg[node];
    int mid = (l + r) / 2;
    int a = query(node * 2, l, mid, ql, qr);
    int b = query(node * 2 + 1, mid + 1, r, ql, qr);
    return a < b ? a : b;
}

void update(int node, int l, int r, int pos, int val) {
    if (l == r) {
        if (val < seg[node]) seg[node] = val;
        return;
    }
    int mid = (l + r) / 2;
    if (pos <= mid) update(node * 2, l, mid, pos, val);
    else update(node * 2 + 1, mid + 1, r, pos, val);
    seg[node] = min(seg[node * 2], seg[node * 2 + 1]);
}

int main() {
    scanf("%d %d", &n, &m);
    build(1, 1, n);
    for (int k = 0; k < m; k++) {
        int l, r;
        scanf("%d %d", &l, &r);
        int q = query(1, 1, n, l, r - 1);
        if (q < INF) {
            update(1, 1, n, r, q + 1);
        }
    }
    int ans = query(1, 1, n, n, n);
    printf("%d\n", ans);
    return 0;
}
