// POJ 1751 - Highways
//
// N <= 750 towns at distinct integer coordinates, M <= 1000 highways already
// built.  Print the *new* highways of a minimum spanning structure, one
// "a b" pair per line; already-built roads must not appear, and if the towns
// are already connected the output is empty.
//
// Approach: minimum spanning tree of the complete graph on the towns, with
// every already-built road given weight 0.  A zero edge is always preferred,
// so the MST absorbs all the free roads first and the remaining tree edges are
// exactly the roads worth building.  Dense Prim in O(N^2) fits 750 towns
// comfortably (17MS here); a heap/Kruskal over all 280k pairs is unnecessary.
//
// Weights are kept as *squared* Euclidean distances in int.  Squaring is
// monotone on non-negative values, so it does not change the ordering of edge
// weights and hence not the MST -- this keeps the whole computation in exact
// integer arithmetic (max 2*20000^2 = 8e8, inside int, and below the 0x3f3f3f3f
// sentinel).
//
// Printing: when Prim pops a vertex, emit "par[v] v" unless the edge that
// pulled it in had weight 0.  Since the statement guarantees every town has a
// unique location, a genuine road has strictly positive length, so weight 0
// identifies an existing road exactly.  The root (and the whole of an
// already-connected instance) therefore prints nothing, which is the required
// empty output.
//
// Statement ambiguity: the answer is not unique -- it is a Special Judge, so
// the sample's edge list need only match as a set, in any order and with the
// endpoints of each pair in either order.  Duplicate or reversed pairs in the
// existing-highway list are tolerated defensively even though the statement
// says each pair appears at most once.

#include <cstdio>

static const int MAXN = 755;

static int X[MAXN], Y[MAXN];
static int W[MAXN][MAXN];
static int dist_[MAXN];
static int par[MAXN];
static bool used[MAXN];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int i, j;
    for (i = 1; i <= n; ++i) scanf("%d %d", &X[i], &Y[i]);
    for (i = 1; i <= n; ++i) {
        for (j = i + 1; j <= n; ++j) {
            int dx = X[i] - X[j];
            int dy = Y[i] - Y[j];
            int d = dx * dx + dy * dy;
            W[i][j] = d;
            W[j][i] = d;
        }
        W[i][i] = 0;
    }
    int m;
    if (scanf("%d", &m) != 1) m = 0;
    for (i = 0; i < m; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        if (a >= 1 && a <= n && b >= 1 && b <= n && a != b) {
            W[a][b] = 0;
            W[b][a] = 0;
        }
    }

    const int INF = 0x3f3f3f3f;
    for (i = 1; i <= n; ++i) { dist_[i] = INF; par[i] = 0; used[i] = false; }
    dist_[1] = 0;
    for (int it = 0; it < n; ++it) {
        int best = -1;
        for (i = 1; i <= n; ++i)
            if (!used[i] && (best == -1 || dist_[i] < dist_[best])) best = i;
        used[best] = true;
        if (par[best] != 0 && dist_[best] > 0)
            printf("%d %d\n", par[best], best);
        const int *wr = W[best];
        for (i = 1; i <= n; ++i) {
            if (!used[i] && wr[i] < dist_[i]) { dist_[i] = wr[i]; par[i] = best; }
        }
    }
    return 0;
}
