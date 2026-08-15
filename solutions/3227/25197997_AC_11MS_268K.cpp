// POJ 3227 - Mountains
// Model: claude-sonnet-5
//
// Observer stands at O=(0,h). The mountain silhouette is an x-monotone
// polyline (a function y=f(x)) from (w0,0) to (w_{n-1},0), zig-zagging up
// and down (peaks at odd indices, valleys at even indices except the
// last). We need the total length of the polyline visible from O, where
// the mountain is opaque (solid) below its own profile, so a point P on
// the terrain is visible iff the straight segment OP never dips below the
// terrain for any x between 0 and x_P.
//
// Since the terrain is piecewise linear, the binding constraint of
// "segment OP stays weakly above terrain" is entirely determined by the
// terrain VERTICES between O and P (the extrema of a piecewise-linear
// difference occur at breakpoints). Writing s(x,y) = (y-h)/x for the slope
// from O to a point (valid since all terrain x>0 except possibly the very
// first vertex), the condition "OP stays above vertex V_k" is exactly
// s_P >= s_k. Hence point P is visible iff its slope from O is >= the
// running maximum slope of all earlier vertices (0..i, where i is the
// start of P's edge).
//
// Along a single straight edge, s(t) is a linear-fractional (Mobius)
// function of the edge parameter t, hence strictly monotonic (or constant
// when the edge's supporting line passes through O - the "radial" case).
// So per edge we only need to:
//   - detect the radial case (edge collinear with O): contributes 0,
//     confirmed by the discuss board ("parallel case treated as
//     invisible" - looking exactly edge-on gives zero visible width,
//     since the near point of such an edge always self-occludes the far
//     part, regardless of any other obstruction).
//   - otherwise, if s is decreasing along the edge: the whole edge is
//     invisible (s(t) <= s(0) <= M for all t, matches "can't see the back
//     of a peak from below/in front").
//   - if s is increasing: the edge is visible from the point where s(t)
//     crosses the running max M onward (or fully visible if s already
//     >= M at t=0, or fully invisible if s never reaches M).
//
// The crossing point is found by intersecting the edge with the line
// y = h + M*x (the ray from O through whichever vertex currently attains
// the running max).
//
// The board's post 174274 also shows the judge accepts non-integer
// coordinates despite the statement claiming "integers", so everything is
// read as double.
//
// Verified against: the 3 official samples, and two board-posted cases
// (7 6 / 0 0 2 5 3 2 4 3 100 1 101 6 102 0 -> 104.42; and wizmann's
// decimal-coordinate case -> 10.53), all matching exactly.

#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    int n;
    double h;
    while (scanf("%d %lf", &n, &h) == 2) {
        if (n == 0 && h == 0) break;
        static double x[1005], y[1005];
        for (int i = 0; i < n; ++i) scanf("%lf %lf", &x[i], &y[i]);

        double total = 0.0;
        double M = 0.0;
        bool haveM = false; // false means M = -infinity (nothing blocks yet)

        for (int i = 0; i + 1 < n; ++i) {
            double x1 = x[i], y1 = y[i], x2 = x[i+1], y2 = y[i+1];
            double len = hypot(x2 - x1, y2 - y1);
            double s1 = (fabs(x1) < 1e-9) ? 0.0 : (y1 - h) / x1;
            double s2 = (y2 - h) / x2; // x2 > 0 always (strictly increasing x, x1>=0)

            bool firstFromObserver = (i == 0 && fabs(x1) < 1e-9);

            if (firstFromObserver) {
                // M is -infinity: entire edge is visible.
                total += len;
                M = s2; haveM = true;
                continue;
            }

            // cross of (V1-O) x (V2-O)
            double cross = x1 * (y2 - h) - (y1 - h) * x2;
            const double EPS = 1e-9;

            if (fabs(cross) < EPS) {
                // radial edge: collinear with O -> invisible regardless.
                // (still update M below)
            } else if (cross > 0) {
                // increasing slope along the edge
                double Mv = haveM ? M : s1; // s1 <= M always when haveM; if !haveM treat as s1 itself (no constraint)
                if (!haveM) {
                    total += len; // nothing blocks yet
                } else if (s2 < Mv - EPS) {
                    // never reaches threshold: fully invisible
                } else if (s1 >= Mv - EPS) {
                    // already at/above threshold from the start: fully visible
                    total += len;
                } else {
                    // partial: find crossing t*
                    double num = h + Mv * x1 - y1;
                    double den = (y2 - y1) - Mv * (x2 - x1);
                    double t = num / den;
                    if (t < 0) t = 0;
                    if (t > 1) t = 1;
                    total += (1.0 - t) * len;
                }
            } else {
                // decreasing slope: entirely invisible (self-shadowed by V1)
            }

            if (!haveM || s2 > M) { M = s2; haveM = true; }
        }

        printf("%.2f\n", total);
    }
    return 0;
}
