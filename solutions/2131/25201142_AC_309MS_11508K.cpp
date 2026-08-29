// POJ 2131 - Key Insertion
// Model: claude-sonnet-5
// Approach: The recursive Insert(L,K) always shifts a *contiguous* run of
// already-occupied cells [L, P-1] one step to the right, where P is the
// smallest empty position >= L, then writes K at L. P is found with a
// path-compressed DSU over positions (classic "next free slot" trick),
// amortized ~O(alpha). The whole array (positions 1..MAXPOS, all present,
// value 0 = empty) is kept in one implicit (size-indexed) treap supporting
// split/merge by rank. Each Insert becomes: split off the block [L,P-1],
// drop the placeholder node that sat at P, prepend a new node K to the
// block, and merge everything back -- reproducing the exact shift in
// O(log(N+M)) amortized per query, with total array size bounded by N+M
// (each op consumes exactly one previously-empty slot, so at most N+M-1
// positions are ever touched). This avoids the naive recursive Insert,
// whose recursion depth can reach N (~131072) and overflow the stack --
// the discuss board specifically complains about this (message 67537).
// No statement ambiguity: verified against the sample and a message-board
// thread whose first WA example was mis-transcribed and later corrected
// (message 121001) -- confirms M only bounds valid L, it does not pad the
// output length; W is simply the greatest ever-filled position. Cross
// checked against an O(N*M) brute-force direct simulation on randomized
// small cases.
#include <cstdio>
#include <cstdlib>

typedef unsigned int u32;

const int MAXN = 400010;

int par[MAXN];
int find(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

struct Node {
    int l, r, sz, val;
    u32 pri;
};
Node tp[MAXN];
int cnt = 0;

u32 rngState = 88172645u;
u32 xorshift() {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 17;
    rngState ^= rngState << 5;
    return rngState;
}

inline void pull(int x) {
    tp[x].sz = tp[tp[x].l].sz + tp[tp[x].r].sz + 1;
}

int newNode(int val) {
    ++cnt;
    tp[cnt].l = 0;
    tp[cnt].r = 0;
    tp[cnt].sz = 1;
    tp[cnt].val = val;
    tp[cnt].pri = xorshift();
    return cnt;
}

int merge(int a, int b) {
    if (!a || !b) return a ? a : b;
    if (tp[a].pri < tp[b].pri) {
        tp[a].r = merge(tp[a].r, b);
        pull(a);
        return a;
    } else {
        tp[b].l = merge(a, tp[b].l);
        pull(b);
        return b;
    }
}

// split treap x into a (first k nodes by rank) and b (the rest)
void split(int x, int k, int &a, int &b) {
    if (!x) {
        a = 0;
        b = 0;
        return;
    }
    if (tp[tp[x].l].sz < k) {
        a = x;
        split(tp[x].r, k - tp[tp[x].l].sz - 1, tp[x].r, b);
        pull(a);
    } else {
        b = x;
        split(tp[x].l, k, a, tp[x].l);
        pull(b);
    }
}

int seq[MAXN];
int outIdx;
void inorder(int x) {
    if (!x) return;
    inorder(tp[x].l);
    seq[++outIdx] = tp[x].val;
    inorder(tp[x].r);
}

int Lval[131077];

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 1; i <= n; ++i) scanf("%d", &Lval[i]);

    int MAXPOS = n + m + 5;

    for (int i = 1; i <= MAXPOS + 2; ++i) par[i] = i;

    int root = 0;
    for (int i = 1; i <= MAXPOS; ++i) {
        int nd = newNode(0);
        root = merge(root, nd);
    }

    int W = 0;
    for (int i = 1; i <= n; ++i) {
        int L = Lval[i];
        int P = find(L);

        int A, B, C, D, E;
        split(root, L - 1, A, B);
        split(B, P - L, B, C);
        split(C, 1, D, E); // D = single placeholder node at position P, discard

        int kNode = newNode(i);
        int newB = merge(kNode, B);
        root = merge(merge(A, newB), E);

        par[P] = P + 1;
        if (P > W) W = P;
    }

    outIdx = 0;
    inorder(root);

    printf("%d\n", W);
    for (int i = 1; i <= W; ++i) {
        printf("%d%c", seq[i], (i == W) ? '\n' : ' ');
    }
    return 0;
}
