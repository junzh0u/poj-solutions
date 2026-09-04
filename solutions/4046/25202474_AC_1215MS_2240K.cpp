// POJ 4046 - Sightseeing
// Model: claude-sonnet-5
//
// n restaurants (price[i]), m roads (undirected, weight z). For each query
// (s,t) find a walk from s to t minimizing (sum of edge weights on the walk)
// + (max price among all restaurants visited, including s and t). Multiple
// test cases, terminated by "0 0".
//
// Standard technique: for every vertex v, treat it as the "peak" (the most
// expensive restaurant actually eaten at) and run a single-source Dijkstra
// from v that is only allowed to enter vertices u with price[u] <= price[v].
// dist_v[x] is then the shortest walk from v to x using only restaurants no
// pricier than v. For a query (s,t), dist_v[s] + dist_v[t] + price[v] is a
// valid candidate answer (path s -> v -> t through vertices <= price[v], and
// v itself is charged as the meal). Ranging v over all vertices and taking
// the minimum recovers the true optimum, because the actual optimal path's
// own maximum-price vertex is covered by v = that vertex.
//
// Complexity: O(n * (n + m) log n) per test case (n <= 1000, m <= 20000).
// Loops are ordered peak-outer / query-inner (per the discuss board, the
// reverse order caused TLE from cache behavior on some submissions).
//
// The statement's Description/Input/Output/Sample sections are blank on the
// live POJ page (confirmed back to a 2012 Wayback capture, and true for
// most of the neighboring problem ids 4040-4050 as well: this looks like a
// bulk-upload bug for a contest whose statements were only ever a PDF link).
// The problem's own page links to http://poj.org/ProblemDescriptions/jinghua.pdf
// (visible on problem 4044's page, which kept its link even though its own
// inline text was also stripped); that PDF's page 5 ("Problem C. Sightseeing")
// is the actual statement and sample used here, recovered via pdftotext/PyMuPDF
// word-position extraction since plain pdftotext -layout squashed the sample's
// columns together.
//
// Ambiguity settled by the sample: "pass none or some restaurants" means the
// walk may revisit vertices / take any walk, but since edge weights are all
// positive, the optimal choice is always a simple shortest-path style walk;
// the peak technique above handles this without needing to reason about
// revisits explicitly. Statement says to print a blank line after each test
// case; taken literally, including after the last one.
#include <cstdio>
#include <vector>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF = (ll)4e18;
const int MAXN = 1005;

int n, m;
struct Edge { int to; ll w; };
vector<Edge> adj[MAXN];
ll price_[MAXN];
ll dist_[MAXN];
bool visited_[MAXN];

int main() {
    while (scanf("%d %d", &n, &m) == 2 && (n != 0 || m != 0)) {
        for (int i = 1; i <= n; i++) adj[i].clear();
        for (int i = 1; i <= n; i++) scanf("%lld", &price_[i]);
        for (int i = 0; i < m; i++) {
            int x, y; ll z;
            scanf("%d %d %lld", &x, &y, &z);
            Edge e1; e1.to = y; e1.w = z; adj[x].push_back(e1);
            Edge e2; e2.to = x; e2.w = z; adj[y].push_back(e2);
        }
        int Q;
        scanf("%d", &Q);
        vector<int> qs(Q), qt(Q);
        vector<ll> ans(Q, INF);
        for (int i = 0; i < Q; i++) scanf("%d %d", &qs[i], &qt[i]);

        for (int v = 1; v <= n; v++) {
            ll P = price_[v];
            for (int i = 1; i <= n; i++) { dist_[i] = INF; visited_[i] = false; }
            dist_[v] = 0;
            priority_queue<pair<ll,int>, vector<pair<ll,int> >, greater<pair<ll,int> > > pq;
            pq.push(make_pair((ll)0, v));
            while (!pq.empty()) {
                pair<ll,int> cur = pq.top(); pq.pop();
                int u = cur.second;
                if (visited_[u]) continue;
                visited_[u] = true;
                ll d = cur.first;
                for (size_t k = 0; k < adj[u].size(); k++) {
                    int to = adj[u][k].to;
                    if (price_[to] > P) continue;
                    ll nd = d + adj[u][k].w;
                    if (nd < dist_[to]) {
                        dist_[to] = nd;
                        pq.push(make_pair(nd, to));
                    }
                }
            }
            for (int i = 0; i < Q; i++) {
                int s = qs[i], t = qt[i];
                if (dist_[s] < INF && dist_[t] < INF) {
                    ll cand = dist_[s] + dist_[t] + P;
                    if (cand < ans[i]) ans[i] = cand;
                }
            }
        }

        for (int i = 0; i < Q; i++) {
            if (ans[i] >= INF) printf("-1\n");
            else printf("%lld\n", ans[i]);
        }
        printf("\n");
    }
    return 0;
}
