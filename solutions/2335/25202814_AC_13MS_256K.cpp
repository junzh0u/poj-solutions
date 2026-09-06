// POJ 2335 - Temple of Dune
// Model: gpt-5.6-terra
// Find the circumcenter, express the three points as polar angles, and take
// the floating-point gcd of their angular differences and one full turn.
// The resulting angle is the largest regular-polygon step consistent with
// all three vertices, so 2*pi divided by it is the fewest vertex count.
// The official Waterloo reference uses a 6e-3 remainder tolerance because
// coordinates are rounded decimal input.
#include <cmath>
#include <cstdio>

const double PI = 2.0 * acos(0.0);
const double EPS = 6e-3;

double gcd_angle(double a, double b) {
    if (fabs(b) < EPS) return a;
    return gcd_angle(b, fmod(a, b));
}

int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        double x[3], y[3];
        for (int i = 0; i < 3; ++i) scanf("%lf%lf", &x[i], &y[i]);

        double a1 = 2.0 * (x[1] - x[0]);
        double b1 = 2.0 * (y[1] - y[0]);
        double c1 = x[1] * x[1] + y[1] * y[1]
                  - x[0] * x[0] - y[0] * y[0];
        double a2 = 2.0 * (x[2] - x[1]);
        double b2 = 2.0 * (y[2] - y[1]);
        double c2 = x[2] * x[2] + y[2] * y[2]
                  - x[1] * x[1] - y[1] * y[1];
        double det = a1 * b2 - b1 * a2;
        double cx = (c1 * b2 - b1 * c2) / det;
        double cy = (a1 * c2 - c1 * a2) / det;

        double angle[3];
        for (int i = 0; i < 3; ++i)
            angle[i] = atan2(x[i] - cx, y[i] - cy);
        double step = gcd_angle(fabs(angle[0] - angle[1]), 2.0 * PI);
        step = gcd_angle(step, fabs(angle[1] - angle[2]));
        printf("%.0f\n", 2.0 * PI / step);
    }
    return 0;
}
