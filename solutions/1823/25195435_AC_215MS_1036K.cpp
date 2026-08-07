// POJ 1823 - Hotel
// Model: claude-sonnet-5
// Segment tree over room indices [1,N], each node keeps the max contiguous
// free run within its range plus the max free run touching its left/right
// border (lmax/rmax), so two children merge in O(1): mx = max(child mx's,
// left.rmax + right.lmax) when the boundary run bridges the split.  Type-1
// (arrival) / type-2 (departure) give the exact room range to occupy/free,
// so they are plain range-assign updates with lazy propagation (lazy=0
// meaning "whole segment occupied", lazy=1 "whole segment free", -1 "mixed,
// no pending assign"); a type-3 query is just the root's mx, no search
// needed.  No statement ambiguity: input already gives the range explicitly
// (unlike POJ 3667's "find leftmost run of length M"), and the sample was
// re-derived by hand instruction by instruction and matches.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 16005;
int N, P;
int mx[MAXN * 4], lmx[MAXN * 4], rmx[MAXN * 4], lazy_[MAXN * 4];

void pushup(int node, int l, int r) {
    int lc = node * 2, rc = node * 2 + 1;
    int mid = (l + r) / 2;
    int lenL = mid - l + 1, lenR = r - mid;
    lmx[node] = lmx[lc];
    if (lmx[lc] == lenL) lmx[node] += lmx[rc];
    rmx[node] = rmx[rc];
    if (rmx[rc] == lenR) rmx[node] += rmx[lc];
    mx[node] = max(max(mx[lc], mx[rc]), rmx[lc] + lmx[rc]);
}

void setNode(int node, int l, int r, int val) {
    int len = r - l + 1;
    int f = val ? len : 0;
    mx[node] = lmx[node] = rmx[node] = f;
    lazy_[node] = val;
}

void pushdown(int node, int l, int r) {
    if (lazy_[node] != -1) {
        int mid = (l + r) / 2;
        setNode(node * 2, l, mid, lazy_[node]);
        setNode(node * 2 + 1, mid + 1, r, lazy_[node]);
        lazy_[node] = -1;
    }
}

void build(int node, int l, int r) {
    lazy_[node] = -1;
    if (l == r) {
        mx[node] = lmx[node] = rmx[node] = 1;
        return;
    }
    int mid = (l + r) / 2;
    build(node * 2, l, mid);
    build(node * 2 + 1, mid + 1, r);
    pushup(node, l, r);
}

void update(int node, int l, int r, int ql, int qr, int val) {
    if (ql <= l && r <= qr) {
        setNode(node, l, r, val);
        return;
    }
    pushdown(node, l, r);
    int mid = (l + r) / 2;
    if (ql <= mid) update(node * 2, l, mid, ql, qr, val);
    if (qr > mid) update(node * 2 + 1, mid + 1, r, ql, qr, val);
    pushup(node, l, r);
}

int main() {
    scanf("%d %d", &N, &P);
    build(1, 1, N);
    for (int k = 0; k < P; ++k) {
        int c;
        scanf("%d", &c);
        if (c == 1) {
            int i, M;
            scanf("%d %d", &i, &M);
            update(1, 1, N, i, i + M - 1, 0);
        } else if (c == 2) {
            int i, M;
            scanf("%d %d", &i, &M);
            update(1, 1, N, i, i + M - 1, 1);
        } else {
            printf("%d\n", mx[1]);
        }
    }
    return 0;
}
