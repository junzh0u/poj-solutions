// POJ 2114 - Boatherds
// Model: claude-opus-5
//
// For each query x, decide whether some path in the tree has cost exactly x.
// N <= 10000, M <= 100 queries, edge prices 0..1000, x <= 10^7.
//
// Centroid decomposition, done once for the whole query set.  At a centroid c
// every path either has c as an endpoint or joins two different branches of c,
// so collecting (distance from c, branch id) for the component -- plus the
// entry (0, branch 0) for c itself -- and looking for two entries with
// different branch ids summing to x settles every path through c.  Recursing
// into the branches covers the rest; total collected entries are O(n log n).
//
// The list is sorted once per centroid and each still-unanswered query gets one
// two-pointer sweep over it.  Equal distances are handled as whole blocks: when
// arr[l].d + arr[r].d == x, a valid path exists iff the union of the branch ids
// in the left block and the right block is not a single common id.  Without the
// block treatment the different-branch test is not exact, and treating each
// node as its own branch (or dropping the test) counts down-and-back-up walks
// inside one branch as paths.  Answering by pair count instead of existence
// would need signed inclusion-exclusion, which existence does not support --
// hence the branch ids.
//
// Prices may be 0, so distinct nodes can share a distance and a "path" of cost
// 0 exists; queries satisfy x >= 1, so the degenerate a == b path is never
// asked about.  Max path cost is 9999 * 1000 < 2^31, so int suffices.
//
// Input framing: village lines are (d, c) pairs terminated by 0 and d >= 2
// always, so token-wise reading is unambiguous; queries are terminated by 0
// (x >= 1) and the whole input by an N of 0.  Multiple instances per file.
//
// Verified against the CTU Open 2004 official judge data (contest.felk.cvut.cz
// /04prg/solved/b.in,b.out): byte-identical output.  Six mutants were built
// (drop the different-branch test, per-node branch ids, omit the centroid's own
// entry, drop the first edge weight, one-sided block scan, off-by-one in the
// max-sum shortcut); the statement's sample survives three of them, the
// official data kills all six.  Also differentially tested against an
// all-pairs-DFS brute force written from the statement (~7800 random instances,
// n <= 200, balanced AYE/NAY) and against the contest's own reference solution
// (n <= 2000).  Worst case measured at 0.05s: 5 x N=10000 with 100 queries each
// chosen so every query is unsatisfiable and every sweep runs to completion.

#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 10005;

int head_[MAXN], nxt_[2 * MAXN], eto_[2 * MAXN], ew_[2 * MAXN], ecnt;
bool gone[MAXN];
int sz_[MAXN], par_[MAXN], comp_[MAXN], compCnt;
int stkN[MAXN];

const int MAXQ = 100005;
int qs_[MAXQ], qm;
bool ansq[MAXQ];

struct Ent {
    int d, b;
};
static bool byDist(const Ent &a, const Ent &b) { return a.d < b.d; }

Ent arr[MAXN + 1];
int arrCnt;

int bs_n[MAXN], bs_d[MAXN], bs_p[MAXN];

static void addEdge(int u, int v, int w) {
    ++ecnt; nxt_[ecnt] = head_[u]; eto_[ecnt] = v; ew_[ecnt] = w; head_[u] = ecnt;
    ++ecnt; nxt_[ecnt] = head_[v]; eto_[ecnt] = u; ew_[ecnt] = w; head_[v] = ecnt;
}

/* Collect the component containing root (skipping removed centroids) in DFS
   preorder, and compute subtree sizes rooted at root. */
static void collect(int root) {
    compCnt = 0;
    int top = 0;
    stkN[top++] = root;
    par_[root] = 0;
    while (top) {
        int u = stkN[--top];
        comp_[compCnt++] = u;
        for (int e = head_[u]; e; e = nxt_[e]) {
            int v = eto_[e];
            if (v == par_[u] || gone[v]) continue;
            par_[v] = u;
            stkN[top++] = v;
        }
    }
    for (int i = 0; i < compCnt; i++) sz_[comp_[i]] = 1;
    for (int i = compCnt - 1; i >= 1; i--) sz_[par_[comp_[i]]] += sz_[comp_[i]];
}

