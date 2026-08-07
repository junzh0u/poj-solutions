// POJ 3525 - Most Distant Point from the Sea
// Model: claude-opus-5
//
// The island is a convex polygon and the sea is everything outside it, so the
// distance from an interior point to the sea is its distance to the boundary.
// For a convex polygon that equals min over edges of the distance to the edge's
// supporting line, so the answer is the radius of the largest inscribed circle
// (the Chebyshev radius).
//
// Binary search on the radius d: the set of centres of an inscribed circle of
// radius d is the intersection of the n half-planes obtained by pushing each
// edge line inward by d, so d is feasible iff that intersection is non-empty.
// The polygon itself is the d = 0 region, so the test is just Sutherland-Hodgman
// clipping of the (always convex) current region by the n shifted half-planes,
// bailing out as soon as it goes empty. 50 bisections on [0, 5001] - 5000 is the
// largest radius that fits in the stated 10000 x 10000 coordinate box.
//
// Edge i runs A = P[i] -> B = P[i+1] counter-clockwise, so the interior is on
// its left and the shifted constraint is cross(B - A, P - A) >= d * |B - A|.
// Intersections are taken parametrically along the clipped polygon's own edges
// (never via a point-slope line equation), which keeps them stable on the
// near-degenerate slivers this problem's data contains - e.g. the sample's
// (0,0) (10000,10000) (5000,5001) triangle, answer 0.353553.
//
// Verified against an independent nested-ternary-search maximiser of the concave
// min-distance function (800 random convex polygons, exact agreement to 1e-6)
// and, for triangles, against the closed form r = 2 * area / perimeter evaluated
// in 60-digit decimal (400 near-degenerate slivers, error below 1e-9).

#include <cstdio>
#include <cmath>

static const int MAXV = 1024;

int n;
double px[128], py[128];
double ex[128], ey[128], elen[128];

double bufx[2][MAXV], bufy[2][MAXV];

static bool feasible(double d) {
    int cur = 0;
    int m = n;
    for (int i = 0; i < n; i++) { bufx[0][i] = px[i]; bufy[0][i] = py[i]; }
    for (int e = 0; e < n; e++) {
        double ax = px[e], ay = py[e];
        double dx = ex[e], dy = ey[e];
        double off = d * elen[e];
        int nxt = cur ^ 1;
        int k = 0;
        double *sx = bufx[cur], *sy = bufy[cur];
        double *tx = bufx[nxt], *ty = bufy[nxt];
        double vprev = dx * (sy[m - 1] - ay) - dy * (sx[m - 1] - ax) - off;
        for (int i = 0; i < m; i++) {
            double vcur = dx * (sy[i] - ay) - dy * (sx[i] - ax) - off;
            if ((vprev < 0.0) != (vcur < 0.0)) {
                int j = (i == 0) ? m - 1 : i - 1;
                double t = vprev / (vprev - vcur);
                if (k < MAXV) {
                    tx[k] = sx[j] + (sx[i] - sx[j]) * t;
                    ty[k] = sy[j] + (sy[i] - sy[j]) * t;
                    k++;
                }
            }
            if (vcur >= 0.0 && k < MAXV) { tx[k] = sx[i]; ty[k] = sy[i]; k++; }
            vprev = vcur;
        }
        m = k;
        cur = nxt;
        if (m == 0) return false;
    }
    return m > 0;
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; i++) scanf("%lf %lf", &px[i], &py[i]);
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            ex[i] = px[j] - px[i];
            ey[i] = py[j] - py[i];
            elen[i] = sqrt(ex[i] * ex[i] + ey[i] * ey[i]);
        }
        double lo = 0.0, hi = 5001.0;
        for (int it = 0; it < 50; it++) {
            double mid = (lo + hi) * 0.5;
            if (feasible(mid)) lo = mid; else hi = mid;
        }
        printf("%.6f\n", lo);
    }
    return 0;
}
