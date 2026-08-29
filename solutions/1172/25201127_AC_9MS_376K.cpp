// POJ 1172 - Street Race
// Model: claude-sonnet-5
//
// The course is a graph on points 0..N (0=start, N=finish, no outgoing
// arrows from N). Well-formed: every point reachable from 0, N reachable
// from every point.
//
// Subtask A (unavoidable points): a point v (0<v<N) is unavoidable iff
// removing it disconnects 0 from N (standard dominator test).
//
// Subtask B (splitting points): for a candidate S (must already be
// unavoidable), let R1 = nodes reachable from 0 in the graph with S
// removed, and R2 = nodes that can reach N in the graph with S removed.
// Since S dominates, R1 and R2 are disjoint and there is no edge from R1
// to R2 (such an edge would give a 0->N path avoiding S). S is a
// splitting point iff:
//   (a) every node other than S lies in R1 or R2 (full coverage), and
//   (b) every edge (u,w) with w in R1 also has u in R1 (no edge enters
//       R1 "from outside" -- not from S, and not from an R2 node).
// Condition (b) captures both "S has an outgoing edge back into the
// day-1 side" and "a day-2 node has an edge back into the day-1 side",
// which is exactly what breaks the clean split into two well-formed
// courses sharing only S. Note: the problem's well-formedness only
// forbids outgoing edges from the overall finish -- it does NOT forbid
// edges into the overall start (0), and the board confirms this trips
// up naive symmetric assumptions. Because R1 always contains 0, any
// such edge into 0 from outside R1 is caught by rule (b) automatically.
//
// This was checked against: the official sample, a hand-built graph
// with edges feeding back into node 0 from downstream nodes (only some
// candidates survive rule (b) once those back-edges are considered),
// and a Floyd-Warshall based independent implementation of the same
// idea on randomized well-formed graphs.

#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

static int F; // finish node index (== N in the statement)
static vector<int> adj[55], radj[55];
static vector<pair<int,int> > edgeList;

// BFS from src avoiding node 'banned' (banned is skipped as both an
// intermediate and endpoint). useRev selects forward or reverse graph.
static void bfsAvoid(int src, int banned, bool useRev, vector<bool> &inSet) {
    inSet.assign(F + 1, false);
    if (src == banned) return;
    inSet[src] = true;
    queue<int> q;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        const vector<int> &lst = useRev ? radj[u] : adj[u];
        for (size_t i = 0; i < lst.size(); i++) {
            int v = lst[i];
            if (v == banned) continue;
            if (!inSet[v]) { inSet[v] = true; q.push(v); }
        }
    }
}

int main() {
    int cur = 0;
    int x;
    while (scanf("%d", &x) == 1) {
        if (x == -1) break;
        if (x == -2) { cur++; continue; }
        adj[cur].push_back(x);
        radj[x].push_back(cur);
        edgeList.push_back(make_pair(cur, x));
    }
    F = cur; // finish node index

    vector<int> unavoidable;
    for (int v = 1; v < F; v++) {
        vector<bool> reachFrom0;
        bfsAvoid(0, v, false, reachFrom0);
        if (!reachFrom0[F]) unavoidable.push_back(v);
    }

    vector<int> splitting;
    for (size_t idx = 0; idx < unavoidable.size(); idx++) {
        int S = unavoidable[idx];
        vector<bool> R1, R2;
        bfsAvoid(0, S, false, R1);   // forward reachable from 0, avoiding S
        bfsAvoid(F, S, true, R2);    // can reach F, avoiding S (reverse BFS from F)

        bool ok = true;
        // coverage: every node other than S must be in R1 or R2
        for (int v = 0; v <= F && ok; v++) {
            if (v == S) continue;
            if (!R1[v] && !R2[v]) ok = false;
        }
        // no edge enters R1 from outside R1
        for (size_t e = 0; e < edgeList.size() && ok; e++) {
            int u = edgeList[e].first, w = edgeList[e].second;
            if (R1[w] && !R1[u]) ok = false;
        }
        if (ok) splitting.push_back(S);
    }

    sort(unavoidable.begin(), unavoidable.end());
    sort(splitting.begin(), splitting.end());

    printf("%d", (int)unavoidable.size());
    for (size_t i = 0; i < unavoidable.size(); i++) printf(" %d", unavoidable[i]);
    printf("\n");
    printf("%d", (int)splitting.size());
    for (size_t i = 0; i < splitting.size(); i++) printf(" %d", splitting[i]);
    printf("\n");
    return 0;
}
