// POJ 3814 - Museum Guards
// Model: claude-sonnet-5
// Approach: Binary search on the answer k (min guards on duty at any time).
// Feasibility check for a given k is a flow-with-lower-bounds problem:
//   S -> guard_i (cap = floor(M_i/30), the guard's half-hour-block budget)
//   guard_i -> slot_j (cap 1) if guard i is fully available during half-hour
//     slot j (slot j is available only if EVERY minute of it lies in the
//     union of the guard's K availability windows, honoring wraparound past
//     midnight and the "start==end means available all day" special rule)
//   slot_j -> T (lower bound k, cap N)
// Solved via the standard supersource/supersink lower-bound reduction plus a
// T->S return edge to turn it into a circulation feasibility check; feasible
// iff max-flow(SS,TT) saturates all edges out of SS (== 48*k). Feasibility is
// monotone decreasing in k, so binary search over k in [0,N].
// No statement ambiguity found; board only confirms the binary-search+maxflow
// approach with no additional pitfalls.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

struct Dinic {
    struct Edge { int to, cap, rev; };
    vector<vector<Edge> > g;
    vector<int> level, iter_, qbuf;
    int n;
    void init(int n_) { n = n_; g.assign(n, vector<Edge>()); level.assign(n, -1); iter_.assign(n, 0); qbuf.assign(n, 0); }
    void add_edge(int from, int to, int cap) {
        Edge a; a.to = to; a.cap = cap; a.rev = (int)g[to].size();
        Edge b; b.to = from; b.cap = 0; b.rev = (int)g[from].size();
        g[from].push_back(a);
        g[to].push_back(b);
    }
    bool bfs(int s, int t) {
        for (int i = 0; i < n; i++) level[i] = -1;
        int head = 0, tail = 0;
        level[s] = 0; qbuf[tail++] = s;
        while (head < tail) {
            int v = qbuf[head++];
            for (size_t i = 0; i < g[v].size(); i++) {
                Edge &e = g[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    qbuf[tail++] = e.to;
                }
            }
        }
        return level[t] >= 0;
    }
    int dfs(int v, int t, int f) {
        if (v == t) return f;
        for (int &i = iter_[v]; i < (int)g[v].size(); i++) {
            Edge &e = g[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }
    int max_flow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            iter_.assign(n, 0);
            int f;
            while ((f = dfs(s, t, 1 << 30)) > 0) flow += f;
        }
        return flow;
    }
};

static const int SLOTS = 48;

int N;
bool guardSlot[50][SLOTS];
int guardCap[50];

// Node layout: 0=SS 1=TT 2=S 3=T 4..4+N-1=guards 4+N..4+N+47=slots
static const int MAXN_NODES = 4 + 50 + SLOTS;
Dinic din;

bool feasible(int k) {
    int base = 4;
    int guardBase = base;
    int slotBase = base + N;
    int n = MAXN_NODES; // fixed size; unused guard slots (i>=N) simply get no edges
    if ((int)din.g.size() != n) din.init(n);
    else for (int i = 0; i < n; i++) din.g[i].clear();
    const int SS = 0, TT = 1, S = 2, T = 3;

    for (int i = 0; i < N; i++) {
        din.add_edge(S, guardBase + i, guardCap[i]);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < SLOTS; j++) {
            if (guardSlot[i][j]) din.add_edge(guardBase + i, slotBase + j, 1);
        }
    }
    for (int j = 0; j < SLOTS; j++) {
        din.add_edge(slotBase + j, T, N - k); // cap - lower
        din.add_edge(slotBase + j, TT, k);    // negative excess -> TT
    }
    din.add_edge(T, S, 1000000); // circulation return edge
    din.add_edge(SS, T, SLOTS * k);

    int need = SLOTS * k;
    int flow = din.max_flow(SS, TT);
    return flow == need;
}

int parseTime(const char *s) {
    int hh, mm;
    sscanf(s, "%d:%d", &hh, &mm);
    return hh * 60 + mm;
}

int main() {
    while (scanf("%d", &N) == 1 && N != 0) {
        for (int i = 0; i < N; i++) {
            int K, M;
            scanf("%d %d", &K, &M);
            static int diff[1441];
            memset(diff, 0, sizeof(diff));
            bool fullDay = false;
            for (int t = 0; t < K; t++) {
                char sbuf[16], ebuf[16];
                scanf("%s %s", sbuf, ebuf);
                int s = parseTime(sbuf);
                int e = parseTime(ebuf);
                if (s == e) {
                    fullDay = true;
                } else if (s < e) {
                    diff[s]++; diff[e]--;
                } else {
                    diff[s]++; diff[1440]--;
                    diff[0]++; diff[e]--;
                }
            }
            static bool avail[1440];
            if (fullDay) {
                for (int m = 0; m < 1440; m++) avail[m] = true;
            } else {
                int running = 0;
                for (int m = 0; m < 1440; m++) {
                    running += diff[m];
                    avail[m] = running > 0;
                }
            }
            for (int j = 0; j < SLOTS; j++) {
                bool full = true;
                for (int m = 30 * j; m < 30 * j + 30; m++) {
                    if (!avail[m]) { full = false; break; }
                }
                guardSlot[i][j] = full;
            }
            guardCap[i] = M / 30;
        }

        int lo = 0, hi = N, ans = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (feasible(mid)) { ans = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
