// POJ 1637 - Sightseeing tour
//
// Does a mixed graph (some streets one-way, some two-way) have an Eulerian
// circuit?  Three conditions, checked in that order:
//
//   1. Connectivity.  Every junction that carries a street must lie in one
//      component of the underlying undirected graph (union-find).  Junctions
//      with no street at all are irrelevant: the tour must cover every street,
//      not every junction.
//   2. Parity.  Orient every two-way street arbitrarily as x -> y and track
//      balance[v] = out(v) - in(v).  Since out-in and out+in share a parity,
//      an odd balance[v] means an odd total degree, which no orientation can
//      fix -> impossible.
//   3. Orientability.  What remains is: can the two-way streets be oriented so
//      that in(v) == out(v) everywhere?  That is a max-flow feasibility
//      question, not a counting one.  Reversing an already-oriented two-way
//      street moves exactly one unit of imbalance from x to y, so build a
//      network where each two-way street is an arc x -> y of capacity 1, the
//      source feeds every junction with balance[v] > 0 with balance[v]/2, and
//      every junction with balance[v] < 0 drains -balance[v]/2 to the sink.
//      A tour exists iff the max flow saturates the source, i.e. every excess
//      can be pushed away along street reversals.
//
// Dinic on ~202 nodes and ~1200 arcs; 12MS for the whole input.
//
// Statement notes:
//   - "m and s ... the number of junctions and streets" - m is the vertex
//     count and s the edge count, the opposite of the usual naming.
//   - Self-loops (the sample's "2 2 0") and parallel streets both occur; each
//     is harmless here, a loop contributing nothing to any balance.
//   - The input guarantees "there exists a junction from where all other
//     junctions can be reached", so check 1 never actually fires on the judge
//     data, but it costs nothing and makes the program correct without it.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 205;      /* m <= 200 junctions, plus source and sink */
const int MAXE = 2 * (1000 + 200 + 5); /* undirected streets + balance arcs, doubled */
const int INF = 1000000000;

int head[MAXN], nxt[MAXE], to_[MAXE], cap_[MAXE];
int ecnt;
int level_[MAXN], iter_[MAXN];
int S, T, N;

void initGraph(int n) {
    N = n;
    ecnt = 0;
    for (int i = 0; i < n; ++i) head[i] = -1;
}

void addEdge(int u, int v, int c) {
    to_[ecnt] = v; cap_[ecnt] = c; nxt[ecnt] = head[u]; head[u] = ecnt++;
    to_[ecnt] = u; cap_[ecnt] = 0; nxt[ecnt] = head[v]; head[v] = ecnt++;
}

int queue_[MAXN];

bool bfs() {
    for (int i = 0; i < N; ++i) level_[i] = -1;
    int qh = 0, qt = 0;
    level_[S] = 0;
    queue_[qt++] = S;
    while (qh < qt) {
        int u = queue_[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            if (cap_[e] > 0 && level_[to_[e]] < 0) {
                level_[to_[e]] = level_[u] + 1;
                queue_[qt++] = to_[e];
            }
        }
    }
    return level_[T] >= 0;
}

int dfs(int u, int f) {
    if (u == T) return f;
    for (int &e = iter_[u]; e != -1; e = nxt[e]) {
        int v = to_[e];
        if (cap_[e] > 0 && level_[v] == level_[u] + 1) {
            int d = dfs(v, min(f, cap_[e]));
            if (d > 0) {
                cap_[e] -= d;
                cap_[e ^ 1] += d;
                return d;
            }
        }
    }
    return 0;
}

int maxflow() {
    int flow = 0;
    while (bfs()) {
        for (int i = 0; i < N; ++i) iter_[i] = head[i];
        int f;
        while ((f = dfs(S, INF)) > 0) flow += f;
    }
    return flow;
}

/* union-find over junctions, for the connectivity check */
int par[MAXN];
int findp(int x) { while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; } return x; }
void unite(int a, int b) { a = findp(a); b = findp(b); if (a != b) par[a] = b; }

int balance[MAXN];
int ex[1005], ey[1005], ed[1005];

int main() {
    int scenarios;
    if (scanf("%d", &scenarios) != 1) return 0;
    while (scenarios-- > 0) {
        int m, s;
        scanf("%d %d", &m, &s);
        for (int i = 0; i < m; ++i) { balance[i] = 0; par[i] = i; }
        for (int i = 0; i < s; ++i) {
            int x, y, d;
            scanf("%d %d %d", &x, &y, &d);
            --x; --y;
            ex[i] = x; ey[i] = y; ed[i] = d;
            unite(x, y);
            /* orient every street x -> y for now */
            ++balance[x];
            --balance[y];
        }

        /* every junction that carries a street must lie in one component */
        bool ok = true;
        int root = -1;
        for (int i = 0; i < s; ++i) {
            int r = findp(ex[i]);
            if (root < 0) root = r;
            else if (r != root) { ok = false; break; }
        }

        /* total degree must be even everywhere: out-in has the same parity as out+in */
        if (ok) {
            for (int i = 0; i < m; ++i) {
                if (balance[i] & 1) { ok = false; break; }
            }
        }

        if (ok) {
            S = m; T = m + 1;
            initGraph(m + 2);
            for (int i = 0; i < s; ++i) {
                /* flipping a two-way street moves one unit of imbalance back */
                if (ed[i] == 0) addEdge(ex[i], ey[i], 1);
            }
            int need = 0;
            for (int i = 0; i < m; ++i) {
                if (balance[i] > 0) { addEdge(S, i, balance[i] / 2); need += balance[i] / 2; }
                else if (balance[i] < 0) { addEdge(i, T, -balance[i] / 2); }
            }
            if (maxflow() != need) ok = false;
        }

        printf("%s\n", ok ? "possible" : "impossible");
    }
    return 0;
}
