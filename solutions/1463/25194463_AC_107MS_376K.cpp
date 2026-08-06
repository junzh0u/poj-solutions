// POJ 1463 - Strategic game
//
// Minimum vertex cover on a tree: pick the fewest nodes so that every edge has
// at least one endpoint picked.  Two-state tree DP, dp0[u] = best cover of u's
// subtree with u NOT taken (then every child must be taken), dp1[u] = best with
// u taken (each child free to choose).  The answer for a component is
// min(dp0[root], dp1[root]).  Because a vertex cover is a property of the
// undirected graph, the rooting is irrelevant -- the input's parent/child
// orientation can be ignored and any node used as root.
//
// Two things the statement leaves awkward:
//   * The lines "u:(k) c1 ... ck" may come in any order and the root is not
//     necessarily node 0, so the tree is built undirected and every unvisited
//     node is used as a DFS root (which also copes if the data were a forest).
//   * The only numeric text in the file is u, k and the children, with ':',
//     '(' and ')' acting as separators glued to the digits.  So instead of a
//     format string the reader just pulls out the next integer, skipping any
//     non-digit -- immune to whatever spacing the data actually uses, and it
//     doubles as the end-of-file test for the "several data sets" loop.
//
// n <= 1500 and the tree can be a 1500-node chain, so the traversal is an
// explicit stack, never recursion.  Nodes are appended when popped, so
// reversing the discovery order gives every child before its parent.
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

/* Reads the next integer appearing in the stream, treating every other
   character (':', '(', ')', spaces, newlines) as a separator.  Returns 0 on
   EOF.  The input format writes the node id glued to ':(' and the child count
   inside parentheses, and nothing else in the file is numeric, so simply
   picking out the integers in order recovers  u, k, c1..ck  for each line
   whatever the spacing happens to be. */
static bool readInt(int &out) {
    int c = getchar();
    while (c != EOF && (c < '0' || c > '9') && c != '-') c = getchar();
    if (c == EOF) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = getchar(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = getchar(); }
    out = sign * v;
    return true;
}

int main() {
    int n;
    while (readInt(n)) {
        if (n <= 0) {
            /* degenerate: nothing to cover */
            printf("0\n");
            continue;
        }
        vector< vector<int> > adj(n);
        for (int i = 0; i < n; ++i) {
            int u, k;
            if (!readInt(u)) { u = i; k = 0; }
            else if (!readInt(k)) k = 0;
            if (u < 0 || u >= n) u = 0;
            for (int j = 0; j < k; ++j) {
                int v;
                if (!readInt(v)) v = 0;
                if (v < 0 || v >= n) continue;
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }

        vector<int> dp0(n, 0), dp1(n, 0);
        vector<char> vis(n, 0);
        vector<int> parent(n, -1);
        vector<int> order;
        order.reserve(n);
        vector<int> stk;
        stk.reserve(n);

        int total = 0;
        for (int s = 0; s < n; ++s) {
            if (vis[s]) continue;
            order.clear();
            stk.clear();
            stk.push_back(s);
            vis[s] = 1;
            parent[s] = -1;
            while (!stk.empty()) {
                int u = stk.back();
                stk.pop_back();
                order.push_back(u);
                for (size_t i = 0; i < adj[u].size(); ++i) {
                    int v = adj[u][i];
                    if (!vis[v]) {
                        vis[v] = 1;
                        parent[v] = u;
                        stk.push_back(v);
                    }
                }
            }
            /* process in reverse discovery order: children before parents */
            for (int i = (int)order.size() - 1; i >= 0; --i) {
                int u = order[i];
                dp0[u] = 0;
                dp1[u] = 1;
                for (size_t j = 0; j < adj[u].size(); ++j) {
                    int v = adj[u][j];
                    if (v == parent[u]) continue;
                    dp0[u] += dp1[v];
                    dp1[u] += dp0[v] < dp1[v] ? dp0[v] : dp1[v];
                }
            }
            total += dp0[s] < dp1[s] ? dp0[s] : dp1[s];
        }
        printf("%d\n", total);
    }
    return 0;
}
