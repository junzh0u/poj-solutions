// POJ 2396 - Budget
// Model: gpt-5.6-terra
// Intersect each cell's integer lower/upper bounds, subtract lower bounds from
// the row and column totals, then find the remaining transportation flow.
// Strict constraints shift an integer bound by one; 0 in either coordinate
// applies a constraint to every row or column in that dimension.
#include <cstdio>
#include <cstring>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef long long ll;

struct Edge {
    int to, rev;
    ll cap;
    Edge(int t, int r, ll c) : to(t), rev(r), cap(c) {}
};

struct Dinic {
    vector<vector<Edge> > g;
    vector<int> level, work;

    Dinic(int n) : g(n), level(n), work(n) {}

    void addEdge(int u, int v, ll cap) {
        g[u].push_back(Edge(v, (int)g[v].size(), cap));
        g[v].push_back(Edge(u, (int)g[u].size() - 1, 0));
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < (int)g[u].size(); ++i) {
                Edge &e = g[u][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    ll dfs(int u, int t, ll f) {
        if (u == t) return f;
        for (int &i = work[u]; i < (int)g[u].size(); ++i) {
            Edge &e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                ll got = dfs(e.to, t, min(f, e.cap));
                if (got > 0) {
                    e.cap -= got;
                    g[e.to][e.rev].cap += got;
                    return got;
                }
            }
        }
        return 0;
    }

    ll maxFlow(int s, int t) {
        const ll INF = (1LL << 60);
        ll result = 0;
        while (bfs(s, t)) {
            fill(work.begin(), work.end(), 0);
            ll pushed;
            while ((pushed = dfs(s, t, INF)) > 0) result += pushed;
        }
        return result;
    }
};

int main() {
    const ll INF = (1LL << 50);
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    for (int tc = 0; tc < cases; ++tc) {
        int m, n;
        scanf("%d%d", &m, &n);
        vector<ll> row(m), col(n);
        for (int i = 0; i < m; ++i) scanf("%lld", &row[i]);
        for (int j = 0; j < n; ++j) scanf("%lld", &col[j]);

        vector<vector<ll> > low(m, vector<ll>(n, 0));
        vector<vector<ll> > high(m, vector<ll>(n, INF));
        int c;
        scanf("%d", &c);
        for (int k = 0; k < c; ++k) {
            int r, q;
            char op[2];
            ll v;
            scanf("%d%d%s%lld", &r, &q, op, &v);
            int rb = r == 0 ? 0 : r - 1;
            int re = r == 0 ? m : r;
            int cb = q == 0 ? 0 : q - 1;
            int ce = q == 0 ? n : q;
            for (int i = rb; i < re; ++i) {
                for (int j = cb; j < ce; ++j) {
                    if (op[0] == '=') {
                        low[i][j] = max(low[i][j], v);
                        high[i][j] = min(high[i][j], v);
                    } else if (op[0] == '>') {
                        low[i][j] = max(low[i][j], v + 1);
                    } else {
                        high[i][j] = min(high[i][j], v - 1);
                    }
                }
            }
        }

        bool possible = true;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (low[i][j] > high[i][j]) possible = false;
                row[i] -= low[i][j];
                col[j] -= low[i][j];
            }
        }
        ll need = 0, columns = 0;
        for (int i = 0; i < m; ++i) {
            if (row[i] < 0) possible = false;
            need += row[i];
        }
        for (int j = 0; j < n; ++j) {
            if (col[j] < 0) possible = false;
            columns += col[j];
        }
        if (need != columns) possible = false;

        vector<vector<ll> > answer = low;
        if (possible) {
            int source = m + n;
            int sink = source + 1;
            Dinic flow(sink + 1);
            for (int i = 0; i < m; ++i) flow.addEdge(source, i, row[i]);
            for (int j = 0; j < n; ++j) flow.addEdge(m + j, sink, col[j]);
            vector<vector<int> > edgeAt(m, vector<int>(n));
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    edgeAt[i][j] = (int)flow.g[i].size();
                    flow.addEdge(i, m + j, high[i][j] - low[i][j]);
                }
            }
            if (flow.maxFlow(source, sink) != need) {
                possible = false;
            } else {
                for (int i = 0; i < m; ++i) {
                    for (int j = 0; j < n; ++j) {
                        Edge &e = flow.g[i][edgeAt[i][j]];
                        answer[i][j] += flow.g[e.to][e.rev].cap;
                    }
                }
            }
        }

        if (tc) printf("\n");
        if (!possible) {
            printf("IMPOSSIBLE\n");
        } else {
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (j) printf(" ");
                    printf("%lld", answer[i][j]);
                }
                printf("\n");
            }
        }
    }
    return 0;
}
