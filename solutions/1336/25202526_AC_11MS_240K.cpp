// POJ 1336 - The K-League
// Model: claude-sonnet-5
// Approach: classic baseball-elimination-style max-flow. For each team S,
// let max = w_S + (remaining games of S). Since S can always win every one
// of its own remaining games without ever exceeding max, the optimal
// strategy assumes S wins all its own games; that fixes the outcome of
// every game involving S and removes S from further consideration. Then
// build a flow network over the remaining games among the OTHER n-1 teams:
// source -> game-node(i,j) with capacity a[i][j] (i<j, i,j != S),
// game-node(i,j) -> team i and team j with infinite capacity,
// team i -> sink with capacity (max - w_i) for i != S (if this is negative,
// S is eliminated immediately). S can still win the championship iff every
// cap is nonnegative and max-flow saturates all remaining games among the
// other teams (i.e. equals their total count).
// Ambiguity check: verified against a brute-force search over small random
// instances (exhaustively assigning winners to every remaining game,
// including S's own) that the "S wins all its own games" reduction gives
// the same feasibility answer as full brute force.
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

const int MAXN = 27;
const int INF = 1 << 29;

struct Dinic {
    struct Edge { int to, cap, rev; };
    vector<Edge> graph[MAXN * MAXN / 2 + MAXN + 5];
    int level[MAXN * MAXN / 2 + MAXN + 5];
    int iter[MAXN * MAXN / 2 + MAXN + 5];
    int n;

    void init(int n_) {
        n = n_;
        for (int i = 0; i < n; i++) graph[i].clear();
    }
    void add_edge(int from, int to, int cap) {
        Edge a = {to, cap, (int)graph[to].size()};
        Edge b = {from, 0, (int)graph[from].size()};
        graph[from].push_back(a);
        graph[to].push_back(b);
    }
    bool bfs(int s, int t) {
        memset(level, -1, sizeof(int) * n);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (size_t i = 0; i < graph[v].size(); i++) {
                Edge &e = graph[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }
    int dfs(int v, int t, int f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < (int)graph[v].size(); i++) {
            Edge &e = graph[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    int max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            memset(iter, 0, sizeof(int) * n);
            int f;
            while ((f = dfs(s, t, INF)) > 0) flow += f;
        }
        return flow;
    }
};

static Dinic dinic;

int n;
int w[MAXN], d[MAXN];
int a[MAXN][MAXN];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++) scanf("%d %d", &w[i], &d[i]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                scanf("%d", &a[i][j]);

        // pair index for i<j
        int pairId[MAXN][MAXN];
        memset(pairId, -1, sizeof(pairId));
        int npairs = 0;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                pairId[i][j] = npairs++;

        vector<int> winners;
        for (int S = 0; S < n; S++) {
            int rS = 0;
            for (int j = 0; j < n; j++) rS += a[S][j];
            int cap_max = w[S] + rS;

            bool eliminated = false;
            for (int i = 0; i < n && !eliminated; i++) {
                if (i == S) continue;
                if (w[i] > cap_max) eliminated = true;
            }

            int total = 0;
            if (!eliminated) {
                for (int i = 0; i < n; i++) {
                    if (i == S) continue;
                    for (int j = i + 1; j < n; j++) {
                        if (j == S) continue;
                        total += a[i][j];
                    }
                }
                if (total == 0) {
                    // trivially feasible
                } else {
                    // nodes: 0 = source, 1..n = teams, n+1..n+npairs = pair nodes,
                    // sink = n+npairs+1
                    int source = 0;
                    int sink = n + npairs + 1;
                    int totalNodes = sink + 1;
                    dinic.init(totalNodes);
                    for (int i = 0; i < n; i++) {
                        if (i == S) continue;
                        for (int j = i + 1; j < n; j++) {
                            if (j == S) continue;
                            if (a[i][j] <= 0) continue;
                            int pid = n + 1 + pairId[i][j];
                            dinic.add_edge(source, pid, a[i][j]);
                            dinic.add_edge(pid, 1 + i, INF);
                            dinic.add_edge(pid, 1 + j, INF);
                        }
                    }
                    for (int i = 0; i < n; i++) {
                        if (i == S) continue;
                        int capi = cap_max - w[i];
                        dinic.add_edge(1 + i, sink, capi);
                    }
                    int flow = dinic.max_flow(source, sink);
                    if (flow != total) eliminated = true;
                }
            }

            if (!eliminated) winners.push_back(S + 1);
        }

        for (size_t i = 0; i < winners.size(); i++) {
            if (i) printf(" ");
            printf("%d", winners[i]);
        }
        printf("\n");
    }
    return 0;
}
