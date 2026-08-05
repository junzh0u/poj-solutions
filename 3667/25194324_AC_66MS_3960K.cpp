// POJ 3667 - Hotel
//
// Segment tree over room indices 1..N, one node per range storing: len (range
// size), lsum/rsum (length of the maximal free run touching the range's left
// and right ends respectively), msum (longest free run anywhere inside the
// range), and a lazy "cover" tag (-1 = none pending, 0 = assign all free,
// 1 = assign all occupied). Assigning a whole range to one state is O(1) at
// that node (applyCover) and composes correctly under lazy propagation
// because a fresh assign completely overwrites whatever tag was pending
// below it - pushDown always sends the current tag down before recursing
// into a partially-covered range, so a checkout can safely land on top of a
// still-pending checkin without the two tags mixing.
//
// Check-in for D rooms is a single top-down descent (query): a node can
// satisfy the request from its left child if left.msum >= D; otherwise the
// run may straddle the boundary (left.rsum + right.lsum >= D), giving the
// start position directly; otherwise recurse right. This finds the leftmost
// valid start in O(log N) without a separate binary search. Checkout is a
// plain range-assign to "free". Overall O((N+M) log N), comfortably inside
// the 3s limit for N, M <= 50000.
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 50005;

int N, M;

struct Node {
    int l, r;
    int len;
    int cover; // -1 = none, 0 = free, 1 = occupied
    int lsum, rsum, msum; // longest free run touching left, touching right, anywhere
} tree[MAXN * 4];

void pushUp(int p) {
    Node &n = tree[p], &lc = tree[p*2], &rc = tree[p*2+1];
    n.lsum = lc.lsum;
    if (lc.lsum == lc.len) n.lsum += rc.lsum;
    n.rsum = rc.rsum;
    if (rc.rsum == rc.len) n.rsum += lc.rsum;
    n.msum = max(lc.msum, rc.msum);
    n.msum = max(n.msum, lc.rsum + rc.lsum);
}

void applyCover(int p, int val) {
    Node &n = tree[p];
    n.cover = val;
    if (val == 1) {
        n.lsum = n.rsum = n.msum = 0;
    } else {
        n.lsum = n.rsum = n.msum = n.len;
    }
}

void pushDown(int p) {
    if (tree[p].cover != -1) {
        applyCover(p*2, tree[p].cover);
        applyCover(p*2+1, tree[p].cover);
        tree[p].cover = -1;
    }
}

void build(int p, int l, int r) {
    tree[p].l = l; tree[p].r = r; tree[p].len = r - l + 1;
    tree[p].cover = -1;
    tree[p].lsum = tree[p].rsum = tree[p].msum = tree[p].len;
    if (l == r) return;
    int mid = (l + r) / 2;
    build(p*2, l, mid);
    build(p*2+1, mid+1, r);
}

void update(int p, int l, int r, int val) {
    if (l <= tree[p].l && tree[p].r <= r) {
        applyCover(p, val);
        return;
    }
    pushDown(p);
    int mid = (tree[p].l + tree[p].r) / 2;
    if (l <= mid) update(p*2, l, r, val);
    if (r > mid) update(p*2+1, l, r, val);
    pushUp(p);
}

// find leftmost starting position of D consecutive free rooms; 0 if not found
int query(int p, int D) {
    if (tree[p].msum < D) return 0;
    if (tree[p].l == tree[p].r) return tree[p].l; // len==1 and msum>=D implies D==1
    pushDown(p);
    if (tree[p*2].msum >= D) return query(p*2, D);
    if (tree[p*2].rsum + tree[p*2+1].lsum >= D) {
        return tree[p*2].r - tree[p*2].rsum + 1;
    }
    return query(p*2+1, D);
}

int main() {
    scanf("%d %d", &N, &M);
    build(1, 1, N);
    for (int i = 0; i < M; i++) {
        int type;
        scanf("%d", &type);
        if (type == 1) {
            int D;
            scanf("%d", &D);
            int pos = query(1, D);
            if (pos == 0) {
                printf("0\n");
            } else {
                printf("%d\n", pos);
                update(1, pos, pos + D - 1, 1);
            }
        } else {
            int X, D;
            scanf("%d %d", &X, &D);
            update(1, X, X + D - 1, 0);
        }
    }
    return 0;
}
