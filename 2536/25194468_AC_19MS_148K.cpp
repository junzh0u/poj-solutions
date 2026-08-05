// POJ 2536 - Gopher II
//
// n gophers, m gopher holes (both < 100), a hawk arrives and every gopher that
// fails to reach a hole within s seconds is vulnerable; each hole saves at most
// one gopher and all gophers run at velocity v. Minimising the vulnerable count
// is maximum bipartite matching: put an edge between gopher i and hole j when
// dist(i,j) <= s*v, find a maximum matching, and print n - |matching|.
//
// Matching is Kuhn's algorithm (repeated DFS for an augmenting path) over the
// adjacency matrix. With n, m <= 99 that is O(V*E) = ~10^6 per case, and the
// input holds several cases per file, which still runs in well under the 2s
// limit (2000 maximal cases finished in 0.5s locally).
//
// Statement details worth pinning down:
//   * "if a gopher does not reach a hole in s seconds it is vulnerable", so a
//     gopher exactly s*v metres away IS safe - the comparison is <=, not <.
//   * n, m, s and v are integers but the coordinates are reals, so the test is
//     kept squared (dx*dx + dy*dy <= (s*v)^2) to avoid a sqrt, with a small
//     relative tolerance for the exact-boundary case.
//   * The input runs to EOF with several cases; there is no case count.

#include <cstdio>
#include <cstring>

static const int MAXN = 105;

int n, m;
double gx[MAXN], gy[MAXN], hx[MAXN], hy[MAXN];
bool adj[MAXN][MAXN];
bool used[MAXN];
int matchHole[MAXN]; /* hole -> gopher */

bool tryKuhn(int g) {
    for (int h = 0; h < m; ++h) {
        if (!adj[g][h] || used[h]) continue;
        used[h] = true;
        if (matchHole[h] < 0 || tryKuhn(matchHole[h])) {
            matchHole[h] = g;
            return true;
        }
    }
    return false;
}

int main() {
    int s, v;
    while (scanf("%d %d %d %d", &n, &m, &s, &v) == 4) {
        int i, j;
        for (i = 0; i < n; ++i) scanf("%lf %lf", &gx[i], &gy[i]);
        for (j = 0; j < m; ++j) scanf("%lf %lf", &hx[j], &hy[j]);
        double reach = (double)s * (double)v;
        double lim = reach * reach;
        for (i = 0; i < n; ++i)
            for (j = 0; j < m; ++j) {
                double dx = gx[i] - hx[j], dy = gy[i] - hy[j];
                double d2 = dx * dx + dy * dy;
                adj[i][j] = (d2 <= lim + 1e-9 * (lim + 1.0));
            }
        for (j = 0; j < m; ++j) matchHole[j] = -1;
        int cnt = 0;
        for (i = 0; i < n; ++i) {
            memset(used, 0, sizeof(bool) * m);
            if (tryKuhn(i)) ++cnt;
        }
        printf("%d\n", n - cnt);
    }
    return 0;
}
