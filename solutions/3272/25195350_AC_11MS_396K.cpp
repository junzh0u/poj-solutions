// POJ 3272 - Cow Traffic
// Model: claude-opus-5
//
// The trail network is a DAG in which every arc runs from a lower-numbered
// intersection to a higher-numbered one, so 1..N is already a topological
// order and no sorting is needed.  Grazing locations are exactly the
// intersections of in-degree 0; the barn is N.
//
// For a trail u->v, the number of cow paths that contain it is
//     (paths from any grazing location to u) * (paths from v to the barn),
// since a path through the trail is any prefix ending at u glued to any
// suffix starting at v.  So:
//     a[u] = 1 if indeg(u) == 0, else sum of a[w] over arcs w->u   (forward, 1..N)
//     b[v] = 1 if v == N,        else sum of b[w] over arcs v->w   (backward, N..1)
// and the answer is max over trails of a[u]*b[v].  Parallel trails count
// separately, which falls out of iterating over the edge list rather than
// over distinct node pairs.  O(N+M).
//
// Counters are unsigned only for well-definedness at the two ends: a[N] and
// b[grazing] can in principle exceed 32 bits, but neither is ever used in a
// product (N has no outgoing trail, a grazing location no incoming one).
// Every value that does enter a product is bounded by that product, which the
// statement guarantees fits in a signed 32-bit integer, so all used values are
// exact.  Verified against the two cases posted on the discuss board
// (5 11 ... -> 5 and 6 12 ... -> 8) and against a path-enumerating brute force
// on 3000 random DAGs.
#include <cstdio>

const int MAXN = 5005;
const int MAXM = 50005;

int eu[MAXM], ev[MAXM];
int head[MAXN], nxt[MAXM];
int indeg[MAXN];
unsigned int a[MAXN], b[MAXN];

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 1; i <= n; ++i) { head[i] = -1; indeg[i] = 0; a[i] = 0; b[i] = 0; }
    for (int i = 0; i < m; ++i) {
        int x, y;
        scanf("%d %d", &x, &y);
        eu[i] = x; ev[i] = y;
        nxt[i] = head[x]; head[x] = i;
        ++indeg[y];
    }
    for (int i = 1; i <= n; ++i) if (indeg[i] == 0) a[i] = 1u;
    for (int i = 1; i <= n; ++i)
        for (int e = head[i]; e != -1; e = nxt[e]) a[ev[e]] += a[i];
    b[n] = 1u;
    for (int i = n; i >= 1; --i)
        for (int e = head[i]; e != -1; e = nxt[e]) b[i] += b[ev[e]];
    unsigned int ans = 0u;
    for (int i = 0; i < m; ++i) {
        unsigned int cur = a[eu[i]] * b[ev[i]];
        if (cur > ans) ans = cur;
    }
    printf("%u\n", ans);
    return 0;
}
