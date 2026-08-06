// POJ 3463 - Sightseeing
//
// Count the routes from S to F that are of minimal length, plus those exactly
// one distance unit longer.  Roads are directed, may run in parallel, and every
// length is at least 1.
//
// Dijkstra over states (vertex, k) where k = 0 is the shortest distance to that
// vertex and k = 1 the next *distinct* distance ("almost shortest"); each state
// carries both a distance and a count of routes realising it.  Relaxing an edge
// into u with the candidate distance nd has four cases, and getting only the
// first one right is the usual failure:
//   nd <  dist[u][0]  the old best demotes into the almost-shortest slot
//                     (distance and count together), then nd becomes the best
//   nd == dist[u][0]  a tie: add the counts rather than replacing
//   nd <  dist[u][1]  a new almost-shortest value
//   nd == dist[u][1]  a tie in the second slot: add the counts
// Because all lengths are >= 1, no state can be improved or tied after it is
// popped, so the usual Dijkstra finality argument still holds for both slots
// and the counts are complete when a state is settled.
//
// The answer is cnt[F][0], plus cnt[F][1] only when dist[F][1] is exactly
// dist[F][0] + 1 -- the second-best distance is generally further away than
// one unit, and then it contributes nothing.
//
// Statement notes: "at most 10^9" arrives as "109" in the extracted text, so
// the answer fits in 32 bits and can be printed with %u -- no long long, which
// keeps this strictly C++98 and sidesteps POJ's %lld quirk.  Parallel edges are
// explicitly allowed and count as distinct routes, which an edge-list
// adjacency handles for free.  Routes are walks in principle, but a repeated
// vertex would need a cycle of length >= 2 (A != B forbids self-loops), so
// nothing within shortest+1 ever revisits a vertex.
//
#include <cstdio>
#include <queue>

using namespace std;

const int MAXN = 1005;
const int MAXM = 10005;
const int INF = 0x3f3f3f3f;

int head[MAXN], nxt[MAXM], to_[MAXM], wt[MAXM];
int ecnt;

int dist_[MAXN][2];
unsigned int cnt_[MAXN][2];
bool done_[MAXN][2];

struct Node {
    int d, v, k;
    Node(int dd, int vv, int kk) : d(dd), v(vv), k(kk) {}
    bool operator<(const Node& o) const { return d > o.d; }  /* min-heap */
};

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n, m;
        scanf("%d %d", &n, &m);
        ecnt = 0;
        for (int i = 1; i <= n; ++i) head[i] = -1;
        for (int i = 0; i < m; ++i) {
            int a, b, l;
            scanf("%d %d %d", &a, &b, &l);
            to_[ecnt] = b; wt[ecnt] = l; nxt[ecnt] = head[a]; head[a] = ecnt; ++ecnt;
        }
        int s, f;
        scanf("%d %d", &s, &f);

        for (int i = 1; i <= n; ++i) {
            dist_[i][0] = dist_[i][1] = INF;
            cnt_[i][0] = cnt_[i][1] = 0u;
            done_[i][0] = done_[i][1] = false;
        }
        priority_queue<Node> pq;
        dist_[s][0] = 0; cnt_[s][0] = 1u;
        pq.push(Node(0, s, 0));

        while (!pq.empty()) {
            Node cur = pq.top(); pq.pop();
            int v = cur.v, k = cur.k;
            if (done_[v][k]) continue;
            if (cur.d != dist_[v][k]) continue;
            done_[v][k] = true;
            unsigned int c = cnt_[v][k];
            for (int e = head[v]; e != -1; e = nxt[e]) {
                int u = to_[e];
                int nd = cur.d + wt[e];
                if (nd < dist_[u][0]) {
                    /* the old best demotes into the almost-shortest slot */
                    dist_[u][1] = dist_[u][0];
                    cnt_[u][1] = cnt_[u][0];
                    dist_[u][0] = nd;
                    cnt_[u][0] = c;
                    pq.push(Node(dist_[u][0], u, 0));
                    if (dist_[u][1] < INF) pq.push(Node(dist_[u][1], u, 1));
                } else if (nd == dist_[u][0]) {
                    cnt_[u][0] += c;
                } else if (nd < dist_[u][1]) {
                    dist_[u][1] = nd;
                    cnt_[u][1] = c;
                    pq.push(Node(nd, u, 1));
                } else if (nd == dist_[u][1]) {
                    cnt_[u][1] += c;
                }
            }
        }

        unsigned int ans = cnt_[f][0];
        if (dist_[f][0] < INF && dist_[f][1] == dist_[f][0] + 1) ans += cnt_[f][1];
        printf("%u\n", ans);
    }
    return 0;
}
