// POJ 2289 - Jamie's Contact Groups
// Model: claude-sonnet-5
// Approach: binary search on the answer K (max group size). Feasibility test
// for a given K is a bipartite b-matching: source -> friend (cap 1) ->
// allowed groups (cap 1 each) -> sink, with group -> sink capacity K.
// Feasible iff max flow == N (Dinic). Lower bound for K is ceil(N/M) since
// M groups of capacity K can hold at most M*K friends.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

static const int MAXNODE = 1000 + 500 + 5;

struct Edge { int to, cap, rev; };

vector<Edge> graph[MAXNODE];
int level_[MAXNODE], iter_[MAXNODE];
int nNodes;

void add_edge(int from, int to, int cap) {
    Edge a = { to, cap, (int)graph[to].size() };
    Edge b = { from, 0, (int)graph[from].size() };
    graph[from].push_back(a);
    graph[to].push_back(b);
}

bool bfs(int s, int t) {
    for (int i = 0; i < nNodes; ++i) level_[i] = -1;
    queue<int> q;
    level_[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (size_t i = 0; i < graph[v].size(); ++i) {
            Edge &e = graph[v][i];
            if (e.cap > 0 && level_[e.to] < 0) {
                level_[e.to] = level_[v] + 1;
                q.push(e.to);
            }
        }
    }
    return level_[t] >= 0;
}

int dfs(int v, int t, int f) {
    if (v == t) return f;
    for (int &i = iter_[v]; i < (int)graph[v].size(); ++i) {
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
    while (bfs(s, t)) {
        for (int i = 0; i < nNodes; ++i) iter_[i] = 0;
        int f;
        while ((f = dfs(s, t, 1 << 30)) > 0) flow += f;
    }
    return flow;
}

int N, M;
vector<vector<int> > friendGroups;

bool feasible(int K) {
    int source = 0, sink = N + M + 1;
    nNodes = N + M + 2;
    for (int i = 0; i < nNodes; ++i) graph[i].clear();

    for (int i = 0; i < N; ++i) {
        add_edge(source, i + 1, 1);
        for (size_t k = 0; k < friendGroups[i].size(); ++k) {
            int g = friendGroups[i][k];
            add_edge(i + 1, N + 1 + g, 1);
        }
    }
    for (int j = 0; j < M; ++j) {
        add_edge(N + 1 + j, sink, K);
    }
    int f = max_flow(source, sink);
    return f == N;
}

int main() {
    char linebuf[1 << 16];
    while (fgets(linebuf, sizeof(linebuf), stdin)) {
        if (sscanf(linebuf, "%d %d", &N, &M) != 2) {
            // skip blank lines
            continue;
        }
        if (N == 0 && M == 0) break;

        friendGroups.assign(N, vector<int>());
        int read = 0;
        while (read < N) {
            if (!fgets(linebuf, sizeof(linebuf), stdin)) break;
            // skip entirely blank lines (shouldn't normally happen)
            {
                bool blank = true;
                for (char *p = linebuf; *p; ++p) {
                    if (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') { blank = false; break; }
                }
                if (blank) continue;
            }
            char name[64];
            int pos = 0;
            sscanf(linebuf, "%63s%n", name, &pos);
            vector<int> groups;
            int off = pos;
            int val, consumed;
            while (sscanf(linebuf + off, "%d%n", &val, &consumed) == 1) {
                groups.push_back(val);
                off += consumed;
            }
            friendGroups[read] = groups;
            ++read;
        }

        int lo = (N + M - 1) / M;
        if (lo < 1) lo = 1;
        int hi = N;
        int ans = N;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (feasible(mid)) {
                ans = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
