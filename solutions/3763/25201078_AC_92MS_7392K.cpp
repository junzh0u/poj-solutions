// POJ 3763 - Tour in Wonder Land
// Model: claude-sonnet-5
//
// The tour Alice wants is a Hamiltonian cycle on the n cities using each
// road (tree edge or newly built edge) at most once. Equivalently: choose
// a subset S of the tree's n-1 edges such that every vertex has degree
// <= 2 in S (S is then automatically a disjoint union of simple paths,
// since the tree has no cycles). If S has k edges it splits the n
// vertices into (n-k) vertex-disjoint paths (isolated vertices count as
// trivial paths); joining the path endpoints end-to-end with new edges
// closes them into one Hamiltonian cycle using exactly (n-k) new edges.
// So we must maximize k, i.e. find the maximum subset of tree edges with
// max degree 2 at every vertex ("maximum path cover" of the tree).
//
// Standard tree DP, rooted arbitrarily at vertex 1 (the answer is
// root-independent). For vertex v define:
//   g[v] = max edges selected within subtree(v) with deg(v) <= 2
//          (v not required to keep a free slot for a parent edge)
//   h[v] = max edges selected within subtree(v) with deg(v) <= 1
//          (v keeps one free slot, for a possible edge to its parent)
// For a child c with edge (v,c): using it contributes 1 + h[c] instead of
// g[c] (v spends one of its own degree slots, c must reserve one for
// this edge). delta_c = (1+h[c]) - g[c] is the gain from attaching c.
// sum_base = sum of g[c] over children. g[v] = sum_base + (top two
// positive deltas), h[v] = sum_base + (top one positive delta) -- v may
// attach at most 2 (resp. 1) children.  Answer = n - g[root].
//
// Iterative DFS (explicit stack) is used to avoid recursion-depth issues
// for a path-shaped tree with n up to 100000.
//
// Ambiguity check: the discuss board's only substantive note (message
// 150409, "it doesn't necessarily start from node 1") concerns a
// different (leaf-peeling) implementation's traversal choice; it does
// not apply here since this DP's root is arbitrary and the maximum path
// cover value is provably root-independent. Verified against a brute
// force (enumerate all 2^(n-1) edge subsets) on random trees n<=12, and
// against hand-derived answers for stars and paths.

#include <cstdio>
#include <vector>
using namespace std;

static const int MAXN = 100005;
vector<int> adj[MAXN];
int par[MAXN];
int order_[MAXN];
int g[MAXN], h[MAXN];

int main(){
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<char> visited(n + 1, 0);
    vector<int> stk;
    stk.reserve(n);
    int cnt = 0;
    stk.push_back(1);
    visited[1] = 1;
    par[1] = 0;
    while (!stk.empty()) {
        int u = stk.back();
        stk.pop_back();
        order_[cnt++] = u;
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (!visited[v]) {
                visited[v] = 1;
                par[v] = u;
                stk.push_back(v);
            }
        }
    }

    for (int idx = n - 1; idx >= 0; --idx) {
        int v = order_[idx];
        int sum_base = 0;
        int max1 = 0, max2 = 0;
        for (size_t i = 0; i < adj[v].size(); ++i) {
            int c = adj[v][i];
            if (c == par[v]) continue;
            sum_base += g[c];
            int delta = (1 + h[c]) - g[c];
            if (delta > max1) {
                max2 = max1;
                max1 = delta;
            } else if (delta > max2) {
                max2 = delta;
            }
        }
        g[v] = sum_base + max1 + max2;
        h[v] = sum_base + max1;
    }

    printf("%d\n", n - g[1]);
    return 0;
}
