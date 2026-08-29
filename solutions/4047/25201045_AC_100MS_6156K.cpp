// POJ 4047 - Garden
// Model: claude-sonnet-5
//
// Approach: Let D(s) = sum(a[s..s+k-1]) for a window of fixed length k
// starting at position s (1 <= s <= n-k+1). Query p=2 x y asks for the
// maximum D(s) over s in [x, y-k+1] (guaranteed non-empty since
// y-x+1 >= k). Setting a[i] = y or swapping two positions changes a[i]
// by some delta, which changes D(s) for every window that contains
// position i, i.e. for s in [max(1,i-k+1), min(n-k+1,i)] -- a contiguous
// range. Since adding a constant to a range commutes with taking a max
// (max(v+c) = max(v)+c), a segment tree supporting range-add + range-max
// with lazy propagation handles both point updates (as range adds on the
// D array) and range-max queries in O(log n) each.
//
// The problem statement is not shown on POJ's own problem page (known
// data loss for this id block); it was recovered from the contest PDF
// linked off the discuss board (Problem D. "Garden" in jinghua.pdf,
// GDUT/whatever contest set). The sample there was verified by hand
// simulation and matches this solution's model exactly.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 200005;
int n, m, k, sz; // sz = n-k+1, number of valid window starts
int a[MAXN];
long long treeMax[4 * MAXN];
long long lazy[4 * MAXN];
long long D[MAXN];

void build(int node, int l, int r) {
    lazy[node] = 0;
    if (l == r) {
        treeMax[node] = D[l];
        return;
    }
    int mid = (l + r) / 2;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    treeMax[node] = max(treeMax[node * 2], treeMax[node * 2 + 1]);
}

void push_down(int node) {
    if (lazy[node] != 0) {
        for (int c = node * 2; c <= node * 2 + 1; c++) {
            lazy[c] += lazy[node];
            treeMax[c] += lazy[node];
        }
        lazy[node] = 0;
    }
}

void update(int node, int l, int r, int ql, int qr, long long delta) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        treeMax[node] += delta;
        lazy[node] += delta;
        return;
    }
    push_down(node);
    int mid = (l + r) / 2;
    update(node * 2, l, mid, ql, qr, delta);
    update(node * 2 + 1, mid + 1, r, ql, qr, delta);
    treeMax[node] = max(treeMax[node * 2], treeMax[node * 2 + 1]);
}

long long query(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return -0x3f3f3f3f3f3f3f3fLL;
    if (ql <= l && r <= qr) return treeMax[node];
    push_down(node);
    int mid = (l + r) / 2;
    long long lv = query(node * 2, l, mid, ql, qr);
    long long rv = query(node * 2 + 1, mid + 1, r, ql, qr);
    return max(lv, rv);
}

// apply a value change at position i (1-indexed) with the given delta
void applyPointDelta(int i, long long delta) {
    if (delta == 0) return;
    int lo = i - k + 1;
    if (lo < 1) lo = 1;
    int hi = i;
    if (hi > sz) hi = sz;
    if (lo > hi) return; // shouldn't happen
    update(1, 1, sz, lo, hi, delta);
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d %d %d", &n, &m, &k);
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        sz = n - k + 1;
        // build initial D array via sliding window sum
        long long cur = 0;
        for (int i = 1; i <= k; i++) cur += a[i];
        D[1] = cur;
        for (int s = 2; s <= sz; s++) {
            cur += a[s + k - 1] - a[s - 1];
            D[s] = cur;
        }
        build(1, 1, sz);

        for (int q = 0; q < m; q++) {
            int p, x, y;
            scanf("%d %d %d", &p, &x, &y);
            if (p == 0) {
                long long delta = (long long)y - a[x];
                if (delta != 0) {
                    a[x] = y;
                    applyPointDelta(x, delta);
                }
            } else if (p == 1) {
                if (x != y) {
                    int tmp = a[x];
                    long long deltaX = (long long)a[y] - a[x];
                    long long deltaY = (long long)tmp - a[y];
                    a[x] = a[y];
                    a[y] = tmp;
                    applyPointDelta(x, deltaX);
                    applyPointDelta(y, deltaY);
                }
            } else {
                int s_lo = x;
                int s_hi = y - k + 1;
                long long ans = query(1, 1, sz, s_lo, s_hi);
                printf("%lld\n", ans);
            }
        }
    }
    return 0;
}
