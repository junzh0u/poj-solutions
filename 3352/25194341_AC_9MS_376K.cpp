// POJ 3352 - Road Construction
//
// Given a connected, simple (no parallel edges) undirected graph, find the
// minimum number of edges to add so the graph becomes 2-edge-connected
// (stays connected after removing any single edge / road under
// construction).
//
// Approach: contract every 2-edge-connected component (found via a bridge
// DFS with low-link values) into a single node. What remains is the
// "bridge tree" T, whose edges are exactly the graph's bridges. Let L be
// the number of leaves (degree-1 nodes) of T. The classic result for
// making a tree 2-edge-connected with the fewest added edges is
// ceil(L / 2): pair up leaves (e.g. walking the leaves in DFS order and
// connecting leaf i with leaf i + L/2) so every tree edge ends up covered
// by at least one new cycle. If the whole graph is already one
// 2-edge-connected component (T has a single node), the answer is 0.
//
// Implementation notes:
//  - Bridge/low-link DFS is done iteratively (explicit stack) to avoid any
//    recursion-depth concerns at n = 1000.
//  - Edges are stored as directed arc pairs (2k, 2k+1); the DFS skips only
//    the arc that is the *reverse of the specific arc used to enter* the
//    current node (compared by edge id via XOR 1), not "any edge back to
//    the parent vertex" - the statement here guarantees at most one road
//    between any pair of attractions, so this distinction doesn't bite on
//    this particular input, but it's the general-purpose correct way to
//    skip the parent edge.
//  - Ambiguity: none really - the statement already assures the input is
//    connected and simple, so no need to special-case disconnected input
//    or parallel edges.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1005;
const int MAXM = 2005; // r<=1000 undirected edges -> 2000 directed arcs

int head[MAXN], nxt[MAXM], to_[MAXM], ecnt;
int disc[MAXN], low[MAXN], timer_;
bool isBridge[MAXM];
int comp[MAXN], compCnt;
int n, r;

void addEdge(int u, int v) {
    to_[ecnt] = v; nxt[ecnt] = head[u]; head[u] = ecnt++;
}

// iterative DFS to avoid recursion depth issues, computing disc/low and bridge marking
void dfsBridges(int start) {
    // stack frames: node, parent edge id (edge used to enter this node), iterator position (edge id currently at)
    static int stkNode[MAXN];
    static int stkPE[MAXN];
    static int stkIt[MAXN];
    int sp = 0;
    stkNode[sp] = start; stkPE[sp] = -1; stkIt[sp] = head[start];
    disc[start] = low[start] = timer_++;
    sp++;
    while (sp > 0) {
        int u = stkNode[sp-1];
        int pe = stkPE[sp-1];
        int &it = stkIt[sp-1];
        if (it != -1) {
            int e = it;
            it = nxt[e];
            if (e == (pe ^ 1)) continue; // skip the reverse of the edge we came in on (by edge id, not by vertex)
            int v = to_[e];
            if (disc[v] == -1) {
                disc[v] = low[v] = timer_++;
                stkNode[sp] = v; stkPE[sp] = e; stkIt[sp] = head[v];
                sp++;
            } else {
                low[u] = min(low[u], disc[v]);
            }
        } else {
            sp--;
            if (sp > 0) {
                int parent = stkNode[sp-1];
                low[parent] = min(low[parent], low[u]);
                if (low[u] > disc[parent]) {
                    isBridge[pe] = true;
                    isBridge[pe ^ 1] = true;
                }
            }
        }
    }
}

void dfsComp(int start, int id) {
    static int stk[MAXN];
    int sp = 0;
    stk[sp++] = start;
    comp[start] = id;
    while (sp > 0) {
        int u = stk[--sp];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (isBridge[e]) continue;
            int v = to_[e];
            if (comp[v] == -1) {
                comp[v] = id;
                stk[sp++] = v;
            }
        }
    }
}

int main() {
    if (scanf("%d %d", &n, &r) != 2) return 0;
    memset(head, -1, sizeof(head));
    ecnt = 0;
    for (int i = 0; i < r; i++) {
        int v, w;
        scanf("%d %d", &v, &w);
        addEdge(v, w);
        addEdge(w, v);
    }
    memset(disc, -1, sizeof(disc));
    memset(isBridge, 0, sizeof(isBridge));
    timer_ = 0;
    for (int i = 1; i <= n; i++) {
        if (disc[i] == -1) dfsBridges(i);
    }
    memset(comp, -1, sizeof(comp));
    compCnt = 0;
    for (int i = 1; i <= n; i++) {
        if (comp[i] == -1) {
            dfsComp(i, compCnt);
            compCnt++;
        }
    }
    if (compCnt <= 1) {
        printf("0\n");
        return 0;
    }
    vector<int> deg(compCnt, 0);
    for (int u = 1; u <= n; u++) {
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (isBridge[e] && e % 2 == 0) { // count each bridge once (each undirected edge added as pair e, e+1)
                int a = comp[u];
                int b = comp[to_[e]];
                deg[a]++;
                deg[b]++;
            }
        }
    }
    int leaves = 0;
    for (int i = 0; i < compCnt; i++) if (deg[i] == 1) leaves++;
    printf("%d\n", (leaves + 1) / 2);
    return 0;
}
