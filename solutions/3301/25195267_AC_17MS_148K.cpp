// POJ 3301 - Texas Trip
// Model: claude-opus-5
//
// Smallest square (any orientation) covering all points.  Rotating the plane by
// t, the smallest covering square with that orientation has side
//     f(t) = max(width(t), height(t)),
// so the answer is min over t in [0, pi/2) of f(t)^2.
//
// f is NOT unimodal on [0, pi/2] -- the discuss board is full of people noting
// that ternary search is wrong here and only passes because the data is weak.
// This solution enumerates candidate angles exactly instead.
//
// width(t) = max_i p_i.u(t) - min_i p_i.u(t).  The argmax/argmin can only
// change at a t where two points tie, i.e. where u(t) is perpendicular to some
// p_i - p_j.  Every pair therefore yields a breakpoint angle, and reducing them
// mod pi/2 also captures the breakpoints of the t+pi/2, t+pi and t+3pi/2
// supports, so the sorted set is a complete superset of f's breakpoints.
//
// Strictly inside one such interval both extents are single sinusoids of t:
//     w1(t) = C1 cos t + D1 sin t,  w2(t) = C2 cos t + D2 sin t
// (fixed extremal pairs, read off at the interval midpoint).  A sinusoid that
// stays positive has no interior local minimum -- its only local minimum is the
// negative one -- so the minimum of max(w1,w2) inside the interval can only sit
// at the crossing w1 = w2, i.e. tan t = -(C1-C2)/(D1-D2).  Checking every
// interval endpoint plus that one crossing per interval is exhaustive.
//
// O(n^2 log n + n^3) per test with n <= 30: 6ms for T=30 worst case.

#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

static const double PI = 3.14159265358979323846;
static const double HALFPI = PI / 2.0;

int n;
double px[64], py[64];

/* side length of the axis-aligned bounding square in the frame rotated by t */
static double sideAt(double t) {
    double c = cos(t), s = sin(t);
    double lo1 = 1e18, hi1 = -1e18, lo2 = 1e18, hi2 = -1e18;
    for (int i = 0; i < n; ++i) {
        double u = px[i] * c + py[i] * s;   /* along u(t)        */
        double v = -px[i] * s + py[i] * c;  /* along u(t + pi/2) */
        if (u < lo1) lo1 = u;
        if (u > hi1) hi1 = u;
        if (v < lo2) lo2 = v;
        if (v > hi2) hi2 = v;
    }
    double w = hi1 - lo1, h = hi2 - lo2;
    return w > h ? w : h;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        if (scanf("%d", &n) != 1) return 0;
        for (int i = 0; i < n; ++i) {
            int a, b;
            scanf("%d %d", &a, &b);
            px[i] = a; py[i] = b;
        }

        /* Breakpoint angles: the support argmax along u(t) can only change at a
           t where two points tie, i.e. u(t) perpendicular to p_i - p_j.  Taking
           every pair gives a superset of those directions; reducing mod pi/2
           also covers the breakpoints of the t+pi/2, t+pi, t+3pi/2 supports. */
        vector<double> ang;
        ang.push_back(0.0);
        ang.push_back(HALFPI);
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j) {
                double dx = px[i] - px[j], dy = py[i] - py[j];
                if (dx == 0.0 && dy == 0.0) continue;
                double a = atan2(dy, dx);
                a = fmod(a, HALFPI);
                if (a < 0) a += HALFPI;
                ang.push_back(a);
            }
        sort(ang.begin(), ang.end());

        double best = 1e18;
        for (size_t k = 0; k + 1 < ang.size(); ++k) {
            double a = ang[k], b = ang[k + 1];
            double va = sideAt(a);
            if (va < best) best = va;
            double vb = sideAt(b);
            if (vb < best) best = vb;
            if (b - a < 1e-15) continue;

            /* Inside (a,b) both widths are single sinusoids in t:
                 w1(t) = C1 cos t + D1 sin t   (extent along u(t))
                 w2(t) = C2 cos t + D2 sin t   (extent along u(t+pi/2))
               A sinusoid that stays positive has no interior local minimum, so
               the minimum of max(w1,w2) on (a,b) can only be at a crossing. */
            double m = 0.5 * (a + b);
            double c = cos(m), s = sin(m);
            int iu = 0, ju = 0, iv = 0, jv = 0;
            double lo1 = 1e18, hi1 = -1e18, lo2 = 1e18, hi2 = -1e18;
            for (int i = 0; i < n; ++i) {
                double u = px[i] * c + py[i] * s;
                double v = -px[i] * s + py[i] * c;
                if (u < lo1) { lo1 = u; ju = i; }
                if (u > hi1) { hi1 = u; iu = i; }
                if (v < lo2) { lo2 = v; jv = i; }
                if (v > hi2) { hi2 = v; iv = i; }
            }
            double d1x = px[iu] - px[ju], d1y = py[iu] - py[ju];
            double d2x = px[iv] - px[jv], d2y = py[iv] - py[jv];
            double C1 = d1x,  D1 = d1y;   /* d1 . u(t)        */
            double C2 = d2y,  D2 = -d2x;  /* d2 . u(t+pi/2)   */

            /* (C1-C2) cos t + (D1-D2) sin t = 0 */
            double A = C1 - C2, B = D1 - D2;
            if (A == 0.0 && B == 0.0) continue;
            double t = atan2(-A, B); /* tan t = -A/B; both roots agree mod pi/2 */
            t = fmod(t, HALFPI);
            if (t < 0) t += HALFPI;
            if (t > a && t < b) {
                double vt = sideAt(t);
                if (vt < best) best = vt;
            }
        }

        if (best < 0) best = 0;
        printf("%.2f\n", best * best);
    }
    return 0;
}
