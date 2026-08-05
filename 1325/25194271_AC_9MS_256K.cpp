// POJ 1325 - Machine Schedule
//
// Machines A and B have n and m modes; each job runs either on A in mode x or
// on B in mode y. Changing a mode costs one restart. Minimise the restarts.
//
// Model each job as an edge (x, y) of a bipartite graph over A-modes and
// B-modes. Picking a mode to restart into covers every job that mode can run,
// so a valid schedule is exactly a vertex cover of that graph and the answer is
// the minimum vertex cover, which by Koenig's theorem equals the maximum
// matching. Kuhn's augmenting-path algorithm computes it in O(V*E), trivial at
// n, m < 100 and k < 1000.
//
// The trap in the statement: both machines *boot* in mode_0. A job with x == 0
// or y == 0 is therefore already runnable before anything is restarted, so it
// must not become an edge, and mode_0 must not be a candidate cover vertex --
// hence the matching runs over modes 1..n-1 and 1..m-1 only. Including those
// edges is the classic wrong answer here.
//
// Other statement details: the k lines are triples (i, x, y) whose leading job
// index i is redundant and ignored; duplicate (x, y) pairs collapse naturally
// because the graph is stored as an adjacency matrix; and the file holds
// several configurations, ending at a line with a single zero (n == 0).

#include <cstdio>
#include <cstring>

static const int MAXN = 105;

static bool g[MAXN][MAXN];
static int matchB[MAXN];
static bool used[MAXN];
static int n, m, k;

static bool tryKuhn(int a) {
    for (int b = 1; b < m; ++b) {
        if (!g[a][b] || used[b]) continue;
        used[b] = true;
        if (matchB[b] == -1 || tryKuhn(matchB[b])) {
            matchB[b] = a;
            return true;
        }
    }
    return false;
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        if (scanf("%d %d", &m, &k) != 2) break;
        memset(g, 0, sizeof(g));
        for (int t = 0; t < k; ++t) {
            int i, x, y;
            if (scanf("%d %d %d", &i, &x, &y) != 3) break;
            /* Both machines boot in mode_0: a job runnable in mode_0 on
               either machine costs no restart at all, so it is not an edge. */
            if (x <= 0 || y <= 0) continue;
            if (x < n && y < m) g[x][y] = true;
        }
        for (int b = 0; b < m; ++b) matchB[b] = -1;
        int res = 0;
        for (int a = 1; a < n; ++a) {
            memset(used, 0, sizeof(used));
            if (tryKuhn(a)) ++res;
        }
        printf("%d\n", res);
    }
    return 0;
}
