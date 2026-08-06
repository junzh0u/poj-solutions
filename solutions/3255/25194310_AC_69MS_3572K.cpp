// POJ 3255 - Roadblocks
//
// Second-shortest path from node 1 to node N, where the second-shortest
// walk may revisit vertices/edges and only needs to be strictly longer
// than the shortest walk's length (ties with the shortest don't count,
// even if there are several shortest paths). Because backtracking is
// allowed, the answer can be as small as shortest + 2*(one edge weight),
// e.g. a two-node graph with a single edge: the second-shortest path is
// forced to walk the edge out and back before crossing it again.
//
// Approach: modified Dijkstra tracking, for every node, both its
// shortest distance dist1[] and its second-shortest distance dist2[]
// (strictly greater than dist1[]). Relax each edge against both arrays:
//   - if it improves dist1[v], the old dist1[v] becomes a candidate for
//     dist2[v] (since it's still reachable via that edge, just no longer
//     the shortest);
//   - else if it's strictly between dist1[v] and dist2[v], it improves
//     dist2[v] directly.
// Each (distance, node) pair is pushed onto the heap as it's produced;
// a pop is skipped once its distance exceeds the node's current
// dist2[], which bounds each node to at most two "useful" pops. The
// answer is dist2[N].
//
// Verified against a brute-force k-shortest-walks reference (bounded-pop
// Dijkstra without the dist1/dist2 dedup) on 500 random small graphs,
// plus explicit two-node cases with a single edge (forces backtracking)
// and with parallel edges (the second edge is cheaper than backtracking).
#include <cstdio>
#include <vector>
#include <queue>
#include <utility>
using namespace std;

const int MAXN = 5005;
const long long INF = 1LL << 60;

int N, R;
vector<pair<int,int> > adj[MAXN]; // (to, weight)
long long dist1[MAXN], dist2[MAXN];

int main() {
    scanf("%d %d", &N, &R);
    for (int i = 0; i < R; i++) {
        int a, b, d;
        scanf("%d %d %d", &a, &b, &d);
        adj[a].push_back(make_pair(b, d));
        adj[b].push_back(make_pair(a, d));
    }

    for (int i = 1; i <= N; i++) {
        dist1[i] = INF;
        dist2[i] = INF;
    }
    dist1[1] = 0;

    // priority_queue of (dist, node), min-heap
    priority_queue<pair<long long,int>, vector<pair<long long,int> >, greater<pair<long long,int> > > pq;
    pq.push(make_pair(0LL, 1));

    while (!pq.empty()) {
        pair<long long,int> top = pq.top();
        pq.pop();
        long long d = top.first;
        int u = top.second;
        if (d > dist2[u]) continue;
        for (size_t i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i].first;
            long long nd = d + adj[u][i].second;
            if (nd < dist1[v]) {
                long long oldDist1 = dist1[v];
                dist1[v] = nd;
                pq.push(make_pair(dist1[v], v));
                if (oldDist1 < dist2[v]) {
                    dist2[v] = oldDist1;
                    pq.push(make_pair(dist2[v], v));
                }
            } else if (nd > dist1[v] && nd < dist2[v]) {
                dist2[v] = nd;
                pq.push(make_pair(dist2[v], v));
            }
        }
    }

    printf("%lld\n", dist2[N]);
    return 0;
}
