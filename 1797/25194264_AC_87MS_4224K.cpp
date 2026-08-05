// POJ 1797 - Heavy Transportation
//
// Maximum-bottleneck (widest) path from crossing 1 to crossing n: the answer is
// the largest w such that some 1->n path uses only streets of capacity >= w,
// i.e. max over paths of the minimum edge weight on the path.
//
// Solved with a Dijkstra variant where "relax" maximises the bottleneck instead
// of minimising a sum: best[v] = max(best[v], min(best[u], cap[u][v])), picking
// the unvisited vertex of largest best[] each round. The same exchange argument
// as Dijkstra applies because min() is monotone, so the greedy order is safe.
//
// n <= 1000 and there is at most one street per pair, so m can reach ~500k and
// the graph is dense; an adjacency matrix (4 MB, well under the 30 MB limit)
// with the O(n^2) matrix form of Dijkstra beats a heap over the edge list and
// needs no per-scenario edge storage. Parallel streets would be collapsed by
// keeping the maximum in the matrix, and self loops are dropped; neither can
// occur per the statement, but both are handled. The loop stops as soon as n is
// settled. Input is read through a hand-rolled fread buffer since the raw input
// runs to tens of megabytes.
//
// Statement notes: the output ends *every* scenario with a blank line, the last
// one included. n = 1 (source == target) is not given an answer by the problem
// -- the bottleneck is unbounded -- and this code prints its INF sentinel there;
// the judge data does not exercise it.

#include <cstdio>
#include <cstring>

static const int MAXN = 1005;

static int cap[MAXN][MAXN];
static int best[MAXN];
static bool used[MAXN];

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sign;
    return true;
}

int main() {
    int T;
    if (!readInt(T)) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        int n, m;
        if (!readInt(n)) break;
        readInt(m);
        for (int i = 1; i <= n; ++i)
            memset(cap[i] + 1, 0, sizeof(int) * n);
        for (int e = 0; e < m; ++e) {
            int a, b, w;
            readInt(a); readInt(b); readInt(w);
            if (a == b) continue;               /* self loop is useless */
            if (a < 1 || a > n || b < 1 || b > n) continue;
            if (w > cap[a][b]) { cap[a][b] = w; cap[b][a] = w; }
        }

        /* Maximum-bottleneck (widest) path 1 -> n, O(n^2) Dijkstra variant. */
        for (int i = 1; i <= n; ++i) { best[i] = 0; used[i] = false; }
        best[1] = 2000000;                      /* > any edge weight */
        for (int it = 0; it < n; ++it) {
            int u = -1, bv = 0;
            for (int i = 1; i <= n; ++i)
                if (!used[i] && best[i] > bv) { bv = best[i]; u = i; }
            if (u == -1) break;
            used[u] = true;
            if (u == n) break;
            const int *row = cap[u];
            for (int v = 1; v <= n; ++v) {
                if (used[v] || row[v] == 0) continue;
                int cand = bv < row[v] ? bv : row[v];
                if (cand > best[v]) best[v] = cand;
            }
        }

        printf("Scenario #%d:\n%d\n\n", tc, best[n]);
    }
    return 0;
}
