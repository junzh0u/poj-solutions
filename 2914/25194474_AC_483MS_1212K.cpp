// POJ 2914 - Minimum Cut
//
// Global minimum cut of an undirected weighted graph: no source or sink is
// given, so max-flow would have to be run over O(V) source/sink pairs.
// Stoer-Wagner finds it directly in O(V^3) on an adjacency matrix, which is
// what the N <= 500 bound is sized for.
//
// Each "minimum cut phase" grows a set A from an arbitrary start, repeatedly
// absorbing the vertex most tightly connected to A (maximum sum of edge
// weights into A).  The last vertex added, t, and its predecessor s give a cut
// of the whole graph whose weight is exactly w[t] at the moment t is absorbed,
// and that cut is provably a minimum s-t cut.  So take it as a candidate, then
// merge s and t into one vertex and repeat: any global min cut either
// separates s from t (candidate already recorded) or does not (unaffected by
// the merge).  After V-1 phases the smallest candidate is the global minimum.
//
// Statement notes:
//  - "two vertices can be connected by multiple edges": a repeated pair must
//    have its multiplicities *summed* into the matrix, not overwritten.  The
//    input can also list the same pair in either order (A B / B A).
//  - "If the graph is disconnected, print 0" needs no special case: a
//    disconnected graph has a zero-weight cut, and Stoer-Wagner reports it.
//  - M = 0 and N = 2 are both legal and fall out of the same code path.
//
// The matrix is kept compacted: merging s,t rewrites row/column s and then
// swaps the last live row/column into t's slot, so all indexing stays direct
// (no indirection table) and the hot inner loops walk contiguous memory.
#include <cstdio>
#include <cstring>

const int MAXN = 505;
const int INF = 0x3f3f3f3f;

static int g[MAXN][MAXN];
static int w[MAXN];
static bool vis[MAXN];

int n, m;

int stoerWagner(int vn) {
    int best = INF;
    while (vn > 1) {
        for (int i = 0; i < vn; i++) { vis[i] = false; w[i] = 0; }
        int prev = -1, last = -1;
        for (int i = 0; i < vn; i++) {
            int sel = -1;
            for (int j = 0; j < vn; j++)
                if (!vis[j] && (sel < 0 || w[j] > w[sel])) sel = j;
            vis[sel] = true;
            prev = last;
            last = sel;
            if (i + 1 == vn) {
                if (w[sel] < best) best = w[sel];
                /* merge `last` into `prev` */
                int *gp = g[prev], *gl = g[last];
                for (int j = 0; j < vn; j++) {
                    gp[j] += gl[j];
                    g[j][prev] = gp[j];
                }
                /* drop row/col `last` by swapping in the final one */
                if (last != vn - 1) {
                    int *gt = g[vn - 1];
                    for (int j = 0; j < vn; j++) gl[j] = gt[j];
                    for (int j = 0; j < vn; j++) g[j][last] = g[j][vn - 1];
                }
                vn--;
            } else {
                int *gs = g[sel];
                for (int j = 0; j < vn; j++)
                    if (!vis[j]) w[j] += gs[j];
            }
        }
    }
    return best;
}

int main() {
    while (scanf("%d %d", &n, &m) == 2) {
        for (int i = 0; i < n; i++)
            memset(g[i], 0, sizeof(int) * n);
        for (int i = 0; i < m; i++) {
            int a, b, c;
            scanf("%d %d %d", &a, &b, &c);
            if (a == b) continue;
            g[a][b] += c;
            g[b][a] += c;
        }
        printf("%d\n", n < 2 ? 0 : stoerWagner(n));
    }
    return 0;
}
