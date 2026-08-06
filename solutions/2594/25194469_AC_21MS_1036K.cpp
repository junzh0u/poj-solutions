// POJ 2594 - Treasure Exploration
//
// Cover every vertex of a DAG with the fewest directed paths, where — unlike
// the vertex-disjoint minimum path cover of 1422 — two robots' routes may
// share points ("the roads of two different robots may contain some same
// point").
//
// Because the graph is acyclic, reachability is a partial order. A robot's
// route is a directed path, so the points it explores form a chain in that
// order; conversely any chain v1 < v2 < ... < vk can be walked by one robot,
// concatenating the paths that realise each reachability step — the extra
// vertices passed through in between cost nothing precisely because routes are
// allowed to overlap. So the answer is the minimum number of chains covering
// V, and since dropping a repeated element from a chain leaves a chain, that
// equals the minimum chain *partition* of the poset.
//
// A minimum chain partition is a minimum vertex-disjoint path cover of the
// poset's own DAG — the transitive closure. So: close the graph transitively
// (reverse topological order, one bitset OR per edge), then run the standard
// n - (maximum bipartite matching) on the closure. Taking the closure first is
// the whole difference from 1422: an edge u->v in the closure means "u can
// reach v at all", which is exactly when one robot can serve both.
//
// n <= 500 and m <= 5000, so the closure holds up to ~125000 edges;
// Hopcroft-Karp runs it in O(E*sqrt(V)) and 500 maximum-size cases finish in
// well under a second.
//
// Statement notes: input is a stream of test cases terminated by "0 0" (the
// count is not given); the graph is stated acyclic, so Kahn's ordering is
// always complete; edges may repeat and A != B is guaranteed.
#include <cstdio>
#include <vector>
#include <bitset>
#include <queue>

using namespace std;

const int MAXN = 505;
const int INF = 1 << 29;

int n, m;
vector<int> g[MAXN];
int indeg[MAXN];
bitset<MAXN> reachb[MAXN];
vector<int> badj[MAXN];
int matchL[MAXN], matchR[MAXN], dst[MAXN];

bool bfsHK() {
    queue<int> Q;
    for (int u = 1; u <= n; ++u) {
        if (matchL[u] == 0) { dst[u] = 0; Q.push(u); }
        else dst[u] = INF;
    }
    bool found = false;
    while (!Q.empty()) {
        int u = Q.front(); Q.pop();
        for (size_t i = 0; i < badj[u].size(); ++i) {
            int v = badj[u][i];
            int w = matchR[v];
            if (w == 0) found = true;
            else if (dst[w] == INF) { dst[w] = dst[u] + 1; Q.push(w); }
        }
    }
    return found;
}

bool dfsHK(int u) {
    for (size_t i = 0; i < badj[u].size(); ++i) {
        int v = badj[u][i];
        int w = matchR[v];
        if (w == 0 || (dst[w] == dst[u] + 1 && dfsHK(w))) {
            matchL[u] = v; matchR[v] = u;
            return true;
        }
    }
    dst[u] = INF;
    return false;
}

int main() {
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        for (int i = 1; i <= n; ++i) {
            g[i].clear();
            badj[i].clear();
            reachb[i].reset();
            indeg[i] = 0;
            matchL[i] = 0;
            matchR[i] = 0;
        }
        for (int i = 0; i < m; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            g[a].push_back(b);
            ++indeg[b];
        }
        /* topological order (the graph is guaranteed acyclic) */
        vector<int> order;
        order.reserve(n);
        queue<int> Q;
        for (int i = 1; i <= n; ++i) if (indeg[i] == 0) Q.push(i);
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            order.push_back(u);
            for (size_t i = 0; i < g[u].size(); ++i) {
                int v = g[u][i];
                if (--indeg[v] == 0) Q.push(v);
            }
        }
        /* transitive closure, back to front along the topological order */
        for (int i = (int)order.size() - 1; i >= 0; --i) {
            int u = order[i];
            for (size_t j = 0; j < g[u].size(); ++j) {
                int v = g[u][j];
                reachb[u] |= reachb[v];
                reachb[u].set(v);
            }
        }
        for (int u = 1; u <= n; ++u)
            for (int v = 1; v <= n; ++v)
                if (reachb[u].test(v)) badj[u].push_back(v);
        /* Hopcroft-Karp maximum bipartite matching on the closure */
        int matching = 0;
        while (bfsHK()) {
            for (int u = 1; u <= n; ++u)
                if (matchL[u] == 0 && dfsHK(u)) ++matching;
        }
        printf("%d\n", n - matching);
    }
    return 0;
}
