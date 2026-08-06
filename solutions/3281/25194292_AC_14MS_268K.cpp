// POJ 3281 - Dining
// Max flow with node splitting: source -> food(cap1) -> cow_in -> cow_out
// (split, cap1 enforces each cow used once) -> drink(cap1) -> sink.
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

const int MAXN = 500;
const int INF = 1 << 29;

struct Edge {
    int to, cap, rev;
    Edge(int t, int c, int r) : to(t), cap(c), rev(r) {}
};
vector<Edge> graph[MAXN];
int level_[MAXN];
int iter_[MAXN];
int V;

void add_edge(int from, int to, int cap) {
    graph[from].push_back(Edge(to, cap, (int)graph[to].size()));
    graph[to].push_back(Edge(from, 0, (int)graph[from].size() - 1));
}

void bfs(int s) {
    memset(level_, -1, sizeof(int) * V);
    queue<int> q;
    level_[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (size_t i = 0; i < graph[v].size(); i++) {
            Edge &e = graph[v][i];
            if (e.cap > 0 && level_[e.to] < 0) {
                level_[e.to] = level_[v] + 1;
                q.push(e.to);
            }
        }
    }
}

int dfs(int v, int t, int f) {
    if (v == t) return f;
    for (int &i = iter_[v]; i < (int)graph[v].size(); i++) {
        Edge &e = graph[v][i];
        if (e.cap > 0 && level_[v] < level_[e.to]) {
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
    while (true) {
        bfs(s);
        if (level_[t] < 0) return flow;
        memset(iter_, 0, sizeof(int) * V);
        int f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
}

int main() {
    int N, F, D;
    if (scanf("%d %d %d", &N, &F, &D) != 3) return 0;

    // node numbering
    int source = 0;
    int foodBase = 1;          // 1..F
    int cowInBase = 1 + F;     // F+1 .. F+N
    int cowOutBase = 1 + F + N; // F+N+1 .. F+2N
    int drinkBase = 1 + F + 2 * N; // F+2N+1 .. F+2N+D
    int sink = 1 + F + 2 * N + D;
    V = sink + 1;

    for (int i = 0; i < F; i++) add_edge(source, foodBase + i, 1);
    for (int i = 0; i < D; i++) add_edge(drinkBase + i, sink, 1);

    for (int j = 0; j < N; j++) {
        add_edge(cowInBase + j, cowOutBase + j, 1);
        int fi, di;
        scanf("%d %d", &fi, &di);
        for (int k = 0; k < fi; k++) {
            int f;
            scanf("%d", &f);
            add_edge(foodBase + f - 1, cowInBase + j, 1);
        }
        for (int k = 0; k < di; k++) {
            int d;
            scanf("%d", &d);
            add_edge(cowOutBase + j, drinkBase + d - 1, 1);
        }
    }

    printf("%d\n", max_flow(source, sink));
    return 0;
}
