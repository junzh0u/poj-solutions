// POJ 3635 - Full Tank?
//
// Cheapest s->e trip for a car of tank capacity c, starting empty, buying fuel
// at each city's own price and burning one unit per unit of road length.
//
// The state is (city, fuel in tank), not city alone: 1000 * 101 states.  Two
// kinds of moves leave a state -- buy one unit of fuel at the current city
// (cost = that city's price, fuel + 1, only while the tank is not full), and
// drive a road of length L (cost 0, fuel - L, only while L <= fuel).  Dijkstra
// over that product graph gives the answer; buying a single unit at a time
// keeps the out-degree tiny and is what makes the greedy correct, since any
// purchase plan decomposes into unit purchases.  The search stops the moment
// any state of the goal city is settled -- fuel left over at the end is free.
//
// Reachability is settled before the search: with capacity c every road of
// length <= c is drivable (fuel can always be bought first) and every longer
// road is unusable, so the goal is reachable exactly when it is connected to s
// in the subgraph of short-enough roads.  One plain Dijkstra from the goal over
// that subgraph answers this and doubles as an A* potential: from (u, f) at
// least D(u) - f more units must still be bought and none costs less than the
// cheapest price in the component, an estimate that is consistent, so states
// still settle once.  Cities from which the goal cannot be reached are dropped
// outright.  Adjacency is stored sorted by road length, so an arc scan can stop
// at the first road longer than the fuel on board.
//
// The q queries share one road network, so the graph is read once; identical
// (capacity, s, e) triples reuse an earlier answer.
//
// Statement notes: the roads are undirected and the cities are 0-indexed.  The
// statement never says the tank must end empty or full, so leftover fuel at the
// goal simply goes unused, and s == e costs 0.  The sample's second query is
// "impossible" because its goal city 4 has no roads at all.
#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

static const int MAXN = 1005;
static const int MAXM = 10005;
static const int INF = 0x3f3f3f3f;

typedef unsigned long long ull;

int n, m, q;
int price[MAXN];

/* CSR adjacency; the arcs of a city are sorted by road length ascending, so a
   scan can stop at the first road too long for the fuel in the tank. */
int adjStart[MAXN + 1];
int adjArc[2 * MAXM];            /* length << 16 | destination */

int eu[MAXM], ev[MAXM], ed[MAXM];

int dist_[MAXN * 101];
int D[MAXN];                     /* road distance to the goal, roads of length <= c only */
int minP;                        /* cheapest fuel among the cities that can still reach the goal */

/* plain Dijkstra from the goal, over the roads a tank of capacity c can drive */
void goalDistances(int c, int e) {
    for (int i = 0; i < n; i++) D[i] = INF;
    priority_queue<ull, vector<ull>, greater<ull> > pq;
    D[e] = 0;
    pq.push((ull)e);
    while (!pq.empty()) {
        ull top = pq.top(); pq.pop();
        int u = (int)(top & 0x3FFu);
        int d = (int)(top >> 10);
        if (d != D[u]) continue;
        int en = adjStart[u + 1];
        for (int i = adjStart[u]; i < en; i++) {
            int arc = adjArc[i], len = arc >> 16;
            if (len > c) break;
            int v = arc & 0xFFFF, nd = d + len;
            if (nd < D[v]) { D[v] = nd; pq.push(((ull)nd << 10) | (ull)v); }
        }
    }
    minP = 101;
    for (int i = 0; i < n; i++) if (D[i] < INF && price[i] < minP) minP = price[i];
}

int solve(int c, int s, int e) {
    /* Every road of length <= c can be driven (fuel is always purchasable), so
       the goal is reachable exactly when it is connected to s in that subgraph. */
    if (D[s] >= INF) return -1;

    int W = c + 1;
    int total = n * W;
    for (int i = 0; i < total; i++) dist_[i] = INF;
    priority_queue<ull, vector<ull>, greater<ull> > pq;
    dist_[s * W] = 0;
    /* A*: at least D(u)-f more units must still be bought and none costs less
       than minP, an estimate that is consistent, so states settle once. */
    pq.push(((ull)(D[s] * minP) << 18) | (ull)(s * W));
    while (!pq.empty()) {
        ull top = pq.top();
        pq.pop();
        int id = (int)(top & 0x3FFFFu);
        int key = (int)(top >> 18);
        int u = id / W, f = id - u * W;
        int rem = D[u] - f;
        int g = key - (rem > 0 ? rem * minP : 0);
        if (g != dist_[id]) continue;
        if (u == e) return g;
        if (f < c) {                                   /* buy one unit of fuel */
            int nd = g + price[u];
            if (nd < dist_[id + 1]) {
                dist_[id + 1] = nd;
                int r = rem - 1;
                pq.push(((ull)(nd + (r > 0 ? r * minP : 0)) << 18) | (ull)(id + 1));
            }
        }
        int en = adjStart[u + 1];
        for (int i = adjStart[u]; i < en; i++) {       /* drive a road */
            int arc = adjArc[i], len = arc >> 16;
            if (len > f) break;
            int v = arc & 0xFFFF;
            if (D[v] >= INF) continue;                 /* dead end: goal unreachable from v */
            int nid = v * W + (f - len);
            if (g < dist_[nid]) {
                dist_[nid] = g;
                int r = D[v] - (f - len);
                pq.push(((ull)(g + (r > 0 ? r * minP : 0)) << 18) | (ull)nid);
            }
        }
    }
    return -1;
}

int main() {
    if (scanf("%d %d", &n, &m) != 2) return 0;
    for (int i = 0; i < n; i++) scanf("%d", &price[i]);
    for (int i = 0; i < m; i++) scanf("%d %d %d", &eu[i], &ev[i], &ed[i]);

    /* build the CSR, inserting the arcs length by length so each list is sorted */
    static int deg[MAXN + 1];
    for (int i = 0; i <= n; i++) deg[i] = 0;
    for (int i = 0; i < m; i++) { deg[eu[i]]++; deg[ev[i]]++; }
    adjStart[0] = 0;
    for (int i = 0; i < n; i++) adjStart[i + 1] = adjStart[i] + deg[i];
    static int pos[MAXN];
    for (int i = 0; i < n; i++) pos[i] = adjStart[i];
    static vector<int> byLen[101];
    for (int i = 0; i < m; i++) byLen[ed[i]].push_back(i);
    for (int L = 1; L <= 100; L++) {
        for (size_t k = 0; k < byLen[L].size(); k++) {
            int i = byLen[L][k];
            adjArc[pos[eu[i]]++] = (L << 16) | ev[i];
            adjArc[pos[ev[i]]++] = (L << 16) | eu[i];
        }
    }

    if (scanf("%d", &q) != 1) return 0;
    vector<int> qc(q), qs(q), qe(q), ans(q, -2);
    for (int i = 0; i < q; i++) scanf("%d %d %d", &qc[i], &qs[i], &qe[i]);
    int lastC = -1, lastE = -1;
    for (int i = 0; i < q; i++) {
        if (ans[i] != -2) continue;
        if (qc[i] != lastC || qe[i] != lastE) {
            goalDistances(qc[i], qe[i]); lastC = qc[i]; lastE = qe[i];
        }
        ans[i] = solve(qc[i], qs[i], qe[i]);
        for (int j = i + 1; j < q; j++)       /* identical queries come for free */
            if (qc[j] == qc[i] && qs[j] == qs[i] && qe[j] == qe[i]) ans[j] = ans[i];
    }
    for (int i = 0; i < q; i++) {
        if (ans[i] < 0) printf("impossible\n");
        else printf("%d\n", ans[i]);
    }
    return 0;
}
