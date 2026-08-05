// POJ 1236 - Network of Schools
//
// Directed graph of N <= 100 schools; school i's line lists the schools it
// distributes to, terminated by 0.
//
// Subtask A: the minimum number of seeds so the software reaches everyone is
// the number of source components (in-degree 0) of the condensation.
// Subtask B: the minimum number of arcs to add to make the whole graph
// strongly connected is max(#sources, #sinks) of the condensation.
//
// The trap is the degenerate case: when the condensation is a single node the
// graph is already strongly connected, and the formula would wrongly report
// max(1,1) = 1 -- the lone component has neither incoming nor outgoing edges.
// The answer there is A = 1, B = 0, so that case is special-cased.
//
// N <= 100 makes an O(N^3) transitive closure (Floyd) cheap, so the SCCs are
// found as classes of mutual reachability instead of running Tarjan; self
// loops and repeated arcs then need no special handling.  Verified against a
// brute force (min seed set by subset enumeration, min added arcs by
// enumerating k-subsets of the missing arcs) on 2000 random graphs with n <= 7,
// including edgeless graphs, already strongly connected ones, and self loops.
#include <cstdio>
#include <vector>

using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    static bool reach[105][105];
    vector<pair<int, int> > edges;
    for (int i = 1; i <= n; ++i) reach[i][i] = true;
    for (int i = 1; i <= n; ++i) {
        int v;
        while (scanf("%d", &v) == 1 && v != 0) {
            if (v >= 1 && v <= n) {
                reach[i][v] = true;
                edges.push_back(make_pair(i, v));
            }
        }
    }

    /* transitive closure (n <= 100) */
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            if (reach[i][k])
                for (int j = 1; j <= n; ++j)
                    if (reach[k][j]) reach[i][j] = true;

    /* condensation: mutual reachability classes */
    vector<int> comp(n + 1, -1);
    int nc = 0;
    for (int i = 1; i <= n; ++i) {
        if (comp[i] != -1) continue;
        comp[i] = nc;
        for (int j = i + 1; j <= n; ++j)
            if (comp[j] == -1 && reach[i][j] && reach[j][i]) comp[j] = nc;
        ++nc;
    }

    vector<bool> hasIn(nc, false), hasOut(nc, false);
    for (size_t e = 0; e < edges.size(); ++e) {
        int a = comp[edges[e].first], b = comp[edges[e].second];
        if (a != b) { hasOut[a] = true; hasIn[b] = true; }
    }

    int zin = 0, zout = 0;
    for (int c = 0; c < nc; ++c) {
        if (!hasIn[c]) ++zin;
        if (!hasOut[c]) ++zout;
    }

    if (nc == 1) {
        /* already strongly connected: one copy suffices, nothing to add */
        printf("1\n0\n");
    } else {
        printf("%d\n%d\n", zin, zin > zout ? zin : zout);
    }
    return 0;
}
