// POJ 3554 - Almost the shortest route
// Model: claude-sonnet-5
//
// Two Dijkstra passes: d1[] = shortest distance from node 1 in the
// original directed graph, d2[] = shortest distance from every node to
// node N (computed as a single Dijkstra from N on the reverse graph).
// D = d1[N] is the true shortest 1->N distance. A directed edge (u,v,w)
// lies on some route from 1 to N whose length is at most D+K iff
// d1[u] + w + d2[v] <= D + K (both finite). This is a per-edge test
// on shortest-distance values only, so it is invariant to which
// particular shortest path Dijkstra happens to pick on ties.
// If node N is unreachable from node 1, D is undefined and the answer
// is empty (L = 0); the board confirms 0 is the expected output for
// the unreachable case.
// The board also confirms int is sufficient (max path length is
// bounded by (N-1)*10000 < 1e8, well under overflow even with K
// added), and that despite the declared M < 100,000,000, the actual
// edge counts in the real test data are far smaller (on the order of
// 10^5-10^6) - consistent with the 64MB memory limit, which could
// not hold anywhere near 1e8 edges. The code makes no assumption
// about M's magnitude beyond what memory allows: all edge and
// adjacency-list arrays are sized dynamically from the M actually
// read.
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

static const int INF = 0x3f3f3f3f;

struct Graph {
    int n;
    vector<int> head;
    vector<int> nxt, to, w;
    void init(int n_, int m_) {
        n = n_;
        head.assign(n + 1, -1);
        nxt.assign(m_, -1);
        to.assign(m_, 0);
        w.assign(m_, 0);
    }
    void addEdge(int idx, int u, int v, int ww) {
        to[idx] = v; w[idx] = ww; nxt[idx] = head[u]; head[u] = idx;
    }
};

void dijkstra(const Graph &g, int src, vector<int> &dist) {
    dist.assign(g.n + 1, INF);
    vector<bool> done(g.n + 1, false);
    priority_queue<pair<int,int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    dist[src] = 0;
    pq.push(make_pair(0, src));
    while (!pq.empty()) {
        pair<int,int> top = pq.top(); pq.pop();
        int d = top.first, u = top.second;
        if (done[u]) continue;
        done[u] = true;
        if (d > dist[u]) continue;
        for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
            int v = g.to[e];
            if (done[v]) continue;
            int nd = d + g.w[e];
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push(make_pair(nd, v));
            }
        }
    }
}

int main() {
    int n, m, k;
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;

    vector<int> eu(m), ev(m), ew(m);

    Graph fwd, rev;
    fwd.init(n, m);
    rev.init(n, m);

    for (int i = 0; i < m; ++i) {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        eu[i] = a; ev[i] = b; ew[i] = c;
        fwd.addEdge(i, a, b, c);
        rev.addEdge(i, b, a, c);
    }

    vector<int> d1, d2;
    dijkstra(fwd, 1, d1);
    dijkstra(rev, n, d2);

    if (d1[n] >= INF) {
        printf("0\n");
        return 0;
    }

    int limit = d1[n] + k;
    vector<int> result;
    for (int i = 0; i < m; ++i) {
        if (d1[eu[i]] >= INF || d2[ev[i]] >= INF) continue;
        int total = d1[eu[i]] + ew[i] + d2[ev[i]];
        if (total <= limit) {
            result.push_back(i + 1);
        }
    }

    printf("%d\n", (int)result.size());
    for (size_t i = 0; i < result.size(); ++i) {
        printf("%d\n", result[i]);
    }
    return 0;
}