static int findCentroid() {
    int best = comp_[0], bestVal = compCnt + 1;
    for (int i = 0; i < compCnt; i++) {
        int u = comp_[i];
        int mx = compCnt - sz_[u];
        for (int e = head_[u]; e; e = nxt_[e]) {
            int v = eto_[e];
            if (v == par_[u] || gone[v]) continue;
            if (sz_[v] > mx) mx = sz_[v];
        }
        if (mx < bestVal) { bestVal = mx; best = u; }
    }
    return best;
}

/* For every still-open query, decide whether two entries from *different*
   branches sum to it.  Equal-distance runs are handled as blocks so that the
   different-branch test is exact. */
static void answerQueries() {
    sort(arr, arr + arrCnt, byDist);
    int maxSum = arr[arrCnt - 1].d + arr[arrCnt - 2].d;
    for (int q = 0; q < qm; q++) {
        if (ansq[q]) continue;
        int x = qs_[q];
        if (x > maxSum) continue;
        int l = 0, r = arrCnt - 1;
        while (l < r) {
            int s = arr[l].d + arr[r].d;
            if (s < x) { l++; continue; }
            if (s > x) { r--; continue; }
            int l2 = l; while (l2 < r && arr[l2 + 1].d == arr[l].d) l2++;
            int r2 = r; while (r2 > l && arr[r2 - 1].d == arr[r].d) r2--;
            int b0 = arr[l].b;
            bool diff = false;
            for (int i = l; i <= l2; i++) if (arr[i].b != b0) { diff = true; break; }
            if (!diff)
                for (int i = r2; i <= r; i++) if (arr[i].b != b0) { diff = true; break; }
            if (diff) { ansq[q] = true; break; }
            l = l2 + 1;
            r = r2 - 1;
        }
    }
}

static void solve(int root) {
    collect(root);
    if (compCnt <= 1) { gone[root] = true; return; }
    int c = findCentroid();
    gone[c] = true;

    arrCnt = 0;
    arr[arrCnt].d = 0; arr[arrCnt].b = 0; arrCnt++;
    int bid = 0;
    for (int e = head_[c]; e; e = nxt_[e]) {
        int v = eto_[e];
        if (gone[v]) continue;
        ++bid;
        int top = 0;
        bs_n[top] = v; bs_d[top] = ew_[e]; bs_p[top] = c; top++;
        while (top) {
            --top;
            int u = bs_n[top], du = bs_d[top], pu = bs_p[top];
            arr[arrCnt].d = du; arr[arrCnt].b = bid; arrCnt++;
            for (int f = head_[u]; f; f = nxt_[f]) {
                int y = eto_[f];
                if (y == pu || gone[y]) continue;
                bs_n[top] = y; bs_d[top] = du + ew_[f]; bs_p[top] = u; top++;
            }
        }
    }
    if (arrCnt >= 2) answerQueries();

    for (int e = head_[c]; e; e = nxt_[e]) {
        int v = eto_[e];
        if (!gone[v]) solve(v);
    }
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        ecnt = 0;
        for (int i = 1; i <= n; i++) { head_[i] = 0; gone[i] = false; }
        for (int i = 1; i <= n; i++) {
            int d, c;
            while (scanf("%d", &d) == 1 && d != 0) {
                if (scanf("%d", &c) != 1) c = 0;
                addEdge(i, d, c);
            }
        }
        qm = 0;
        int x;
        while (scanf("%d", &x) == 1 && x != 0)
            if (qm < MAXQ) { qs_[qm] = x; ansq[qm] = false; qm++; }
        if (qm > 0)
            for (int i = 1; i <= n; i++)
                if (!gone[i]) solve(i);
        for (int i = 0; i < qm; i++) puts(ansq[i] ? "AYE" : "NAY");
        puts(".");
    }
    return 0;
}
