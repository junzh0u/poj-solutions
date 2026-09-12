// POJ 3831 - Open-air shopping malls
// Model: claude-sonnet-5
//
// For each candidate umbrella center (must coincide with one of the mall
// centers), and for every mall, binary-search the minimum umbrella radius r
// such that the circle-circle intersection (lens) area between the umbrella
// (center O, radius r) and that mall (center C_i, radius R_i) is at least
// half of the mall's own area (pi*R_i^2/2). The umbrella must satisfy every
// mall simultaneously, so take the max over malls for a fixed center, then
// minimize that over the choice of center.
//
// The statement guarantees the malls' circles are pairwise disjoint and that
// none contains another, but the discuss board (message 166123) reports the
// actual judge data violates that (one circle contained in another). The
// general two-circle intersection-area formula used here (with the standard
// d<=|r-R| containment branch) does not rely on the malls' circles being
// disjoint from each other at all -- it only needs the pairwise distance
// between the (fixed) umbrella center and each mall center, so containment
// among the malls' own circles never enters the computation and this
// solution is unaffected by that data quirk.
#include <cstdio>
#include <cmath>
using namespace std;

static const double PI = 3.14159265358979323846;

static double clampd(double x, double lo, double hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

// Area of intersection of circle(radius r) and circle(radius R) whose
// centers are distance d apart.
static double lensArea(double d, double r, double R) {
    if (d >= r + R) return 0.0;
    if (d <= fabs(r - R)) {
        double m = (r < R) ? r : R;
        return PI * m * m;
    }
    double a1 = acos(clampd((d * d + r * r - R * R) / (2 * d * r), -1.0, 1.0));
    double a2 = acos(clampd((d * d + R * R - r * r) / (2 * d * R), -1.0, 1.0));
    double t = (-d + r + R) * (d + r - R) * (d - r + R) * (d + r + R);
    if (t < 0) t = 0;
    double part3 = 0.5 * sqrt(t);
    return r * r * a1 + R * R * a2 - part3;
}

// Minimum radius r (umbrella centered at distance d from mall center, mall
// radius R) so that the intersection area is at least half of the mall's
// own area.
static double neededR(double d, double R) {
    double target = 0.5 * PI * R * R;
    double lo = 0.0, hi = d + R + 1.0;
    for (int iter = 0; iter < 100; iter++) {
        double mid = (lo + hi) / 2.0;
        if (lensArea(d, mid, R) >= target) hi = mid; else lo = mid;
    }
    return hi;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T--) {
        int n;
        scanf("%d", &n);
        static double x[25], y[25], r[25];
        for (int i = 0; i < n; i++) scanf("%lf %lf %lf", &x[i], &y[i], &r[i]);
        double ans = 1e18;
        for (int c = 0; c < n; c++) {
            double need = 0.0;
            for (int i = 0; i < n; i++) {
                double dx = x[c] - x[i], dy = y[c] - y[i];
                double d = sqrt(dx * dx + dy * dy);
                double ri = neededR(d, r[i]);
                if (ri > need) need = ri;
            }
            if (need < ans) ans = need;
        }
        printf("%.4f\n", ans);
    }
    return 0;
}
