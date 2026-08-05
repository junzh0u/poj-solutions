// POJ 3615 - Cow Hurdles
//
// Minimax (bottleneck) path on a directed graph: for each query (A,B) report the
// smallest possible value of the tallest hurdle along some A->B route, or -1 when
// B is unreachable from A.
//
// N <= 300 stations, so all-pairs Floyd-Warshall fits comfortably: 300^3 = 27M
// relaxations, and it answers all T <= 40,000 queries in O(1) each afterwards
// (a per-query bottleneck Dijkstra would redo the work 40,000 times).
//
// The only change from ordinary Floyd-Warshall is the composition operator:
// the cost of concatenating two routes is the MAX of their bottlenecks, not the
// sum, so the relaxation is d[i][j] = min(d[i][j], max(d[i][k], d[k][j])).
// Because it is a max and never a sum, INF needs no headroom against overflow.
//
// Details the statement leaves implicit:
//   - Paths are one-way (S -> E only), so the matrix is asymmetric.
//   - Parallel edges between the same pair can differ in height; keep the lowest.
//   - d[i][i] = 0: the tasks are stated to use distinct A and B, but a zero
//     diagonal is both the correct answer for an empty route and what Floyd
//     needs, and it costs nothing to be safe if a test violates that promise.
//   - Self-loops are harmless: they can never lower a bottleneck.
//
// getchar-based input parsing keeps the 25,000 edges plus 40,000 queries cheap.

#include <cstdio>

static const int MAXN = 305;
static const int INF = 0x3f3f3f3f;

static int d[MAXN][MAXN];

static int readInt() {
    int c = getchar();
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    int sgn = 1;
    if (c == '-') { sgn = -1; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = getchar(); }
    return x * sgn;
}

int main() {
    int n = readInt(), m = readInt(), t = readInt();
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) d[i][j] = INF;
        d[i][i] = 0;
    }
    for (int e = 0; e < m; ++e) {
        int s = readInt(), t2 = readInt(), h = readInt();
        if (h < d[s][t2]) d[s][t2] = h;   /* parallel edges: keep the lowest hurdle */
    }
    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            int dik = d[i][k];
            if (dik == INF) continue;
            int *di = d[i], *dk = d[k];
            for (int j = 1; j <= n; ++j) {
                int cand = dk[j] > dik ? dk[j] : dik;   /* bottleneck: max, not sum */
                if (cand < di[j]) di[j] = cand;
            }
        }
    }
    for (int q = 0; q < t; ++q) {
        int a = readInt(), b = readInt();
        int r = d[a][b];
        printf("%d\n", r == INF ? -1 : r);
    }
    return 0;
}
