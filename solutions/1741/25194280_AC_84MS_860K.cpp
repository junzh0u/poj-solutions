// POJ 1741 - Tree
//
// Count unordered pairs (u,v) of tree vertices with dist(u,v) <= k.
// n <= 10000 per case, edge weights in [1,1000], several cases until "0 0".
//
// Centroid decomposition. For a component, find its centroid c (the node whose
// largest remaining piece is smallest), then every path that has not already
// been counted either ends at c or crosses it, so it can be measured as
// d(c,u) + d(c,v). Collect the distance from c to every node of the component
// (plus a 0 for c itself), sort, and a two-pointer sweep counts all pairs whose
// sum is <= k in one pass. That over-counts pairs living in the same child
// subtree, whose real distance is shorter than the sum through c, so for each
// child subtree the same sweep is run over just that subtree's distances and
// subtracted -- the two counts use identical values, so the cancellation is
// exact. Then c is deleted and each remaining piece is solved recursively;
// depth is O(log n), giving O(n log^2 n) overall.
//
// Notes on the statement and on POJ:
//  - "pair" is unordered and u != v; the sample's 8 is C(5,2) minus the two
//    pairs at distance 5.
//  - n == 1 is legal (zero edge lines follow) and the answer is 0; only "0 0"
//    terminates the input.
//  - The answer is at most C(10000,2) = 49995000, so it is printed with %d --
//    POJ's Windows toolchain makes %lld unreliable and it is not needed here.
//  - Both traversals (component collection and distance gathering) are
//    explicit-stack loops rather than recursion: a path-shaped tree would push
//    10000 frames, and the 30000K memory limit leaves little room to gamble.
//  - The scratch arrays (nodes/dbuf/sbuf/stk_) are global and shared across
//    recursion levels, which is safe because every frame finishes using them
//    before it recurses.

#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 10005;

int head[MAXN], nxt[2 * MAXN], eto[2 * MAXN], ewt[2 * MAXN], ecnt;
bool done_[MAXN];
int sz[MAXN], mx[MAXN];
int stk_[MAXN], par[MAXN], dis[MAXN];
int nodes[MAXN], dbuf[MAXN], sbuf[MAXN];
int n, K;
long long ans;

static void addEdge(int u, int v, int w) {
    eto[ecnt] = v; ewt[ecnt] = w; nxt[ecnt] = head[u]; head[u] = ecnt++;
}

/* collect the component containing root (skipping removed nodes); parents first */
static int getComp(int root, int *arr) {
    int top = 0, cnt = 0;
    stk_[top++] = root; par[root] = 0;
    while (top) {
        int u = stk_[--top];
        arr[cnt++] = u;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = eto[e];
            if (v != par[u] && !done_[v]) { par[v] = u; stk_[top++] = v; }
        }
    }
    return cnt;
}

/* distances from the centroid inside one of its subtrees */
static int gather(int start, int startd, int cent, int *out) {
    int top = 0, cnt = 0;
    stk_[top++] = start; par[start] = cent; dis[start] = startd;
    while (top) {
        int u = stk_[--top];
        out[cnt++] = dis[u];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = eto[e];
            if (v != par[u] && !done_[v]) {
                par[v] = u; dis[v] = dis[u] + ewt[e]; stk_[top++] = v;
            }
        }
    }
    return cnt;
}

/* number of unordered pairs i<j with a[i]+a[j] <= K */
static long long countPairs(int *a, int m) {
    sort(a, a + m);
    int i = 0, j = m - 1;
    long long res = 0;
    while (i < j) {
        if (a[i] + a[j] <= K) { res += j - i; ++i; }
        else --j;
    }
    return res;
}

static void solve(int root) {
    int cnt = getComp(root, nodes);
    if (cnt <= 1) return;

    int i;
    for (i = 0; i < cnt; ++i) { sz[nodes[i]] = 1; mx[nodes[i]] = 0; }
    for (i = cnt - 1; i >= 1; --i) sz[par[nodes[i]]] += sz[nodes[i]];
    for (i = cnt - 1; i >= 1; --i) {
        int u = nodes[i], p = par[u];
        if (sz[u] > mx[p]) mx[p] = sz[u];
    }
    int c = nodes[0], best = cnt + 1;
    for (i = 0; i < cnt; ++i) {
        int u = nodes[i];
        int m = mx[u];
        if (cnt - sz[u] > m) m = cnt - sz[u];
        if (m < best) { best = m; c = u; }
    }

    done_[c] = true;

    int m = 0;
    dbuf[m++] = 0;
    for (int e = head[c]; e != -1; e = nxt[e]) {
        int v = eto[e];
        if (done_[v]) continue;
        int cs = gather(v, ewt[e], c, sbuf);
        ans -= countPairs(sbuf, cs);
        for (i = 0; i < cs; ++i) dbuf[m++] = sbuf[i];
    }
    ans += countPairs(dbuf, m);

    for (int e = head[c]; e != -1; e = nxt[e]) {
        int v = eto[e];
        if (!done_[v]) solve(v);
    }
}

/* ---- fast input ---- */
static char ibuf[1 << 16];
static int ipos, ilen;
static inline int gc() {
    if (ipos == ilen) { ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin); ipos = 0; if (ilen <= 0) return -1; }
    return ibuf[ipos++];
}
static bool readInt(int &out) {
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
        if (!readInt(K)) break;
        if (n == 0 && K == 0) break;
        ecnt = 0;
        int i;
        for (i = 1; i <= n; ++i) { head[i] = -1; done_[i] = false; }
        for (i = 0; i < n - 1; ++i) {
            int u, v, w;
            readInt(u); readInt(v); readInt(w);
            addEdge(u, v, w);
            addEdge(v, u, w);
        }
        ans = 0;
        if (n > 1) solve(1);
        /* the answer is at most C(10000,2) = 49995000, so it fits in an int:
           print it as one and dodge the %lld / %I64d portability trap on POJ */
        printf("%d\n", (int)ans);
    }
    return 0;
}
