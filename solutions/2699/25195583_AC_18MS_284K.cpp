// POJ 2699 - The Maximum Number of Strong Kings
// Model: claude-opus-5
//
// A strong king beats every player whose score is strictly greater than its
// own.  For a candidate set K of strong kings the tournament is constrained
// only by: (a) each pair plays once, (b) player i wins exactly s[i] games,
// (c) for i in K and s[j] > s[i], i beats j.  Constraint (c) fixes some
// matches outright (never contradictorily: if s[i] != s[j] at most one of the
// two directions is forced); the rest is a degree-constrained orientation,
// i.e. a bipartite flow feasibility test - source -> each free match (cap 1)
// -> its two players (cap 1) -> sink (cap = remaining wins).  K is realizable
// iff the flow saturates every free match.
//
// The strong-king set of any tournament satisfies (c), and any tournament
// satisfying (c) makes every member of K a strong king, so the answer is the
// largest realizable K.  n <= 10, so all 2^n subsets are tested rather than
// relying on the (true, but unproved here) folklore that an optimal K is the
// top-k scores; that costs ~4ms for the whole worst case.
//
// Input framing: the discuss board reports irregular whitespace ("1  1 1  ")
// and stray content after the case count, so the count is taken as the first
// integer of the first line and each following non-empty line is parsed as a
// whole sequence with istringstream.  The sequence length is what the line
// holds, and is not stated anywhere in the input.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 12;
const int MAXV = 2 + 64 + MAXN;
const int INF = 1000000000;

struct Edge { int to, cap, rev; };
vector<Edge> graph_[MAXV];
int level_[MAXV], iter_[MAXV], nv;

void initGraph(int n) {
    nv = n;
    for (int i = 0; i < n; i++) graph_[i].clear();
}
void addEdge(int from, int to, int cap) {
    Edge a; a.to = to; a.cap = cap; a.rev = (int)graph_[to].size();
    Edge b; b.to = from; b.cap = 0; b.rev = (int)graph_[from].size();
    graph_[from].push_back(a);
    graph_[to].push_back(b);
}
bool bfs(int s, int t) {
    for (int i = 0; i < nv; i++) level_[i] = -1;
    int q[MAXV], head = 0, tail = 0;
    level_[s] = 0; q[tail++] = s;
    while (head < tail) {
        int v = q[head++];
        for (size_t i = 0; i < graph_[v].size(); i++) {
            Edge &e = graph_[v][i];
            if (e.cap > 0 && level_[e.to] < 0) {
                level_[e.to] = level_[v] + 1;
                q[tail++] = e.to;
            }
        }
    }
    return level_[t] >= 0;
}
int dfs(int v, int t, int f) {
    if (v == t) return f;
    for (int &i = iter_[v]; i < (int)graph_[v].size(); i++) {
        Edge &e = graph_[v][i];
        if (e.cap > 0 && level_[v] < level_[e.to]) {
            int d = dfs(e.to, t, min(f, e.cap));
            if (d > 0) {
                e.cap -= d;
                graph_[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}
int maxflow(int s, int t) {
    int flow = 0;
    while (bfs(s, t)) {
        for (int i = 0; i < nv; i++) iter_[i] = 0;
        int f;
        while ((f = dfs(s, t, INF)) > 0) flow += f;
    }
    return flow;
}

int n;
int s[MAXN];

/* Can every player in the set `mask` be a strong king simultaneously? */
bool feasible(int mask) {
    int need[MAXN];
    int i, j;
    for (i = 0; i < n; i++) need[i] = s[i];

    /* forced results: a strong king must beat everyone with a strictly larger score */
    bool forced[MAXN][MAXN];
    for (i = 0; i < n; i++) for (j = 0; j < n; j++) forced[i][j] = false;
    for (i = 0; i < n; i++) {
        if (!(mask & (1 << i))) continue;
        for (j = 0; j < n; j++) {
            if (j == i) continue;
            if (s[j] > s[i]) forced[i][j] = true;
        }
    }
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (forced[i][j] && forced[j][i]) return false; /* cannot happen, but be safe */
            if (forced[i][j]) { if (--need[i] < 0) return false; }
            else if (forced[j][i]) { if (--need[j] < 0) return false; }
        }
    }

    /* remaining (free) matches distributed by max flow */
    int freeCnt = 0;
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (!forced[i][j] && !forced[j][i]) freeCnt++;

    int src = 0, playerBase = 1 + freeCnt, snk = playerBase + n;
    initGraph(snk + 1);
    int id = 1;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (forced[i][j] || forced[j][i]) continue;
            addEdge(src, id, 1);
            addEdge(id, playerBase + i, 1);
            addEdge(id, playerBase + j, 1);
            id++;
        }
    }
    for (i = 0; i < n; i++) addEdge(playerBase + i, snk, need[i]);

    return maxflow(src, snk) == freeCnt;
}

int popcount(int x) { int c = 0; while (x) { c += x & 1; x >>= 1; } return c; }

int solve() {
    int best = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        int pc = popcount(mask);
        if (pc <= best) continue;
        if (feasible(mask)) best = pc;
    }
    return best;
}

int main() {
    string line;
    int m = 0;

    /* The first line holds the number of test cases; the board reports stray
       trailing content there, so take only its first integer. */
    while (getline(cin, line)) {
        istringstream is(line);
        if (is >> m) break;
    }

    int done = 0;
    while (done < m && getline(cin, line)) {
        istringstream is(line);
        vector<int> v;
        int x;
        while (is >> x) v.push_back(x);
        if (v.empty()) continue;   /* blank / whitespace-only line: not a case */
        n = (int)v.size();
        if (n > MAXN) n = MAXN;
        for (int i = 0; i < n; i++) s[i] = v[i];
        sort(s, s + n);
        printf("%d\n", solve());
        done++;
    }
    return 0;
}
