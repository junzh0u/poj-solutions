// POJ 1584 - A Round Peg in a Ground Hole
// Model: claude-opus-5
//
// The hole has "protrusions" exactly when the polygon is not convex, so the
// first test is a convexity check: the cross product of consecutive edges must
// never change sign.  Collinear vertices (cross == 0) are legal and are simply
// ignored -- the discuss board confirms the judge data contains convex polygons
// with collinear edges -- and the vertex list may be given clockwise or
// counter-clockwise, so the accepted sign is whichever one actually occurs.
//
// For a convex polygon, orient it by the sign of its signed area and let
//     d_i = s * cross(v_{i+1} - v_i, p - v_i) / |v_{i+1} - v_i|
// be the signed distance from the peg centre p to the line of edge i, positive
// on the interior side.  The peg fits iff min_i d_i >= r.  This single test
// covers both required conditions: if p lies outside the polygon some d_i is
// negative and the minimum falls below r >= 0 automatically, which is the case
// the board warns about (the peg is not guaranteed to be inside the hole).
// Zero-length edges (repeated vertices) are skipped so no division by zero.
//
// Ambiguity: "the peg fits" is taken to include exact tangency, so the
// comparison uses a 1e-9 tolerance; the same tolerance decides the sign of a
// cross product in the convexity test.

#include <stdio.h>
#include <math.h>

const double EPS = 1e-9;
const int MAXN = 10000;   /* the discuss board reports n < 150 */

int main() {
    int n;
    double r, px, py;
    static double x[MAXN], y[MAXN];

    while (scanf("%d", &n) == 1 && n >= 3) {
        if (scanf("%lf %lf %lf", &r, &px, &py) != 3) break;
        if (n > MAXN) n = MAXN;
        for (int i = 0; i < n; i++)
            if (scanf("%lf %lf", &x[i], &y[i]) != 2) return 0;

        /* convexity: sign of cross(v[i]-v[i-1], v[i+1]-v[i]) must be consistent */
        int pos = 0, neg = 0;
        double area2 = 0.0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n, k = (i + 2) % n;
            double cr = (x[j] - x[i]) * (y[k] - y[j]) - (y[j] - y[i]) * (x[k] - x[j]);
            if (cr > EPS) pos = 1;
            else if (cr < -EPS) neg = 1;
            area2 += x[i] * y[j] - x[j] * y[i];
        }

        if (pos && neg) {
            printf("HOLE IS ILL-FORMED\n");
            continue;
        }

        double s = (area2 < 0.0) ? -1.0 : 1.0;   /* orient outward-consistently */
        double best = 1e100;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            double ex = x[j] - x[i], ey = y[j] - y[i];
            double len = sqrt(ex * ex + ey * ey);
            if (len < 1e-12) continue;           /* repeated vertex */
            double cr = ex * (py - y[i]) - ey * (px - x[i]);
            double d = s * cr / len;
            if (d < best) best = d;
        }

        if (best >= r - EPS) printf("PEG WILL FIT\n");
        else printf("PEG WILL NOT FIT\n");
    }
    return 0;
}
