// POJ 3169 - Layout
//
// N cows stand on a number line in index order (cow i's coordinate x_i is
// implicit but must satisfy x_1 <= x_2 <= ... <= x_N -- this ordering
// constraint is never stated as a "distance" bound in the input but is
// required to interpret every "like"/"dislike" bound as a plain difference
// rather than an absolute value). ML "like" pairs (A,B,D) with A<B require
// x_B - x_A <= D; MD "dislike" pairs require x_B - x_A >= D. We want to
// maximize x_N - x_1.
//
// This is a classic system-of-difference-constraints problem. Each bound
// x_j - x_i <= w becomes a directed edge i -> j with weight w (the ordering
// gives edges i+1 -> i with weight 0, and each dislike bound x_B - x_A >= D
// becomes x_A - x_B <= -D, i.e. edge B -> A weight -D). The tightest upper
// bound on x_N - x_1 is then the shortest path from node 1 to node N.
//
// Three outputs, matching three states of that shortest-path graph:
//   - a negative cycle exists ANYWHERE in the graph -> the whole constraint
//     set is self-contradictory -> -1. Note this must be checked globally,
//     not just among nodes reachable from cow 1: a contradiction confined
//     to, say, cows 3 and 4 still makes the entire layout impossible even
//     if cow 1 can't otherwise reach that part of the graph. We check this
//     with SPFA from a virtual source node 0 wired to every real node with
//     weight-0 edges, so it reaches (and can find a cycle in) the whole
//     graph regardless of cow 1's own connectivity.
//   - node N is unreachable from node 1 (once global feasibility is
//     confirmed) -> nothing bounds x_N - x_1 from above -> -2.
//   - otherwise the shortest path distance from 1 to N is the answer.
//
// Distances are computed in a 64-bit type: while a negative cycle is being
// detected, SPFA can relax a node's distance many times before the
// per-node relax-count cap (> N) trips, and with D up to 1e6 and N up to
// 1000 that intermediate value can overflow a 32-bit int (observed via
// UBSan on an adversarial stress case). The final legitimate answer is
// always safely within int range, so it is narrowed only for printing.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;

const int MAXN = 1005;
const int MAXM = 25000;
// A negative cycle can be relaxed many times before the cnt_ check catches
// it, and each relaxation can move a distance by up to 1e6, so distances
// must be computed in a wider type than int to avoid signed overflow (UB)
// while a cycle is still being detected. The real (non-cycle) answer is
// always well within int range, so the final result is safely narrowed.
typedef long long ll;
const ll INF = (ll)4e18;

struct Edge { int to, w, next; };
Edge edges[MAXM];
int head[MAXN], ecnt;
ll dist_[MAXN];
int cnt_[MAXN];
bool inq[MAXN];
int N, ML, MD;

void addEdge(int u, int v, int w) {
    edges[ecnt].to = v;
    edges[ecnt].w = w;
    edges[ecnt].next = head[u];
    head[u] = ecnt++;
}

// Runs SPFA from `src` over nodes 0..N (0 may be a virtual source with 0-weight
// edges to every real node, used only for global negative-cycle detection).
// Returns true if a negative cycle reachable from src was found.
bool spfa(int src, int nodeCount) {
    for (int i = 0; i <= N; i++) { dist_[i] = INF; cnt_[i] = 0; inq[i] = false; }
    dist_[src] = 0;
    queue<int> q;
    q.push(src); inq[src] = true; cnt_[src] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop(); inq[u] = false;
        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            if (dist_[u] != INF && dist_[u] + edges[e].w < dist_[v]) {
                dist_[v] = dist_[u] + edges[e].w;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = true;
                    cnt_[v]++;
                    if (cnt_[v] > nodeCount) return true;
                }
            }
        }
    }
    return false;
}

int main() {
    scanf("%d %d %d", &N, &ML, &MD);
    memset(head, -1, sizeof(head));
    ecnt = 0;
    for (int i = 1; i < N; i++) {
        addEdge(i + 1, i, 0);
    }
    for (int i = 0; i < ML; i++) {
        int a, b, d;
        scanf("%d %d %d", &a, &b, &d);
        addEdge(a, b, d);
    }
    for (int i = 0; i < MD; i++) {
        int a, b, d;
        scanf("%d %d %d", &a, &b, &d);
        addEdge(b, a, -d);
    }

    // Global feasibility: virtual node 0 connects to every real node with
    // weight 0, so SPFA from 0 reaches the whole graph and any negative
    // cycle anywhere (not just ones reachable from cow 1) is detected.
    for (int i = 1; i <= N; i++) addEdge(0, i, 0);
    bool negCycle = spfa(0, N + 1);

    if (negCycle) {
        printf("-1\n");
        return 0;
    }

    // Distance from cow 1 specifically (node 0's edges don't affect this,
    // since we start the search at node 1).
    bool neg2 = spfa(1, N + 1);
    (void) neg2; // already proven acyclic globally; cannot trigger here
    if (dist_[N] == INF) printf("-2\n");
    else printf("%d\n", (int)dist_[N]);
    return 0;
}
