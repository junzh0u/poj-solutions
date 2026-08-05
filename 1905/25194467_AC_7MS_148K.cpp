// POJ 1905 - Expanding Rods
//
// A rod of length L heated by n degrees becomes L' = (1 + n*C) * L and bows into
// a circular arc whose chord is still the original L (the walls do not move).
// With 2t the angle the arc subtends at the centre and R the radius:
//
//     chord L  = 2*R*sin(t)
//     arc   L' = 2*R*t
//
// Dividing kills R and leaves one transcendental equation in t alone:
//
//     t / sin(t) = L' / L = 1 + n*C
//
// t/sin(t) rises monotonically from 1 at t->0 to +inf at t->pi, so bisect on
// t over (0, pi). The statement caps the expansion at half the original length,
// i.e. the ratio at 1.5, which puts t below 1.497 rad — comfortably inside the
// bracket. 100 iterations exhaust double precision (pi / 2^100).
//
// The displacement of the rod's centre is the sagitta:
//
//     h = R - R*cos(t) = L*(1 - cos t) / (2 sin t) = (L/2) * tan(t/2)
//
// The tan(t/2) form is used because it stays well-conditioned as t -> 0, where
// (1-cos t)/sin t would cancel catastrophically.
//
// Statement details worth pinning down:
//   * n = 0 (or C = 0) is a real input — the sample's third line has it. There
//     is no bowing, t collapses to 0 and the equation degenerates (sin t = 0 in
//     the denominator), so the ratio <= 1 case is short-circuited to 0.000
//     before the search ever runs. L = 0 is guarded the same way.
//   * Input terminates on a line of three negative numbers, not at EOF, so the
//     loop breaks on any negative field (all real inputs are non-negative).
//   * "3 digits of precision" means three digits after the decimal point, as
//     the sample's 61.329 / 225.020 / 0.000 shows.
//
// Verified against an independently derived formulation (bisect on the sagitta
// h itself, recovering R = (h^2 + L^2/4) / 2h and the arc from atan2) over
// 200000 random (L, ratio) pairs across the full legal range: byte-identical
// output at three decimals.

#include <cstdio>
#include <cmath>

int main() {
    double L, n, C;
    while (scanf("%lf %lf %lf", &L, &n, &C) == 3) {
        if (L < 0 || n < 0 || C < 0) break;
        double s = 1.0 + n * C;          /* L'/L */
        if (L <= 0.0 || s <= 1.0) { printf("0.000\n"); continue; }
        /* arc = 2*R*t, chord = 2*R*sin(t)  =>  t/sin(t) = s, t in (0, pi) */
        double lo = 0.0, hi = 3.14159265358979323846;
        for (int i = 0; i < 100; ++i) {
            double t = (lo + hi) * 0.5;
            if (t / sin(t) < s) lo = t; else hi = t;
        }
        double t = (lo + hi) * 0.5;
        /* displacement = R - R cos t = (L/2) * tan(t/2) */
        printf("%.3f\n", L * 0.5 * tan(t * 0.5));
    }
    return 0;
}
