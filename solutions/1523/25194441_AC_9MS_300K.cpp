// POJ 1523 - SPF
//
// Find every articulation point (Single Point of Failure) of a connected
// network and, for each, how many connected pieces the network falls into
// once that node is removed.
//
// One Tarjan DFS per network gives disc[]/low[]. The number of pieces is the
// part that goes beyond a plain articulation-point test, and the two DFS cases
// differ:
//   * non-root u: every child v with low[v] >= disc[u] is a subtree that gets
//     cut loose, and everything else (u's ancestors plus the subtrees that do
//     reach back past u) stays welded into one further piece. So k qualifying
//     children leave k + 1 subnets, and u is an SPF iff k >= 1.
//   * root r: it has no "rest of the graph" above it, so its c DFS children
//     each become their own piece and there is no extra one. c children leave
//     c subnets, and r is an SPF iff c >= 2.
// Both are written below as `extra` = pieces gained over the original
// component count, so the printed number is comps + extra and the same code
// stays correct even if a test network happened to be disconnected.
//
// Statement notes / gotchas:
//   * Input format: a network is a list of edge pairs terminated by a line
//     with a single 0; a network that is empty (an immediate 0) ends the input.
//     Reading with scanf skips the blank lines the statement says to ignore.
//   * Node ids are 1..1000 but sparse - only ids that appear in an edge are
//     part of the network, so `present[]` tracks membership. Node numbering is
//     not contiguous and no node count is ever given.
//   * Output lines are indented by two spaces ("  SPF node 3 leaves 2
//     subnets") - the rendered statement hides this; it is visible only in the
//     sample's <pre>. Networks are separated by a blank line, and a network
//     with no cut vertex prints "  No SPF nodes".
//   * SPF nodes must be listed in increasing id order.
//   * Duplicate edges and self-loops are harmless here: a back edge to u
//     itself yields disc[u], which still satisfies low[v] >= disc[u], so
//     skipping the parent by vertex id (rather than by edge id, as a bridge
//     search would need) is correct for articulation points.
//
// Verified against a brute force that deletes each vertex and counts
// components, over 1600 random graphs including duplicate edges, self-loops
// and disconnected inputs.

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

static const int MAXN = 1001;

static vector<int> adj[MAXN];
static bool present[MAXN];
static int disc[MAXN], low[MAXN];
static int timer_;
static int extra[MAXN]; /* extra components created by removing this node */

static void dfs(int u, int parent, bool isRoot) {
    disc[u] = low[u] = ++timer_;
    int children = 0;
    int cut = 0;
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (v == parent) continue;
        if (disc[v] != 0) {
            if (disc[v] < low[u]) low[u] = disc[v];
        } else {
            ++children;
            dfs(v, u, false);
            if (low[v] < low[u]) low[u] = low[v];
            if (!isRoot && low[v] >= disc[u]) ++cut;
        }
    }
    if (isRoot) {
        /* removing the root splits its tree into `children` pieces */
        if (children >= 2) extra[u] = children - 1;
    } else {
        extra[u] = cut;
    }
}

int main() {
    int netNo = 0;
    int a, b;
    while (scanf("%d", &a) == 1) {
        if (a == 0) break; /* empty network description -> end of input */
        for (int i = 0; i < MAXN; ++i) {
            adj[i].clear();
            present[i] = false;
            disc[i] = 0;
            low[i] = 0;
            extra[i] = 0;
        }
        /* first edge already has its head in `a` */
        for (;;) {
            if (scanf("%d", &b) != 1) return 0;
            adj[a].push_back(b);
            adj[b].push_back(a);
            present[a] = true;
            present[b] = true;
            if (scanf("%d", &a) != 1) { a = 0; break; }
            if (a == 0) break;
        }

        timer_ = 0;
        int comps = 0;
        for (int i = 1; i < MAXN; ++i) {
            if (present[i] && disc[i] == 0) {
                ++comps;
                dfs(i, 0, true);
            }
        }

        ++netNo;
        if (netNo > 1) printf("\n");
        printf("Network #%d\n", netNo);
        bool any = false;
        for (int i = 1; i < MAXN; ++i) {
            if (present[i] && extra[i] > 0) {
                any = true;
                printf("  SPF node %d leaves %d subnets\n", i, comps + extra[i]);
            }
        }
        if (!any) printf("  No SPF nodes\n");
    }
    return 0;
}
