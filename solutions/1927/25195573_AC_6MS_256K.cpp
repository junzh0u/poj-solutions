// POJ 1927 - Area in Triangle
// Model: claude-opus-5
//
// A closed rope of length L lies inside a triangle T; maximize the enclosed area.
// The rope is the whole boundary of the region (it costs length even where it runs
// along a side of the field), which is what the samples pin down.
//
// The optimal region is T eroded by r and then dilated by r: a triangle homothetic
// to T with its three corners rounded by arcs of radius r. Its straight parts lie on
// T's sides and its three arcs turn by the exterior angles, which sum to 2*pi, so by
// Steiner's formulae
//     L(r) = P*(1 - r/rin) + 2*pi*r      area(r) = A - A*r^2/rin^2 + pi*r^2
// with P the perimeter, A the area and rin = A/s the inradius. L(r) is linear and
// decreasing (P/rin = 2s^2/A >= 2*pi), so r follows in closed form; no search needed.
//
// Three regimes, and the middle one is the case the official sample never reaches:
//   L >= P            the whole field
//   L <= 2*pi*rin     r would exceed rin, the eroded triangle is empty: a free circle
//                     of radius L/(2*pi), which fits since that radius is <= rin
//   otherwise         r = (P - L) / (P/rin - 2*pi)
// All three samples land in the last two branches, so a program missing the circle
// branch reproduces the sample exactly; the discuss board reports that as the usual
// wrong answer here. Boundary cases built for it (e.g. 100 100 100 100 -> 795.77)
// separate them, and the two branches agree at L = 2*pi*rin.
//
// Input framing: several data sets, one per line, ended by a line of four zeros
// (written "0 0 0 0" in the sample, not "0.0000 ..."), so read as doubles and also
// stop at EOF in case the terminator is absent. L is promised to be at most P, but
// the L >= P branch keeps a violation harmless rather than producing a negative r.

#include <cstdio>
#include <cmath>

int main() {
    const double PI = 2.0 * asin(1.0);
    double a, b, c, L;
    int cs = 0;
    while (scanf("%lf %lf %lf %lf", &a, &b, &c, &L) == 4) {
        if (a == 0.0 && b == 0.0 && c == 0.0 && L == 0.0) break;
        double P = a + b + c;
        double s = P / 2.0;
        double t = s * (s - a) * (s - b) * (s - c);
        if (t < 0.0) t = 0.0;
        double A = sqrt(t);
        double ans;
        if (A <= 0.0) {
            ans = 0.0;
        } else {
            double rin = A / s;
            if (L >= P) {
                ans = A;
            } else if (L <= 2.0 * PI * rin) {
                double r = L / (2.0 * PI);
                ans = PI * r * r;
            } else {
                double r = (P - L) / (P / rin - 2.0 * PI);
                ans = A - A * r * r / (rin * rin) + PI * r * r;
            }
        }
        if (ans <= 0.0) ans = 0.0;
        printf("Case %d: %.2f\n", ++cs, ans);
    }
    return 0;
}
