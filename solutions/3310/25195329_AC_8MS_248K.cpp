// POJ 3310 - Caterpillar
// Model: claude-opus-5
//
// A graph is a caterpillar iff it is a tree whose non-leaf nodes induce a path.
//
// Checks, in order:
//   1. Union-find over every edge. Any edge joining two already-connected nodes
//      closes a cycle (this also catches self-loops and repeated edges, both of
//      which the statement's "no cycles" forbids and the input does not exclude).
//   2. All of 1..n in one component.
//   3. Strip the leaves: in a tree the nodes of degree >= 2 induce a connected
//      subtree, so that subtree is a path exactly when no such node has three or
//      more neighbours of degree >= 2.  n <= 2 leaves the set empty, which is the
//      trivial path.
//
// Node ids are only assumed to be positive; arrays are oversized so an id beyond
// n would surface as an extra component rather than as memory corruption.

#include <cstdio>
#include <cstring>

const int MAXV = 1005;
const int MAXE = 1005;

int parent[MAXV];
int deg[MAXV];
int ea[MAXE], eb[MAXE];

int find_set(int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

int main() {
    int n;
    int gid = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        int e;
        if (scanf("%d", &e) != 1) break;

        for (int i = 0; i < MAXV; ++i) {
            parent[i] = i;
            deg[i] = 0;
        }

        bool cyclic = false;
        for (int i = 0; i < e; ++i) {
            int a, b;
            scanf("%d%d", &a, &b);
            if (a < 0 || a >= MAXV) a = 0;
            if (b < 0 || b >= MAXV) b = 0;
            ea[i] = a;
            eb[i] = b;
            ++deg[a];
            ++deg[b];
            int ra = find_set(a), rb = find_set(b);
            if (ra == rb) cyclic = true;
            else parent[ra] = rb;
        }

        bool ok = !cyclic;
        if (ok) {
            int root = find_set(1);
            for (int v = 2; v <= n && ok; ++v)
                if (find_set(v) != root) ok = false;
        }
        if (ok) {
            // internal-neighbour count for every node of degree >= 2
            int cnt[MAXV];
            memset(cnt, 0, sizeof(cnt));
            for (int i = 0; i < e; ++i) {
                if (deg[ea[i]] >= 2 && deg[eb[i]] >= 2) {
                    ++cnt[ea[i]];
                    ++cnt[eb[i]];
                }
            }
            for (int v = 1; v <= n && ok; ++v)
                if (deg[v] >= 2 && cnt[v] > 2) ok = false;
        }

        ++gid;
        printf("Graph %d is %sa caterpillar.\n", gid, ok ? "" : "not ");
    }
    return 0;
}
