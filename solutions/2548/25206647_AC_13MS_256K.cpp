// POJ 2548 - Subway
// Model: claude-sonnet-5
//
// Jerk-limited point-to-point motion: minimize time to travel distance d,
// starting and ending at rest, with |v|<=m, |a|<=amax, |da/dt|<=j.
// The time-optimal profile is the classic symmetric S-curve: an
// acceleration ramp (jerk up, possibly constant accel, jerk down) up to a
// peak velocity v_peak<=m, an optional cruise at v_peak, then the mirror
// image deceleration ramp back to rest.
//
// For a ramp reaching peak velocity vp:
//  - if vp <= amax^2/j, the acceleration never saturates: pure triangular
//    jerk profile with half-duration tau=sqrt(vp/j), one-way distance
//    s(vp) = vp^1.5/sqrt(j), one-way time t(vp) = 2*tau.
//  - otherwise accel saturates at amax for ta=(vp-amax^2/j)/amax between
//    the two jerk phases (each of duration amax/j); one-way distance
//    s(vp) = amax^3/j^2 + 1.5*amax^2/j*ta + 0.5*amax*ta^2,
//    one-way time t(vp) = 2*amax/j + ta.
//
// Algorithm: compute s(m); if 2*s(m) <= d there is a cruise phase of
// length (d-2*s(m))/m at speed m. Otherwise m is never reached: solve for
// the peak velocity (equivalently ta, or tau) that makes 2*s(vp) = d,
// using the triangular formula while d <= 2*amax^3/j^2 and the quadratic
// (in ta) formula otherwise, then report the resulting one-way time
// doubled.
//
// Verified against the sample (1000 70 20 1 -> 31.7) and against a
// numerical bang-bang jerk simulator over randomized parameters.
#include <cstdio>
#include <cmath>

int main() {
    int di, mi, ai, ji;
    while (scanf("%d %d %d %d", &di, &mi, &ai, &ji) == 4) {
        double d = di, m = mi, a = ai, j = ji;
        double av2 = a * a / j; // velocity at which accel ramp alone reaches amax

        // one-way distance/time to reach peak velocity vp
        // returns via references
        double sM, tM;
        if (m <= av2) {
            double tau = sqrt(m / j);
            sM = m * sqrt(m) / sqrt(j);
            tM = 2 * tau;
        } else {
            double tj = a / j;
            double ta = (m - av2) / a;
            sM = a * a * a / (j * j) + 1.5 * a * a / j * ta + 0.5 * a * ta * ta;
            tM = 2 * tj + ta;
        }

        double ans;
        if (2 * sM <= d) {
            double tCruise = (d - 2 * sM) / m;
            ans = 2 * tM + tCruise;
        } else {
            double dThresh1 = 2 * a * a * a / (j * j);
            if (d <= dThresh1) {
                double vp = pow(d * sqrt(j) / 2.0, 2.0 / 3.0);
                double tau = sqrt(vp / j);
                ans = 4 * tau;
            } else {
                double A = a;
                double B = 3 * a * a / j;
                double C = 2 * a * a * a / (j * j) - d;
                double disc = B * B - 4 * A * C;
                double ta = (-B + sqrt(disc)) / (2 * A);
                ans = 4 * (a / j) + 2 * ta;
            }
        }
        printf("%.1f\n", ans);
    }
    return 0;
}
