// POJ 2486 - Apple Tree
//
// A rooted tree (root = node 1), each node holding some apples; a walk starting
// at node 1 may take at most K steps along edges and collects the apples of
// every distinct node it visits.  Maximise the total.
//
// Tree knapsack over the step budget, with a second state dimension for whether
// the walk comes back:
//   dp[u][j][0] = best apples collectable using j steps inside subtree(u),
//                 starting at u and ENDING at u  (a closed tour)
//   dp[u][j][1] = best apples using j steps inside subtree(u), starting at u
//                 and ending anywhere in the subtree (an open walk)
// Base: both are w[u] for every j (the walk may simply stay put).
//
// Merging a child v into u, with t steps spent inside v:
//   closed:  dp[u][j][0] <- dp[u][j-t-2][0] + dp[v][t][0]   (down and back, 2 edges)
//   open:    dp[u][j][1] <- dp[u][j-t-2][1] + dp[v][t][0]   (the open part was in an
//                                                            earlier child; v is a
//                                                            detour taken before it)
//            dp[u][j][1] <- dp[u][j-t-1][0] + dp[v][t][1]   (everything else closed,
//                                                            the walk ends inside v)
// The first open rule is the one that is easy to miss: a child that is entered
// and returned from can still be followed by ending somewhere else.  Iterating j
// downwards keeps the dp[u][.] values on the right-hand side pre-merge.
// Answer: dp[1][K][1].
//
// Notes on the statement: the input is a stream of test cases ended by EOF, the
// walk always starts at node 1, and apples are counted once per node, so a node
// re-entered later adds nothing.  N=1 (no edge lines) and K=0 are both legal.
//
// The post-order is produced from a BFS ordering walked in reverse rather than
// by recursion, so a 100-node path costs no stack.
// O(N*K^2) per case, ~1.6ms at N=100, K=200.

#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

static const int MAXN = 105;
static const int MAXK = 205;

int n, k;
int w[MAXN];
vector<int> adj[MAXN];
int dp[MAXN][MAXK][2];   /* dp[u][j][0] = best using j steps in subtree(u), start and END at u
                            dp[u][j][1] = best using j steps in subtree(u), start at u, end anywhere */
int par[MAXN];
int order[MAXN];

static void buildOrder() {
    /* iterative BFS from node 1 -> order[] is a valid top-down order; reverse is post-order */
    int head = 0, tail = 0;
    order[tail++] = 1;
    par[1] = 0;
    while (head < tail) {
        int u = order[head++];
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (v == par[u]) continue;
            par[v] = u;
            order[tail++] = v;
        }
    }
}

int main() {
    while (scanf("%d %d", &n, &k) == 2) {
        int i;
        for (i = 1; i <= n; ++i) { adj[i].clear(); scanf("%d", &w[i]); }
        for (i = 0; i < n - 1; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            adj[a].push_back(b);
            adj[b].push_back(a);
        }
        buildOrder();

        for (i = n - 1; i >= 0; --i) {
            int u = order[i];
            int j;
            for (j = 0; j <= k; ++j) { dp[u][j][0] = w[u]; dp[u][j][1] = w[u]; }
            for (size_t c = 0; c < adj[u].size(); ++c) {
                int v = adj[u][c];
                if (v == par[u]) continue;
                for (j = k; j >= 1; --j) {
                    int best0 = dp[u][j][0];
                    int best1 = dp[u][j][1];
                    int t;
                    /* enter v, come back: costs 2 + t */
                    for (t = 0; t + 2 <= j; ++t) {
                        int cand = dp[u][j - t - 2][0] + dp[v][t][0];
                        if (cand > best0) best0 = cand;
                        cand = dp[u][j - t - 2][1] + dp[v][t][0];
                        if (cand > best1) best1 = cand;
                    }
                    /* enter v and stop there: costs 1 + t */
                    for (t = 0; t + 1 <= j; ++t) {
                        int cand = dp[u][j - t - 1][0] + dp[v][t][1];
                        if (cand > best1) best1 = cand;
                    }
                    dp[u][j][0] = best0;
                    dp[u][j][1] = best1;
                }
            }
            /* keep monotone in j */
            for (j = 1; j <= k; ++j) {
                if (dp[u][j][0] < dp[u][j - 1][0]) dp[u][j][0] = dp[u][j - 1][0];
                if (dp[u][j][1] < dp[u][j - 1][1]) dp[u][j][1] = dp[u][j - 1][1];
            }
        }
        printf("%d\n", dp[1][k][1]);
    }
    return 0;
}
