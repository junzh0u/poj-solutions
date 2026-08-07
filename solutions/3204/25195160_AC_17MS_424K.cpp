// POJ 3204 - Ikki's Story I - Road Reconstruction
// Model: claude-opus-5
//
// Count the roads whose capacity, if raised, would raise the maximum flow
// from city 0 to city n-1.
//
// Run one max flow (Dinic).  In the residual network let
//     A = { u : s can reach u },   B = { v : v can reach t }.
// Adding one unit of capacity to road (u,v) puts one unit of residual room on
// the arc u->v, so it creates a new augmenting path exactly when u is in A and
// v is in B.  Such an edge is necessarily already saturated: if it still had
// residual room and u were in A, then v would be in A too, and a vertex in
// both A and B would contradict maximality of the flow.
//
// Notes from the data / discuss board: roads are directed, parallel and
// antiparallel roads occur (so every arc gets its own reverse arc of capacity
// 0, and each input line is counted separately), the graph may contain cycles,
// and capacity-0 roads occur -- those are genuinely critical when u is in A
// and v is in B, so they must not be filtered out.

#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 505;
const int MAXM = 5005;
const int MAXE = 2 * MAXM;

int head[MAXN], nxt[MAXE], dest[MAXE], cap[MAXE];
int ecnt;
int ea[MAXM], eb[MAXM];
int level_[MAXN], iter_[MAXN], que[MAXN];
bool fromS[MAXN], toT[MAXN];
int n, m;

static void addEdge(int u, int v, int c) {
    dest[ecnt] = v; cap[ecnt] = c; nxt[ecnt] = head[u]; head[u] = ecnt++;
    dest[ecnt] = u; cap[ecnt] = 0; nxt[ecnt] = head[v]; head[v] = ecnt++;
}

static bool bfsLevel(int s, int t) {
    for (int i = 0; i < n; ++i) level_[i] = -1;
    int qh = 0, qt = 0;
    que[qt++] = s; level_[s] = 0;
    while (qh < qt) {
        int u = que[qh++];
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = dest[e];
            if (cap[e] > 0 && level_[v] < 0) {
                level_[v] = level_[u] + 1;
                que[qt++] = v;
            }
        }
    }
    return level_[t] >= 0;
}

static int dfsAug(int u, int t, int f) {
    if (u == t) return f;
    for (int &e = iter_[u]; e != -1; e = nxt[e]) {
        int v = dest[e];
        if (cap[e] > 0 && level_[v] == level_[u] + 1) {
            int d = dfsAug(v, t, min(f, cap[e]));
            if (d > 0) { cap[e] -= d; cap[e ^ 1] += d; return d; }
        }
    }
    return 0;
}

static void maxflow(int s, int t) {
    while (bfsLevel(s, t)) {
        for (int i = 0; i < n; ++i) iter_[i] = head[i];
        int f;
        while ((f = dfsAug(s, t, 1 << 30)) > 0) { }
    }
}

int main() {
    while (scanf("%d %d", &n, &m) == 2) {
        if (n <= 0) break;
        ecnt = 0;
        for (int i = 0; i < n; ++i) head[i] = -1;
        for (int i = 0; i < m; ++i) {
            int a, b, c;
            scanf("%d %d %d", &a, &b, &c);
            ea[i] = a; eb[i] = b;
            addEdge(a, b, c);
        }
        if (n == 1) { printf("0\n"); continue; }

        int s = 0, t = n - 1;
        maxflow(s, t);

        // A: reachable from s along arcs with residual capacity.
        for (int i = 0; i < n; ++i) fromS[i] = false;
        int qh = 0, qt = 0;
        que[qt++] = s; fromS[s] = true;
        while (qh < qt) {
            int u = que[qh++];
            for (int e = head[u]; e != -1; e = nxt[e])
                if (cap[e] > 0 && !fromS[dest[e]]) {
                    fromS[dest[e]] = true;
                    que[qt++] = dest[e];
                }
        }

        // B: can reach t.  Arc x->v exists iff the residual capacity of the
        // arc leaving v that pairs with it (e^1, going x->v) is positive.
        for (int i = 0; i < n; ++i) toT[i] = false;
        qh = qt = 0;
        que[qt++] = t; toT[t] = true;
        while (qh < qt) {
            int v = que[qh++];
            for (int e = head[v]; e != -1; e = nxt[e]) {
                int x = dest[e];
                if (cap[e ^ 1] > 0 && !toT[x]) {
                    toT[x] = true;
                    que[qt++] = x;
                }
            }
        }

        int ans = 0;
        for (int i = 0; i < m; ++i)
            if (cap[2 * i] == 0 && fromS[ea[i]] && toT[eb[i]]) ++ans;
        printf("%d\n", ans);
    }
    return 0;
}
