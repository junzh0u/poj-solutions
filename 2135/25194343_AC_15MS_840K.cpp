// POJ 2135 - Farm Tour
//
// FJ walks from field 1 to field N and back, using each of the M
// undirected paths at most once, minimizing total length. This is two
// edge-disjoint trails between 1 and N with minimum combined length --
// NOT two independent shortest-path runs, since the optimal pair of
// trails may not include either endpoint's individual shortest path
// (they might need to share a prefix/suffix and diverge only in the
// middle to stay edge-disjoint).
//
// Model it as a min-cost flow of value 2 from node 1 to node N. Each
// undirected edge (u, v, w) becomes two independent capacity-1 arcs,
// u->v and v->u, each with its own capacity-0 reverse residual arc of
// cost -w. Because all original costs are nonnegative, a minimum-cost
// solution never uses both directions of the same edge at once (doing
// so could always be cancelled to strictly lower cost), so this
// correctly enforces "each edge used at most once" while still letting
// the two flow units share vertices freely.
//
// Successive shortest augmenting paths, found with SPFA (Bellman-Ford)
// since residual arcs can have negative cost. Only two units of flow
// are ever needed, so just two augmentations regardless of graph size.
//
// N <= 1000, M <= 10000 => at most 4*M ~= 40000 directed arcs; two SPFA
// passes over that is trivial well within the 1000ms limit.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MAXN = 1005;
const int MAXM = 40010;
const int INF = 0x3f3f3f3f;

int head[MAXN], nxt[MAXM], to[MAXM], cap[MAXM], cost[MAXM], ecnt;
int n, m;

void addEdge(int u, int v, int c, int w) {
    to[ecnt] = v; cap[ecnt] = c; cost[ecnt] = w; nxt[ecnt] = head[u]; head[u] = ecnt++;
    to[ecnt] = u; cap[ecnt] = 0; cost[ecnt] = -w; nxt[ecnt] = head[v]; head[v] = ecnt++;
}

int dist[MAXN], prevEdge[MAXN];
bool inq[MAXN];

bool spfa(int s, int t) {
    for (int i = 1; i <= n; i++) { dist[i] = INF; inq[i] = false; prevEdge[i] = -1; }
    dist[s] = 0;
    queue<int> q;
    q.push(s); inq[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop(); inq[u] = false;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (cap[e] > 0 && dist[u] + cost[e] < dist[to[e]]) {
                dist[to[e]] = dist[u] + cost[e];
                prevEdge[to[e]] = e;
                if (!inq[to[e]]) {
                    inq[to[e]] = true;
                    q.push(to[e]);
                }
            }
        }
    }
    return dist[t] < INF;
}

int main() {
    scanf("%d %d", &n, &m);
    memset(head, -1, sizeof(head));
    ecnt = 0;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        addEdge(u, v, 1, w);
        addEdge(v, u, 1, w);
    }
    int totalCost = 0;
    int flow = 0;
    while (flow < 2 && spfa(1, n)) {
        int aug = INF;
        int v = n;
        while (v != 1) {
            int e = prevEdge[v];
            if (cap[e] < aug) aug = cap[e];
            v = to[e ^ 1];
        }
        v = n;
        while (v != 1) {
            int e = prevEdge[v];
            cap[e] -= aug;
            cap[e ^ 1] += aug;
            v = to[e ^ 1];
        }
        totalCost += aug * dist[n];
        flow += aug;
    }
    printf("%d\n", totalCost);
    return 0;
}
