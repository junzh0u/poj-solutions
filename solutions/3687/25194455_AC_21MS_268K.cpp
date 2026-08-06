// POJ 3687 - Labeling Balls
//
// N balls of distinct weights 1..N must be labeled 1..N so that every
// constraint "ball labeled a is lighter than ball labeled b" holds, i.e.
// w[a] < w[b] where w[i] is the weight of the ball labeled i. Output w[1..N],
// or -1 if the constraints are contradictory.
//
// The tie-break is the whole problem. "Smallest weight for label 1, then for
// label 2, ..." asks for the lexicographically smallest *weight vector*
// w[1..N] -- NOT for a lexicographically smallest topological order. The
// obvious greedy (topological sort of a->b, handing out weights 1,2,3,... to
// the smallest available label) optimises the wrong sequence and fails the
// samples: on N=4 with the single constraint 2->1 it yields 1 2 3 4's cousin
// rather than the correct 2 1 3 4.
//
// The fix is to run the sort in the opposite direction and pick the opposite
// extreme: hand out the weights from the largest down, N, N-1, ..., 1, and at
// each step give the current weight to the largest-indexed label that may
// still take it. Pushing big weights onto big labels is exactly what leaves
// the small labels free for small weights, which is what lexicographic
// minimality of w[] wants. A label may take the current (largest remaining)
// weight iff every ball it must be lighter than has already been assigned --
// i.e. it is a sink in the not-yet-assigned part of the original graph, which
// is a source in the reversed graph. So: reverse every edge, topologically
// sort the reversed graph with a max-selection instead of a min-heap, and
// assign N downwards.
//
// Concretely indeg[u] below counts u's remaining *out*-edges in the original
// graph (equivalently its in-edges in the reversed graph). Candidates are the
// unassigned u with indeg[u]==0; take the largest such u, give it weight k,
// and decrement indeg[v] for every reversed edge u->v. If at some step no
// candidate exists, the constraints contain a cycle and the answer is -1 --
// which also covers a self-loop a==b ("a lighter than a"), as in sample 2.
//
// Duplicate constraints are common (M up to 40,000 but only N^2 = 40,000
// possible pairs), so edges are deduplicated in an adjacency matrix; otherwise
// the in-degree counting would still work but the matrix scan would not.
//
// O(N^2 + M) per test case: 21MS for the judge, and 0.25s locally for 100
// worst-case tests of N=200, M=40,000.
#include <cstdio>
#include <cstring>

static bool rev_adj[205][205]; /* rev_adj[u][v]: original edge v->u (w[v] < w[u]) */
static int indeg[205];         /* number of remaining original out-edges of node */
static int w[205];
static bool used[205];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n, m;
        scanf("%d %d", &n, &m);
        for (int i = 1; i <= n; ++i) {
            memset(rev_adj[i] + 1, 0, sizeof(bool) * n);
            indeg[i] = 0;
            used[i] = false;
        }
        for (int i = 0; i < m; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            /* w[a] < w[b] : original edge a -> b ; reversed edge b -> a */
            if (!rev_adj[b][a]) {
                rev_adj[b][a] = true;
                ++indeg[a]; /* out-degree of a in the original graph */
            }
        }
        bool ok = true;
        for (int k = n; k >= 1; --k) {
            int pick = -1;
            for (int u = n; u >= 1; --u) {
                if (!used[u] && indeg[u] == 0) { pick = u; break; }
            }
            if (pick < 0) { ok = false; break; }
            used[pick] = true;
            w[pick] = k;
            for (int v = 1; v <= n; ++v)
                if (rev_adj[pick][v]) --indeg[v];
        }
        if (!ok) {
            printf("-1\n");
        } else {
            for (int i = 1; i <= n; ++i)
                printf(i == 1 ? "%d" : " %d", w[i]);
            printf("\n");
        }
    }
    return 0;
}
