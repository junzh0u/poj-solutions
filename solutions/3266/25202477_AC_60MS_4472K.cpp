// POJ 3266 - Cow School
// Model: claude-sonnet-5
//
// Sort tests by percentage T_i/P_i descending. For keep-count K = N-D, the
// teacher keeps the prefix A = {1..K} (highest percentages) and the resulting
// grade is r_K = ST(K)/SP(K) (prefix sums of T and P). Bessy can beat the
// teacher for this D iff some size-K subset has a strictly higher grade than
// r_K. Since the unique maximizer of sum(T_i - r_K*P_i) over all size-K
// subsets is exactly the K items of largest value at x=r_K, and A itself
// achieves sum exactly 0 at x=r_K, A fails to be optimal iff some element
// of the suffix B={K+1..N} has value(r_K) strictly greater than the minimum
// value(r_K) over A -- i.e. iff max_{B} v(r_K) > min_{A} v(r_K). (If every
// element of A dominates every element of B at r_K, A trivially is the top-K
// set and no improvement exists.)
//
// v_i(x) = T_i - P_i*x, evaluated at x = r_K = ST(K)/SP(K). Comparing values
// at a fixed K only requires comparing T_i*SP(K) - P_i*ST(K) (scaling by the
// positive SP(K) preserves order), which stays exact in 64-bit integers
// (bounded by ~8e13, since T,P < 40000 and prefix sums < 2e9 for N<=50000).
//
// For each K we need min over the prefix {1..K} and max over the suffix
// {K+1..N} of this (line-valued) expression at position K. Both are answered
// with a Li Chao tree over the N possible positions (positions serve directly
// as the sorted domain because r_K is strictly decreasing in K -- adding a
// lower-percentage item always drags the weighted average down): insert
// lines in index order (1..N for the prefix, N..1 for the suffix) and query
// immediately after/ before insertion as appropriate; insertion order need
// not match a slope order since the tree only ever compares two lines at the
// SAME query position, so this generalized "scaled" evaluation is safe.
//
// Ambiguity: the discuss board only debates translation ("over" = fraction
// bar) and confirms this is considered a hard problem; nothing about input
// framing. D=0 and D=N are automatically never reported since the prefix (or
// suffix) set is empty, giving no possible improvement -- verified against
// the sample (N=5) which yields exactly D=1,2, matching official output.
#include <cstdio>
#include <algorithm>
#include <climits>
using namespace std;
typedef long long ll;

static int N;
static ll ST[50005], SP[50005];
static int treeSize;
static const int TREE_CAP = 4 * 50005 + 10;
static ll treeT[TREE_CAP], treeP[TREE_CAP];
static bool occ[TREE_CAP];

static inline ll evalLine(ll T, ll P, int pos) {
    return T * SP[pos] - P * ST[pos];
}

static void resetTree() {
    for (int i = 0; i < treeSize; i++) occ[i] = false;
}

static void insertLine(int node, int l, int r, ll T, ll P) {
    if (!occ[node]) {
        occ[node] = true;
        treeT[node] = T;
        treeP[node] = P;
        return;
    }
    if (l == r) {
        if (evalLine(T, P, l) > evalLine(treeT[node], treeP[node], l)) {
            treeT[node] = T;
            treeP[node] = P;
        }
        return;
    }
    int m = (l + r) / 2;
    bool lNew = evalLine(T, P, l) > evalLine(treeT[node], treeP[node], l);
    bool mNew = evalLine(T, P, m) > evalLine(treeT[node], treeP[node], m);
    if (mNew) {
        ll tt = treeT[node], pp = treeP[node];
        treeT[node] = T;
        treeP[node] = P;
        T = tt;
        P = pp;
    }
    if (lNew != mNew) insertLine(node * 2, l, m, T, P);
    else insertLine(node * 2 + 1, m + 1, r, T, P);
}

static ll queryPos(int node, int l, int r, int pos) {
    if (!occ[node]) return LLONG_MIN;
    ll best = evalLine(treeT[node], treeP[node], pos);
    if (l == r) return best;
    int m = (l + r) / 2;
    if (pos <= m) {
        ll v = queryPos(node * 2, l, m, pos);
        if (v > best) best = v;
    } else {
        ll v = queryPos(node * 2 + 1, m + 1, r, pos);
        if (v > best) best = v;
    }
    return best;
}

struct Item {
    ll T, P;
};

static Item items[50005]; // 1-indexed after sort

static bool cmpDesc(const Item &a, const Item &b) {
    return a.T * b.P > b.T * a.P;
}

int main() {
    if (scanf("%d", &N) != 1) return 0;
    for (int i = 0; i < N; i++) {
        ll t, p;
        scanf("%lld %lld", &t, &p);
        items[i].T = t;
        items[i].P = p;
    }
    sort(items, items + N, cmpDesc);
    // shift to 1-indexed
    for (int i = N; i >= 1; i--) items[i] = items[i - 1];

    ST[0] = 0;
    SP[0] = 0;
    for (int k = 1; k <= N; k++) {
        ST[k] = ST[k - 1] + items[k].T;
        SP[k] = SP[k - 1] + items[k].P;
    }

    static ll maxSuffix[50005];
    static ll minPrefix[50005];

    treeSize = 4 * (N + 2);

    // suffix max: process K = N down to 1
    resetTree();
    for (int K = N; K >= 1; K--) {
        if (K < N) {
            maxSuffix[K] = queryPos(1, 1, N, K);
        }
        insertLine(1, 1, N, items[K].T, items[K].P);
    }

    // prefix min via negated-line max tree: process K = 1..N
    resetTree();
    for (int K = 1; K <= N; K++) {
        insertLine(1, 1, N, -items[K].T, -items[K].P);
        ll negMax = queryPos(1, 1, N, K);
        minPrefix[K] = -negMax;
    }

    static int resD[50005];
    int cnt = 0;
    for (int K = 1; K <= N - 1; K++) {
        if (maxSuffix[K] > minPrefix[K]) {
            int D = N - K;
            resD[cnt++] = D;
        }
    }
    sort(resD, resD + cnt);
    printf("%d\n", cnt);
    for (int i = 0; i < cnt; i++) printf("%d\n", resD[i]);
    return 0;
}
