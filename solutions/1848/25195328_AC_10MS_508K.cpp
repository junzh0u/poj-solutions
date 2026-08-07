// POJ 1848 - Tree
// Model: claude-opus-5
//
// Add the fewest edges to a tree so that every vertex lies on exactly one cycle.
//
// Model.  Adding k edges to a spanning tree gives a graph whose cycle space has
// dimension k.  Two cycles sharing a vertex would put that vertex on more than one
// cycle, so the k cycles must be pairwise vertex-disjoint, and together they must
// cover V.  Each added edge (u,v) closes exactly the tree path u..v, so the task is:
// partition the vertices into vertex-disjoint tree paths, each with at least 3
// vertices, minimizing the number of parts.  The answer is that number, or -1.
//
// Ambiguity, settled by the discuss board (the statement defines neither "cycle" nor
// the input framing):
//   * a cycle needs >= 3 vertices -- self-loops and doubled edges do not count,
//     which is exactly why -1 is ever printed;
//   * the input holds several data sets concatenated with no count, and must be read
//     to EOF.  Treating it as a single data set draws Wrong Answer.
//
// DP.  Root at 1, process vertices in reverse BFS order (iterative, so no recursion
// depth risk).  For each vertex v:
//   dp0[v]   = min paths in subtree(v) when v's own path is closed inside subtree(v)
//              (v is the topmost vertex of that path);
//   dpo[v][L]= min closed paths in subtree(v) when v's path still runs up through the
//              parent, its part inside subtree(v) being a descending chain of L
//              vertices, L capped at 3 (only the ">= 3 vertices" threshold matters).
// Children are folded in with a small knapsack f[k][l]: k = how many children's open
// chains attach to v (at most 2, since v's path branches down at most twice), l = the
// capped total vertex count of those chains.  Then dpo[v][1]=f[0][0], dpo[v][2]=f[1][1],
// dpo[v][3]=min(f[1][2],f[1][3]), and closing at v costs 1 plus the best f[k][l] with
// k>=1 and 1+l >= 3.  Answer dp0[1], INF meaning -1.
//
// Verified: all labelled trees for n=3..6 and random ones for n=7,8 against a brute
// force that enumerates every subset of addable edges and every simple cycle straight
// from the statement's wording; plus the 100 test cases posted on the discuss board
// (n up to 54, answers up to 13 and many -1), all matching.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int INF = 1000000;
const int MAXN = 10005;

int n;
vector<int> adj[MAXN];
int par[MAXN], order_[MAXN];
int dp0[MAXN];
int dpo[MAXN][4];

static int addc(int a, int b) { return (a >= INF || b >= INF) ? INF : a + b; }

int main() {
    while (scanf("%d", &n) == 1) {
        if (n <= 0 || n >= MAXN) break;
        for (int i = 1; i <= n; i++) adj[i].clear();
        for (int i = 0; i < n - 1; i++) {
            int x, y;
            if (scanf("%d %d", &x, &y) != 2) return 0;
            if (x >= 1 && x <= n && y >= 1 && y <= n) {
                adj[x].push_back(y);
                adj[y].push_back(x);
            }
        }
        int cnt = 0;
        par[1] = 0;
        order_[cnt++] = 1;
        for (int i = 0; i < cnt; i++) {
            int u = order_[i];
            for (size_t k = 0; k < adj[u].size(); k++) {
                int w = adj[u][k];
                if (w != par[u]) { par[w] = u; order_[cnt++] = w; }
            }
        }
        for (int i = cnt - 1; i >= 0; i--) {
            int v = order_[i];
            int f[3][4];
            for (int a = 0; a < 3; a++)
                for (int b = 0; b < 4; b++) f[a][b] = INF;
            f[0][0] = 0;
            for (size_t k = 0; k < adj[v].size(); k++) {
                int c = adj[v][k];
                if (c == par[v]) continue;
                int g[3][4];
                for (int a = 0; a < 3; a++)
                    for (int b = 0; b < 4; b++) g[a][b] = INF;
                for (int a = 0; a < 3; a++) {
                    for (int b = 0; b < 4; b++) {
                        if (f[a][b] >= INF) continue;
                        g[a][b] = min(g[a][b], addc(f[a][b], dp0[c]));
                        if (a < 2) {
                            for (int L = 1; L <= 3; L++) {
                                int nb = b + L; if (nb > 3) nb = 3;
                                g[a + 1][nb] = min(g[a + 1][nb], addc(f[a][b], dpo[c][L]));
                            }
                        }
                    }
                }
                for (int a = 0; a < 3; a++)
                    for (int b = 0; b < 4; b++) f[a][b] = g[a][b];
            }
            dpo[v][1] = f[0][0];
            dpo[v][2] = f[1][1];
            dpo[v][3] = min(f[1][2], f[1][3]);
            int best = min(min(f[1][2], f[1][3]), min(f[2][2], f[2][3]));
            dp0[v] = addc(best, 1);
        }
        printf("%d\n", dp0[1] >= INF ? -1 : dp0[1]);
    }
    return 0;
}
