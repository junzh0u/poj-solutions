// POJ 3245 - Sequence Partitioning
// Model: claude-sonnet-5
//
// Constraint 1 (Bp > Aq whenever p's part index < q's part index) is
// equivalent to: a cut after position r is legal iff
//   min(B[1..r]) > max(A[r+1..n])
// This is because that cut separates every earlier element from every
// later element, so the worst pair (the prefix-min B, the suffix-max A)
// must itself satisfy the inequality; conversely if it holds, every pair
// separated by that cut satisfies it too (min/max dominate). Any final
// partition's boundaries must each individually be a "legal" cut (the
// legality of one cut does not depend on which other cuts are chosen,
// since it is defined from the whole array's prefix-min B / suffix-max A).
//
// So first compute the set of legal cut positions and merge the sequence
// into blocks between them (a block cannot be split further; but any
// subset of the legal cuts may be used, i.e. adjacent blocks may be
// merged freely). Each block's aggregate is (max A in it, sum B in it).
//
// After that reduction, constraint 1 is automatically satisfied by any
// grouping of blocks, and the problem becomes: partition N blocks into
// contiguous groups, minimizing max(group B-sum) subject to
// sum(group max-A) <= Limit.
//
// Binary search the answer X (the minimized max B-sum). For fixed X,
// feasibility (and the minimal achievable sum of maxes) is a DP:
//   dp[0] = 0
//   dp[i] = min over l in [lo(i), i] of dp[l-1] + max(a[l..i])
// where lo(i) is the leftmost l with sum(b[l..i]) <= X (two-pointer,
// since b > 0). dp[i] is provably non-decreasing in i (extending the
// prefix can only add work), so within a group of l's sharing the same
// max(a[l..i]) value the best l is the smallest one. That lets a
// monotonic stack (grouping l's by their current running max, exactly
// like "next greater element") maintain, via a segment tree with range
// add + range min + point assign, the function l -> dp[l-1]+max(a[l..i])
// so dp[i] is one range-min query over [lo(i), i]. O(N log N) per check,
// O(N log N log(sum B)) overall.
//
// The board's own unanswered thread ("第一个条件怎么保证？") shows this
// reduction is the non-obvious part of the problem; verified locally by
// hand on the sample and by a brute force that checks constraint 1 the
// direct (pairwise) way on small random cases, independent of this
// derivation.

#include <cstdio>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

typedef long long ll;
static const ll INF = (ll)4e18;

int segN;
vector<ll> segMin, segLazy;

void build(int node, int l, int r) {
    segLazy[node] = 0;
    if (l == r) { segMin[node] = INF; return; }
    int mid = (l + r) / 2;
    build(node*2, l, mid);
    build(node*2+1, mid+1, r);
    segMin[node] = min(segMin[node*2], segMin[node*2+1]);
}

inline void push_down(int node) {
    if (segLazy[node] != 0) {
        ll v = segLazy[node];
        segMin[node*2] += v; segLazy[node*2] += v;
        segMin[node*2+1] += v; segLazy[node*2+1] += v;
        segLazy[node] = 0;
    }
}

void range_add(int node, int l, int r, int ql, int qr, ll val) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) { segMin[node] += val; segLazy[node] += val; return; }
    push_down(node);
    int mid = (l + r) / 2;
    range_add(node*2, l, mid, ql, qr, val);
    range_add(node*2+1, mid+1, r, ql, qr, val);
    segMin[node] = min(segMin[node*2], segMin[node*2+1]);
}

void point_set(int node, int l, int r, int pos, ll val) {
    if (l == r) { segMin[node] = val; return; }
    push_down(node);
    int mid = (l + r) / 2;
    if (pos <= mid) point_set(node*2, l, mid, pos, val);
    else point_set(node*2+1, mid+1, r, pos, val);
    segMin[node] = min(segMin[node*2], segMin[node*2+1]);
}

ll range_min(int node, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return INF;
    if (ql <= l && r <= qr) return segMin[node];
    push_down(node);
    int mid = (l + r) / 2;
    ll lv = range_min(node*2, l, mid, ql, qr);
    ll rv = range_min(node*2+1, mid+1, r, ql, qr);
    return min(lv, rv);
}

int N;
vector<ll> a, b, sb; // 1-indexed, size N+1

bool check(ll X, ll Limit) {
    build(1, 1, N);
    vector<ll> F(N+1);
    F[0] = 0;
    vector<int> stk;
    stk.reserve(N);
    int lo = 1;
    for (int i = 1; i <= N; i++) {
        while (sb[i] - sb[lo-1] > X) lo++;
        point_set(1, 1, N, i, F[i-1]);
        ll curVal = a[i];
        int curStart = i;
        while (!stk.empty() && a[stk.back()] <= curVal) {
            int p = stk.back(); stk.pop_back();
            int groupStart = stk.empty() ? 1 : stk.back() + 1;
            range_add(1, 1, N, groupStart, p, -a[p]);
            curStart = groupStart;
        }
        range_add(1, 1, N, curStart, i, curVal);
        stk.push_back(i);
        F[i] = range_min(1, 1, N, lo, i);
    }
    return F[N] <= Limit;
}

int main() {
    int n;
    ll Limit;
    scanf("%d %lld", &n, &Limit);
    vector<ll> A(n+1), B(n+1);
    for (int i = 1; i <= n; i++) {
        scanf("%lld %lld", &A[i], &B[i]);
    }

    vector<ll> sufMaxA(n+2, 0);
    for (int i = n; i >= 1; i--) sufMaxA[i] = max(sufMaxA[i+1], A[i]);

    // Build blocks by scanning left to right, tracking running prefix-min B
    // from the current block start, and cutting whenever the cut is legal.
    a.push_back(0); // index 0 unused
    b.push_back(0);

    ll curMaxA = 0, curSumB = 0, curMinB = INF;
    for (int i = 1; i <= n; i++) {
        curMaxA = max(curMaxA, A[i]);
        curSumB += B[i];
        curMinB = min(curMinB, B[i]);
        bool cutHere = (i == n) || (curMinB > sufMaxA[i+1]);
        if (cutHere) {
            a.push_back(curMaxA);
            b.push_back(curSumB);
            curMaxA = 0; curSumB = 0; curMinB = INF;
        }
    }

    N = (int)a.size() - 1;
    sb.assign(N+1, 0);
    ll totalB = 0, maxBlockB = 0;
    for (int i = 1; i <= N; i++) {
        sb[i] = sb[i-1] + b[i];
        totalB += b[i];
        maxBlockB = max(maxBlockB, b[i]);
    }

    segN = N;
    segMin.assign(4*(N+1)+10, INF);
    segLazy.assign(4*(N+1)+10, 0);

    ll lo = maxBlockB, hi = totalB;
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (check(mid, Limit)) hi = mid;
        else lo = mid + 1;
    }

    printf("%lld\n", lo);
    return 0;
}
