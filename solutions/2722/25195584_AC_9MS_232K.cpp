// POJ 2722 - Angle and Squares
// Model: claude-opus-5
//
// The squares must form a barrier from one radial line to the other; the enclosed
// region is bounded by ray OA, the lower-left staircase of that barrier, and ray OB.
// Shoelace around O gives area = 1/2 * sum over staircase edges of (w_j*y_j + h_j*x_j)
// (w = horizontal run at height y, h = vertical drop at abscissa x), so every edge wants
// to be as long and as far from O as possible: the optimum is the corner-to-corner chain,
// each square contributing its full side down and its full side right, its first corner on
// the steeper ray and its last on the shallower one.  With S = sum a_i that puts
// P = (p, k1*p) and Q = (p+S, k1*p-S) on the two rays and collapses to
//
//     area = 1/2 * ( S^2 * (xa+ya)*(xb+yb) / |xa*yb - xb*ya|  -  sum a_i^2 )
//
// which is independent of the order of the squares and symmetric in A and B.  Verified
// against an independent reference that flood-fills the exact arrangement of the sector
// minus the squares, under randomized global search over free placements (n = 1..3,
// several angles): nothing ever beat this value.
//
// Note sum a_i^2, not S^2 -- the sample has n = 1, where the two coincide.
//
// Precision (the discuss board's recurring complaint): the triangle is computed from the
// cross product, never from Heron's formula, and the inputs are snapped to exact integers
// scaled by 1e6 so that xa*yb - xb*ya is computed exactly.  That denominator is a
// difference of near-equal products for a narrow angle, where it is also the whole
// magnitude of the answer, so it is the one quantity that must not lose digits.
//
// Ambiguity: the statement does not say the two radial lines are distinct.  Collinear
// rays enclose no area, so that degenerate case prints 0.000.

#include <cstdio>
#include <cmath>

static const double SCALE = 1000000.0;

/* A decimal with at most 6 places, snapped to its exact integer value times 1e6.
   Every scaled input is then <= 2e7, so xa*yb and xb*ya are whole numbers below 4e14
   and their difference is computed exactly in double -- and that difference is where a
   narrow angle would otherwise lose every digit it has. */
static double scaled(double v) {
    return floor(v * SCALE + 0.5);
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        double xa, ya, xb, yb;
        if (scanf("%lf %lf %lf %lf", &xa, &ya, &xb, &yb) != 4) return 0;
        double XA = scaled(xa), YA = scaled(ya), XB = scaled(xb), YB = scaled(yb);
        double S = 0.0, T = 0.0;                   /* sum a_i and sum a_i^2, scaled */
        for (int i = 0; i < n; i++) {
            double a;
            if (scanf("%lf", &a) != 1) return 0;
            double A = scaled(a);
            S += A;
            T += A * A;
        }
        double cross = fabs(XA * YB - XB * YA);    /* exact: each product < 4e14 < 2^53 */
        if (cross == 0.0) {                        /* degenerate angle encloses nothing */
            printf("0.000\n");
            continue;
        }
        double area = 0.5 * (S * S * (XA + YA) * (XB + YB) / (SCALE * SCALE * cross)
                             - T / (SCALE * SCALE));
        printf("%.3f\n", area);
    }
    return 0;
}
