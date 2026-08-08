// POJ 2662 - A Walk Through the Forest
// Model: claude-opus-5
//
// Let dist[v] be the length of the shortest route from v to the house
// (node 2), so one Dijkstra run sourced at node 2 gives it for every v.
// "Taking a path from A to B is progress if there exists a route from B to
// his home that is shorter than any possible route from A" then reads as
// dist[B] < dist[A], strictly: the route from B must beat *every* route
// from A, i.e. beat dist[A].  The progress edges form a DAG (each step
// strictly decreases dist), and the answer is the number of DAG paths from
// the office (node 1) to the house.
//
// The DP runs over the nodes reachable from node 1 in that DAG, taken in
// increasing dist order, so every successor of a node is already final when
// the node is processed.  Restricting to reachable nodes also keeps every
// intermediate count bounded by the answer, which the statement caps at
// 2147483647; counts are kept in long long and printed through iostream so
// no 64-bit printf conversion is involved.
//
// Note a route can never get stuck short of the house: any node with a
// finite dist other than node 2 has at least one progress successor - the
// first hop of its own shortest route home - so "paths ending at node 2"
// and "maximal paths" count the same thing here.
//
// Input framing: cases run until a line holding 0, and the loop also stops
// cleanly at EOF if that terminator is absent.
#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;

static const long long INF = 1LL << 62;
static const int NODE_CAP = 200000;   /* N <= 1000; guard against stray ids */

struct DistLess {
    const vector<long long> *d;
    DistLess(const vector<long long> *dd) : d(dd) {}
    bool operator()(int a, int b) const { return (*d)[a] < (*d)[b]; }
};

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int m;
        if (scanf("%d", &m) != 1) break;

        vector<int> ea, eb, ew;
        ea.reserve(m > 0 ? m : 1);
        eb.reserve(m > 0 ? m : 1);
        ew.reserve(m > 0 ? m : 1);
        int maxNode = n;
        for (int i = 0; i < m; ++i) {
            int a, b, d;
            if (scanf("%d%d%d", &a, &b, &d) != 3) break;
            if (a < 1 || b < 1 || a > NODE_CAP || b > NODE_CAP) continue;
            ea.push_back(a);
            eb.push_back(b);
            ew.push_back(d);
            if (a > maxNode) maxNode = a;
            if (b > maxNode) maxNode = b;
        }
        if (maxNode < 2) maxNode = 2;

        vector<vector<pair<int, int> > > adj(maxNode + 1);
        for (size_t i = 0; i < ea.size(); ++i) {
            adj[ea[i]].push_back(make_pair(eb[i], ew[i]));
            adj[eb[i]].push_back(make_pair(ea[i], ew[i]));
        }

        /* Dijkstra from the house (node 2): dist[v] is the length of the
           shortest possible route from v to home. */
        vector<long long> dist(maxNode + 1, INF);
        priority_queue<pair<long long, int>, vector<pair<long long, int> >,
                       greater<pair<long long, int> > > pq;
        dist[2] = 0;
        pq.push(make_pair(0LL, 2));
        while (!pq.empty()) {
            pair<long long, int> top = pq.top();
            pq.pop();
            int u = top.second;
            if (top.first != dist[u]) continue;
            for (size_t i = 0; i < adj[u].size(); ++i) {
                int v = adj[u][i].first;
                long long nd = dist[u] + (long long)adj[u][i].second;
                if (nd < dist[v]) {
                    dist[v] = nd;
                    pq.push(make_pair(nd, v));
                }
            }
        }

        if (dist[1] >= INF) { /* office cannot reach home at all */
            std::cout << 0 << "\n";
            continue;
        }

        /* Nodes reachable from the office along progress edges u->v
           (allowed iff dist[v] < dist[u]).  Restricting the DP to these
           keeps every intermediate count bounded by the final answer. */
        vector<char> reach(maxNode + 1, 0);
        vector<int> stk, nodes;
        reach[1] = 1;
        stk.push_back(1);
        while (!stk.empty()) {
            int u = stk.back();
            stk.pop_back();
            nodes.push_back(u);
            for (size_t i = 0; i < adj[u].size(); ++i) {
                int v = adj[u][i].first;
                if (dist[v] < dist[u] && !reach[v]) {
                    reach[v] = 1;
                    stk.push_back(v);
                }
            }
        }

        /* Process in increasing distance-to-home: every successor of u is
           strictly closer to home, so it is already final when u is done. */
        sort(nodes.begin(), nodes.end(), DistLess(&dist));
        vector<long long> cnt(maxNode + 1, 0);
        for (size_t i = 0; i < nodes.size(); ++i) {
            int u = nodes[i];
            if (u == 2) { cnt[u] = 1; continue; }
            long long c = 0;
            for (size_t j = 0; j < adj[u].size(); ++j) {
                int v = adj[u][j].first;
                if (dist[v] < dist[u]) c += cnt[v];
            }
            cnt[u] = c;
        }
        std::cout << cnt[1] << "\n";
    }
    return 0;
}
