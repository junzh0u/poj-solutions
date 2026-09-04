// POJ 2883 - Points
// Model: claude-sonnet-5
//
// Each rule "i rel j" fixes, for the x-coordinate and the y-coordinate
// separately, either an equality (xi=xj or yi=yj) or a strict order
// (xi<xj, xi>xj, yi<yj or yi>yj). N/S fix x equal and give a strict y
// order; E/W fix y equal and give a strict x order; the four diagonals
// give a strict order on both coordinates and no equality on either.
//
// So the x-axis and y-axis are two independent constraint systems, each
// solved the same way ("merge equal points with a union-find, then treat
// the strict relations as edges of a partial order" per the board's
// 50047/50053/101102 threads): union the equality pairs for that axis,
// then add a directed edge u->v meaning val(u) < val(v) for every strict
// pair, using the union-find root of each endpoint. A system of strict
// "<" constraints over the reals is satisfiable iff the resulting directed
// graph is acyclic (any DAG admits an increasing real assignment via a
// topological order); running plain cycle detection on that graph also
// covers a self-loop (an edge whose endpoints collapsed into the same
// equality class, or even a literal "i rel i" input) and a length-2 cycle
// from two directly conflicting rules on the same pair, so no separate
// special-casing of duplicate/degenerate pairs is needed. The whole
// answer is POSSIBLE iff both the x-system and the y-system are acyclic.
//
// The discuss board (message 50315) posts a good stress case: rules that
// only ever equate coordinates transitively (1 N 2, 2 N 4, 1 E 3, 3 E 4)
// force x1=x2=x4 and y1=y3=y4, but the very same rules also chain strict
// inequalities across those merged classes into a 2-node cycle on each
// axis, so it comes out IMPOSSIBLE without needing any extra
// "points must be distinct" rule; verified this by hand and reproduced
// it as a local test.
//
// n <= 500, m <= 10000, t <= 20: an O(n+m) union-find + DFS per axis per
// test case is trivial well within the 1s limit.

#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
using namespace std;

const int MAXN = 505;

int n, m;
int ri[10005], rj[10005];
char rel[10005][4];

int parent_[MAXN];

int find(int x) {
    int root = x;
    while (parent_[root] != root) root = parent_[root];
    while (parent_[x] != root) {
        int nxt = parent_[x];
        parent_[x] = root;
        x = nxt;
    }
    return root;
}

void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (a != b) parent_[a] = b;
}

vector<int> adj[MAXN];
int color_[MAXN];
bool hasCycle;

void dfs(int u) {
    color_[u] = 1;
    for (size_t k = 0; k < adj[u].size(); ++k) {
        int v = adj[u][k];
        if (color_[v] == 1) { hasCycle = true; return; }
        if (color_[v] == 0) {
            dfs(v);
            if (hasCycle) return;
        }
    }
    color_[u] = 2;
}

// axis == 0: X-axis. Equal relation = N,S. Strict "u<v" edge from E,NE,SE (i->j); W,NW,SW (j->i).
// axis == 1: Y-axis. Equal relation = E,W. Strict "u<v" edge from N,NE,NW (i->j); S,SE,SW (j->i).
bool isEqualRel(int axis, const char *r) {
    if (axis == 0) return strcmp(r, "N") == 0 || strcmp(r, "S") == 0;
    else return strcmp(r, "E") == 0 || strcmp(r, "W") == 0;
}

// returns +1 if this relation means val(i) < val(j) on this axis,
// -1 if it means val(j) < val(i) on this axis,
// 0 if it's the equality relation for this axis (handled separately).
int strictDir(int axis, const char *r) {
    if (axis == 0) {
        if (strcmp(r, "E") == 0 || strcmp(r, "NE") == 0 || strcmp(r, "SE") == 0) return 1;
        if (strcmp(r, "W") == 0 || strcmp(r, "NW") == 0 || strcmp(r, "SW") == 0) return -1;
        return 0;
    } else {
        if (strcmp(r, "N") == 0 || strcmp(r, "NE") == 0 || strcmp(r, "NW") == 0) return 1;
        if (strcmp(r, "S") == 0 || strcmp(r, "SE") == 0 || strcmp(r, "SW") == 0) return -1;
        return 0;
    }
}

bool axisConsistent(int axis) {
    for (int i = 1; i <= n; ++i) parent_[i] = i;
    for (int k = 0; k < m; ++k) {
        if (isEqualRel(axis, rel[k])) unite(ri[k], rj[k]);
    }
    for (int i = 1; i <= n; ++i) { adj[i].clear(); color_[i] = 0; }
    for (int k = 0; k < m; ++k) {
        int d = strictDir(axis, rel[k]);
        if (d == 0) continue;
        int a = ri[k], b = rj[k];
        if (d == -1) { int t = a; a = b; b = t; } // now a < b
        int ra = find(a), rb = find(b);
        adj[ra].push_back(rb); // ra -> rb means val(ra) < val(rb)
    }
    hasCycle = false;
    for (int i = 1; i <= n; ++i) {
        if (color_[i] == 0) {
            dfs(i);
            if (hasCycle) return false;
        }
    }
    return true;
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        scanf("%d %d", &n, &m);
        for (int k = 0; k < m; ++k) {
            scanf("%d %s %d", &ri[k], rel[k], &rj[k]);
        }
        bool ok = axisConsistent(0) && axisConsistent(1);
        printf(ok ? "POSSIBLE\n" : "IMPOSSIBLE\n");
    }
    return 0;
}
