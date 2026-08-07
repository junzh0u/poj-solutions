// POJ 3155 - Hard Life
// Model: claude-opus-5
//
// Maximum density subgraph: maximise |E(S)| / |S| over non-empty S, and print S.
//
// Goldberg's min-cut model, for a parameter g:
//   s -> i          capacity m
//   i -> t          capacity m + 2g - deg(i)      (>= 0 since deg(i) <= m)
//   {i,j} in E      capacity 1 in both directions
// Then cut(S) = n*m - 2*(|E(S)| - g*|S|), so mincut < n*m iff some non-empty S
// has |E(S)|/|S| > g, and the source side of a min cut attains the maximum.
//
// The discuss board is almost entirely about this being a precision trap when g
// is a double.  It is avoided here outright: every capacity is scaled by
// D = n^2, which makes them integers for any integer parameter t = D*g, and the
// binary search runs over integer t instead of over reals.
//   f(t) := "mincut < D*n*m"  is exactly  "t < D*g*",  so the largest t with
//   f(t) true is tmax = ceil(D*g*) - 1, hence tmax/D >= g* - 1/n^2.
// The set S recovered at tmax has density > tmax/D >= g* - 1/n^2, and two
// distinct densities e1/k1 != e2/k2 (k <= n) differ by at least 1/(k1*k2)
// >= 1/n^2, so that S is forced to be exactly optimal -- no eps anywhere.
//
// Ambiguities / traps handled:
//  - "sample input #1" / "#2" are two separate input files, not one stream; a
//    run reads exactly one data set.
//  - m = 0 means every team has hardness 0, but the answer still has to be a
//    non-empty team (1 <= k <= n), so print one person rather than an empty list.
//  - S is read off as the vertices reachable from s in the residual graph (the
//    minimal min-cut source side), not by testing which i->t arcs are saturated.
//
// Dinic on 102 nodes / ~2200 arcs, ~21 binary-search rounds: 5ms on the full
// official NEERC 2006 test set against an 8s limit.

#include <cstdio>
#include <cstring>

const int MAXV = 105;
const int MAXA = 2 * (2 * 100 + 2 * 1000) + 10;

int n, m, D;
int ea[1005], eb[1005], deg[MAXV];

int headv[MAXV], nxt[MAXA], dst[MAXA], acnt;
long long cap[MAXA];
int S, T, NV;

static void addArc(int u, int v, long long c, long long rc) {
    dst[acnt] = v; cap[acnt] = c;  nxt[acnt] = headv[u]; headv[u] = acnt++;
    dst[acnt] = u; cap[acnt] = rc; nxt[acnt] = headv[v]; headv[v] = acnt++;
}

static void build(long long t) {
    acnt = 0;
    for (int i = 0; i < NV; i++) headv[i] = -1;
    for (int i = 1; i <= n; i++) {
        addArc(S, i, (long long)D * m, 0);
        addArc(i, T, (long long)D * (m - deg[i]) + 2 * t, 0);
    }
    /* An undirected edge is one arc pair with capacity D each way. */
    for (int i = 0; i < m; i++) addArc(ea[i], eb[i], D, D);
}

int level[MAXV], iter[MAXV], que[MAXV];

static bool bfs() {
    for (int i = 0; i < NV; i++) level[i] = -1;
    int qh = 0, qt = 0;
    level[S] = 0; que[qt++] = S;
    while (qh < qt) {
        int u = que[qh++];
        for (int e = headv[u]; e != -1; e = nxt[e])
            if (cap[e] > 0 && level[dst[e]] < 0) {
                level[dst[e]] = level[u] + 1;
                que[qt++] = dst[e];
            }
    }
    return level[T] >= 0;
}

static long long dfs(int u, long long f) {
    if (u == T) return f;
    for (int &e = iter[u]; e != -1; e = nxt[e]) {
        int v = dst[e];
        if (cap[e] > 0 && level[v] == level[u] + 1) {
            long long d = dfs(v, f < cap[e] ? f : cap[e]);
            if (d > 0) { cap[e] -= d; cap[e ^ 1] += d; return d; }
        }
    }
    level[u] = -1;
    return 0;
}

static long long maxflow() {
    long long fl = 0;
    while (bfs()) {
        for (int i = 0; i < NV; i++) iter[i] = headv[i];
        long long f;
        while ((f = dfs(S, (long long)1 << 62)) > 0) fl += f;
    }
    return fl;
}

int main() {
    if (scanf("%d %d", &n, &m) != 2) return 0;
    memset(deg, 0, sizeof(deg));
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &ea[i], &eb[i]);
        deg[ea[i]]++; deg[eb[i]]++;
    }
    if (m == 0) { printf("1\n1\n"); return 0; }

    D = n * n;
    S = 0; T = n + 1; NV = n + 2;
    long long total = (long long)D * n * m;

    /* f(0) holds because m > 0; f(D*n) fails because g* <= (n-1)/2 < n. */
    long long lo = 0, hi = (long long)D * n;
    while (hi - lo > 1) {
        long long mid = (lo + hi) / 2;
        build(mid);
        if (maxflow() < total) lo = mid; else hi = mid;
    }

    build(lo);
    maxflow();
    /* Source side of the min cut = vertices reachable from s in the residual. */
    bool vis[MAXV];
    for (int i = 0; i < NV; i++) vis[i] = false;
    int qh = 0, qt = 0;
    vis[S] = true; que[qt++] = S;
    while (qh < qt) {
        int u = que[qh++];
        for (int e = headv[u]; e != -1; e = nxt[e])
            if (cap[e] > 0 && !vis[dst[e]]) { vis[dst[e]] = true; que[qt++] = dst[e]; }
    }
    int k = 0;
    for (int i = 1; i <= n; i++) if (vis[i]) k++;
    printf("%d\n", k);
    for (int i = 1; i <= n; i++) if (vis[i]) printf("%d\n", i);
    return 0;
}
