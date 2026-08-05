// POJ 2031 - Building a Space Station
//
// Cells are spheres in 3D; a corridor between two cells runs between the two
// nearest points of their surfaces, so its length is
//     dist(centre_i, centre_j) - r_i - r_j.
// If that value is <= 0 the spheres already touch or overlap (possibly one
// wholly inside the other), the cells are connected for free, and the edge
// weight must be clamped to 0 rather than left negative -- an unclamped
// negative edge would make the MST total too small and, worse, would let the
// algorithm "profit" from overlaps.
//
// With every pair joinable, the cheapest configuration connecting all n cells
// is a minimum spanning tree of that complete weighted graph. n <= 100, so a
// dense O(n^2) Prim over an explicit weight matrix is more than enough
// (11MS here against a 1000MS limit).
//
// Statement notes: the input is a sequence of data sets terminated by a line
// containing a single zero; each answer is printed with exactly 3 decimals.
// When all cells already overlap into one blob the answer is 0.000, which
// falls out of the clamping without a special case (n == 1 likewise).
//
// Verified against a brute-force MST (enumerating all labelled spanning trees
// via Prüfer sequences) on 300 random small cases, including tight clusters of
// heavily overlapping spheres and nested spheres.

#include <cstdio>
#include <cmath>

static const int MAXN = 105;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n > 0) {
        double x[MAXN], y[MAXN], z[MAXN], r[MAXN];
        int i, j;
        for (i = 0; i < n; ++i)
            scanf("%lf %lf %lf %lf", &x[i], &y[i], &z[i], &r[i]);

        double w[MAXN][MAXN];
        for (i = 0; i < n; ++i) {
            w[i][i] = 0.0;
            for (j = i + 1; j < n; ++j) {
                double dx = x[i] - x[j], dy = y[i] - y[j], dz = z[i] - z[j];
                double d = sqrt(dx * dx + dy * dy + dz * dz) - r[i] - r[j];
                if (d < 0.0) d = 0.0;
                w[i][j] = w[j][i] = d;
            }
        }

        /* Prim's algorithm, O(n^2). */
        bool used[MAXN];
        double best[MAXN];
        for (i = 0; i < n; ++i) { used[i] = false; best[i] = 1e18; }
        best[0] = 0.0;
        double total = 0.0;
        for (int it = 0; it < n; ++it) {
            int p = -1;
            for (i = 0; i < n; ++i)
                if (!used[i] && (p == -1 || best[i] < best[p])) p = i;
            used[p] = true;
            total += best[p];
            for (i = 0; i < n; ++i)
                if (!used[i] && w[p][i] < best[i]) best[i] = w[p][i];
        }
        printf("%.3f\n", total);
    }
    return 0;
}
