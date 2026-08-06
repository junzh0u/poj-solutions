// POJ 3177 - Redundant Paths
//
// Given a connected undirected graph (possibly with parallel edges between
// the same pair of fields), find the minimum number of new edges to add so
// that every pair of vertices is connected by two edge-disjoint routes
// (i.e. the graph becomes 2-edge-connected).
//
// Approach: identical to POJ 3352 (Road Construction) - contract every
// 2-edge-connected component (found via bridge/low-link DFS) into a single
// node to get the "bridge tree" T. If T has a single node the graph is
// already 2-edge-connected and the answer is 0. Otherwise let L be the
// number of leaves (degree-1 nodes) of T; the minimum number of edges to
// add to make a tree 2-edge-connected is ceil(L / 2), achieved by pairing
// leaves from opposite ends of a DFS/leaf ordering so every tree edge ends
// up covered by some new cycle.
//
// The one real difference from 3352: this statement explicitly allows
// (and the hint even suggests) more than one road between the same pair
// of fields, so a doubled edge must NOT be treated as a bridge. A naive
// bridge DFS that skips "any edge back to the parent vertex" gets this
// wrong, since it would also skip the second parallel edge and wrongly
// call the first one a bridge. The fix is to skip only the exact reverse
// of the arc used to enter the current vertex, identified by edge id
// (arcs are stored as consecutive pairs 2k/2k+1, compared via XOR 1), not
// by "goes to the parent vertex". With that fix a second parallel edge is
// seen as a back edge to an already-discovered vertex and correctly pulls
// low[] down, so neither parallel edge is marked a bridge. Self-loops
// (not present in this problem, since paths "connect exactly two
// different fields") would also be handled correctly by this scheme, as
// they never match the entry arc's reverse id.
//
// Ambiguity: none - the statement guarantees the graph is already
// connected (R >= F-1), so no need to handle a disconnected start.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 5005;
const int MAXM = 20005; // r<=10000 undirected edges -> 20000 directed arcs

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
