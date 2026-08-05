// POJ 1947 - Rebuilding Roads
//
// A tree of N <= 150 barns; find the minimum number of edges to destroy so that
// some connected component of exactly P nodes is isolated.
//
// Tree knapsack. Root the tree and let dp[v][k] be the minimum number of edges
// cut strictly inside subtree(v) so that the component containing v holds
// exactly k nodes. Base: dp[v][1] = (number of children of v), i.e. cut every
// child edge. Merging a child c into v is a knapsack step: keeping the edge v-c
// means undoing the cut already charged in the base, so
//     dp[v][k+j] = min(dp[v][k] + dp[c][j] - 1)   for j >= 1.
// Bounding k by the running subtree size and j by sz[c] keeps the merge O(N^2)
// overall.
//
// The root's accounting is the only subtlety: a component topped by v != root
// must also sever v from its parent, so its cost is dp[v][P] + 1, whereas the
// root has no parent edge and costs dp[root][P]. The answer is the minimum of
// that over all v.
//
// The statement gives each edge as "I is J's parent", but the quantity asked
// for -- the minimum boundary size of a connected P-node set -- does not depend
// on where the tree is rooted, so this reads the edges as undirected and roots
// at node 1. That also makes the code immune to any input line that lists the
// pair the other way round.
#include <cstdio>
#include <vector>

using namespace std;

const int MAXN = 155;
const int INF = 1000000;

int n, p;
vector<int> adj[MAXN];
int par[MAXN];
int dp[MAXN][MAXN]; /* dp[v][k]: min edges cut inside subtree(v) leaving v in a component of k nodes */
int sz[MAXN];
int order[MAXN];
int nch[MAXN];

int main() {
    if (scanf("%d %d", &n, &p) != 2) return 0;
    for (int e = 0; e < n - 1; e++) {
        int a, b;
        if (scanf("%d %d", &a, &b) != 2) break;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    /* root the tree at 1; the answer does not depend on the choice of root */
    int cnt = 0, top = 0;
    int stk[MAXN];
    stk[top++] = 1;
    par[1] = 0;
    while (top) {
        int v = stk[--top];
        order[cnt++] = v;
        for (size_t i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i];
            if (u != par[v]) { par[u] = v; stk[top++] = u; }
        }
    }

    for (int idx = cnt - 1; idx >= 0; idx--) {
        int v = order[idx];
        for (int k = 0; k <= n; k++) dp[v][k] = INF;
        nch[v] = 0;
        for (size_t i = 0; i < adj[v].size(); i++) if (adj[v][i] != par[v]) nch[v]++;
        dp[v][1] = nch[v]; /* keep v alone: cut every child edge */
        sz[v] = 1;
        for (size_t i = 0; i < adj[v].size(); i++) {
            int c = adj[v][i];
            if (c == par[v]) continue;
            int nsz = sz[v] + sz[c];
            for (int k = nsz; k >= 2; k--) {
                int hi = sz[c];
                if (hi > k - 1) hi = k - 1;
                for (int j = 1; j <= hi; j++) {
                    if (dp[v][k - j] >= INF || dp[c][j] >= INF) continue;
                    /* keep edge v-c: undo the cut counted in dp[v][k-j] */
                    int cand = dp[v][k - j] + dp[c][j] - 1;
                    if (cand < dp[v][k]) dp[v][k] = cand;
                }
            }
            sz[v] = nsz;
        }
    }

    int best = INF;
    for (int v = 1; v <= n; v++) {
        if (dp[v][p] >= INF) continue;
        int cost = dp[v][p] + (v == 1 ? 0 : 1); /* non-root: also cut the parent edge */
        if (cost < best) best = cost;
    }
    printf("%d\n", best);
    return 0;
}
