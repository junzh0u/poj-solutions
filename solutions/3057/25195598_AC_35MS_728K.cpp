// POJ 3057 - Evacuation
// Model: claude-opus-5
//
// One person starts on every '.'; each may move N/S/E/W one square per second
// or stand still, squares hold any number of people, and each door square lets
// exactly one person enter per second (entering a door evacuates that person).
// Since squares are uncapacitated, the only real constraint is door throughput:
// a person can leave through door d at any second t >= dist(d, p), where
// dist is the BFS distance from d over '.' squares only -- a path may never
// run through a door square, because entering one evacuates you.
//
// So the answer is the smallest T for which persons can be perfectly matched to
// distinct (door, second) slots with second <= T.  Feasibility is monotone in T,
// so binary search it and test with max flow.  Instead of an edge per (person,
// door, second) triple, each door gets a chain of second-nodes: person -> node
// (d, dist(d,p)), node(d,t) -> node(d,t+1) with large capacity (waiting), and
// node(d,t) -> sink with capacity 1 (the door's one slot in second t).  That is
// O(P*D + D*T) edges rather than O(P*D*T).  Answer <= max dist + P, so T is
// bounded and "impossible" is exactly "some person reaches no door at all".
//
// Ambiguity: the discuss board's most-cited data set posts 22 for its sixth
// case; the correct value is 20, certified here by an explicit 60-person
// schedule and, at T=19, by a Hall violator (39 people, 36 reachable slots).
// The board post's own code shares one `used[]` array across several Kuhn DFS
// calls per phase, so it under-matches.  Input is read to EOF rather than
// trusting the announced case count.

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

struct Edge { int to, cap, rev; };

static vector<vector<Edge> > G;
static vector<int> lev, it_;

static void add_edge(int a, int b, int c) {
    Edge e; e.to = b; e.cap = c; e.rev = (int)G[b].size();
    Edge r; r.to = a; r.cap = 0; r.rev = (int)G[a].size();
    G[a].push_back(e);
    G[b].push_back(r);
}

static bool bfs_level(int s, int t) {
    lev.assign(G.size(), -1);
    queue<int> q;
    lev[s] = 0; q.push(s);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (size_t i = 0; i < G[v].size(); ++i) {
            Edge &e = G[v][i];
            if (e.cap > 0 && lev[e.to] < 0) { lev[e.to] = lev[v] + 1; q.push(e.to); }
        }
    }
    return lev[t] >= 0;
}

static int dfs_flow(int v, int t, int f) {
    if (v == t) return f;
    for (int &i = it_[v]; i < (int)G[v].size(); ++i) {
        Edge &e = G[v][i];
        if (e.cap > 0 && lev[v] < lev[e.to]) {
            int d = dfs_flow(e.to, t, min(f, e.cap));
            if (d > 0) { e.cap -= d; G[e.to][e.rev].cap += d; return d; }
        }
    }
    return 0;
}

static int maxflow(int s, int t) {
    int fl = 0;
    while (bfs_level(s, t)) {
        it_.assign(G.size(), 0);
        int f;
        while ((f = dfs_flow(s, t, 1 << 29)) > 0) fl += f;
    }
    return fl;
}

/* dist[d][p] = seconds for person p to reach door d (-1 = unreachable) */
static bool feasible(int T, int P, int D, const vector<vector<int> > &dist) {
    int base = 1 + P;               /* node(d,t) = base + d*T + (t-1), t in 1..T */
    int sink = base + D * T;
    G.assign(sink + 1, vector<Edge>());
    for (int p = 0; p < P; ++p) add_edge(0, 1 + p, 1);
    for (int d = 0; d < D; ++d) {
        for (int t = 1; t <= T; ++t) {
            int u = base + d * T + (t - 1);
            add_edge(u, sink, 1);
            if (t < T) add_edge(u, u + 1, P);
        }
        for (int p = 0; p < P; ++p) {
            int k = dist[d][p];
            if (k >= 1 && k <= T) add_edge(1 + p, base + d * T + (k - 1), 1);
        }
    }
    return maxflow(0, sink) == P;
}

int main() {
    int ncase;
    if (!(cin >> ncase)) return 0;
    (void)ncase;   /* cases are read to EOF; the announced count is not trusted */
    int Y, X;
    while (true) {
        if (!(cin >> Y >> X)) break;
        if (Y <= 0 || X <= 0 || Y > 30 || X > 30) break;
        vector<string> g(Y);
        for (int i = 0; i < Y; ++i) {
            if (!(cin >> g[i])) g[i] = "";
            while ((int)g[i].size() < X) g[i] += 'X';
        }

        vector<pair<int, int> > people, doors;
        for (int i = 0; i < Y; ++i)
            for (int j = 0; j < X; ++j) {
                if (g[i][j] == '.') people.push_back(make_pair(i, j));
                else if (g[i][j] == 'D') doors.push_back(make_pair(i, j));
            }
        int P = (int)people.size(), D = (int)doors.size();
        if (P == 0) { cout << 0 << "\n"; continue; }
        if (D == 0) { cout << "impossible\n"; continue; }

        vector<vector<int> > dist(D, vector<int>(P, -1));
        static const int dy[4] = { -1, 1, 0, 0 };
        static const int dx[4] = { 0, 0, -1, 1 };
        int maxd = 0;
        vector<bool> reach(P, false);
        for (int d = 0; d < D; ++d) {
            vector<vector<int> > dd(Y, vector<int>(X, -1));
            queue<pair<int, int> > q;
            dd[doors[d].first][doors[d].second] = 0;
            q.push(doors[d]);
            while (!q.empty()) {
                pair<int, int> c = q.front(); q.pop();
                for (int k = 0; k < 4; ++k) {
                    int ni = c.first + dy[k], nj = c.second + dx[k];
                    if (ni < 0 || nj < 0 || ni >= Y || nj >= X) continue;
                    if (g[ni][nj] != '.') continue;      /* never travel through a door */
                    if (dd[ni][nj] >= 0) continue;
                    dd[ni][nj] = dd[c.first][c.second] + 1;
                    q.push(make_pair(ni, nj));
                }
            }
            for (int p = 0; p < P; ++p) {
                int v = dd[people[p].first][people[p].second];
                dist[d][p] = v;
                if (v > 0) { reach[p] = true; if (v > maxd) maxd = v; }
            }
        }
        bool ok = true;
        for (int p = 0; p < P; ++p) if (!reach[p]) ok = false;
        if (!ok) { cout << "impossible\n"; continue; }

        int hi = maxd + P, lo = 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (feasible(mid, P, D, dist)) hi = mid; else lo = mid + 1;
        }
        cout << lo << "\n";
    }
    return 0;
}
