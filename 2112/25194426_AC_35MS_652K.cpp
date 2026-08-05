// POJ 2112 - Optimal Milking
//
// K milking machines (ids 1..K) and C cows (ids K+1..K+C) sit in a weighted
// graph given as a symmetric adjacency matrix; each machine handles at most M
// cows. Minimise the distance walked by the furthest-walking cow.
//
// Approach: minimax over an assignment => binary search the answer, with a
// max-flow feasibility test.
//   1. The matrix gives direct paths only and does NOT satisfy the triangle
//      inequality ("cows can traverse several paths"), so first run
//      Floyd-Warshall over all K+C <= 230 entities. A 0 off the diagonal means
//      "no path", not "distance 0", so it must be read as infinity before the
//      closure; the 0s on the diagonal are genuine.
//   2. Collect the distinct cow->machine shortest distances and binary search
//      over them. For a limit L build source -> machine (cap M) -> cow (cap 1,
//      only where dist <= L) -> sink (cap 1); L works iff the max flow
//      saturates all C cows. Capacity M is why this is a flow problem rather
//      than plain bipartite matching.
// Dinic on ~230 nodes / ~6000 unit-capacity edges, ~13 binary-search steps.
//
// Ambiguity worth noting: the statement's "distance of 0 means not directly
// connected" collides with the 0s on the diagonal, and the output asks for the
// "total distance" of the furthest walker, which is just its walked distance.
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXE = 235;              // K + C
const int DINF = 1000000;
const int INF = 1 << 29;

int K, C, M, N;
int d[MAXE][MAXE];

// ---- Dinic ----
const int MAXV = 240;
struct Edge {
    int to, cap, rev;
    Edge(int t, int c, int r) : to(t), cap(c), rev(r) {}
};
vector<Edge> gph[MAXV];
int level_[MAXV], iter_[MAXV];
int V;

void add_edge(int from, int to, int cap) {
    gph[from].push_back(Edge(to, cap, (int)gph[to].size()));
    gph[to].push_back(Edge(from, 0, (int)gph[from].size() - 1));
}

void bfs(int s) {
    memset(level_, -1, sizeof(int) * V);
    queue<int> q;
    level_[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (size_t i = 0; i < gph[v].size(); i++) {
            Edge &e = gph[v][i];
            if (e.cap > 0 && level_[e.to] < 0) {
                level_[e.to] = level_[v] + 1;
                q.push(e.to);
            }
        }
    }
}

int dfs(int v, int t, int f) {
    if (v == t) return f;
    for (int &i = iter_[v]; i < (int)gph[v].size(); i++) {
        Edge &e = gph[v][i];
        if (e.cap > 0 && level_[v] < level_[e.to]) {
            int dd = dfs(e.to, t, min(f, e.cap));
            if (dd > 0) {
                e.cap -= dd;
                gph[e.to][e.rev].cap += dd;
                return dd;
            }
        }
    }
    return 0;
}

int max_flow(int s, int t) {
    int flow = 0;
    for (;;) {
        bfs(s);
        if (level_[t] < 0) return flow;
        memset(iter_, 0, sizeof(int) * V);
        int f;
        while ((f = dfs(s, t, INF)) > 0) flow += f;
    }
}

// feasible if every cow can be assigned a machine within distance lim
bool feasible(int lim) {
    // nodes: 0 = source, 1..K machines, K+1..K+C cows, K+C+1 = sink
    V = K + C + 2;
    int source = 0, sink = K + C + 1;
    for (int i = 0; i < V; i++) gph[i].clear();
    for (int i = 1; i <= K; i++) add_edge(source, i, M);
    for (int j = 1; j <= C; j++) add_edge(K + j, sink, 1);
    for (int j = 1; j <= C; j++)
        for (int i = 1; i <= K; i++)
            if (d[K + j - 1][i - 1] <= lim) add_edge(i, K + j, 1);
    return max_flow(source, sink) == C;
}

int main() {
    while (scanf("%d %d %d", &K, &C, &M) == 3) {
        N = K + C;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                int x;
                if (scanf("%d", &x) != 1) x = 0;
                d[i][j] = (i == j) ? 0 : (x == 0 ? DINF : x);
            }
        // all-pairs shortest paths
        for (int k = 0; k < N; k++)
            for (int i = 0; i < N; i++) {
                if (d[i][k] == DINF) continue;
                int dik = d[i][k];
                for (int j = 0; j < N; j++)
                    if (dik + d[k][j] < d[i][j]) d[i][j] = dik + d[k][j];
            }
        // candidate distances: cow -> machine
        vector<int> cand;
        for (int j = 0; j < C; j++)
            for (int i = 0; i < K; i++)
                if (d[K + j][i] < DINF) cand.push_back(d[K + j][i]);
        sort(cand.begin(), cand.end());
        cand.erase(unique(cand.begin(), cand.end()), cand.end());

        int lo = 0, hi = (int)cand.size() - 1, ans = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (feasible(cand[mid])) { ans = cand[mid]; hi = mid - 1; }
            else lo = mid + 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
