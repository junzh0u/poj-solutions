// POJ 2957 - Planet Hunting
// Model: claude-sonnet-5
//
// Let S be the origin. Planet P orbits S counterclockwise with known period T,
// so if z = P(0) (unknown 2D point), then P(t) = R(beta(t)) * z, where
// R is the ccw rotation matrix and beta(t) = 2*pi*t/T is fully known from the
// input times t=0, k1, k1+k2.
//
// Moon M orbits P on a circle of unknown (but constant) radius, so
// |M_i - P(t_i)| is the same value rm for i=1,2,3. Since rotation preserves
// norm, |M_i - R(beta_i) z|^2 = |M_i|^2 - 2 M_i . (R(beta_i) z) + |z|^2, and the
// |z|^2 term cancels when we equate this expression for two different i's.
// That leaves a LINEAR equation in z=(zx,zy) for each pair, giving a 2x2
// linear system (using i=1 vs i=2, and i=1 vs i=3) that determines z exactly
// -- no need to separately enforce the moon's angular-velocity/ratio
// condition, since the problem guarantees the input comes from a consistent
// physical configuration, so the unique solution of this necessary
// (magnitude-only) subsystem must be the true z.
//
// Answer is r = |z|, rounded to nearest integer.
//
// Ambiguity: none found in the statement; the discuss board (message ids
// 127182, 142601) had no content bearing on the algorithm. Verified the
// derivation reproduces the sample (z=(5,0), r=5) and against a randomized
// simulator that builds M_i from random ground-truth r,phi,rm,omega,theta0.

#include <cstdio>
#include <cmath>

int main() {
    double T, k1, k2;
    while (scanf("%lf %lf %lf", &T, &k1, &k2) == 3) {
        if (T == 0 && k1 == 0 && k2 == 0) break;
        double x1, y1, x2, y2, x3, y3;
        scanf("%lf %lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &x3, &y3);

        double beta2 = 2.0 * M_PI * k1 / T;
        double beta3 = 2.0 * M_PI * (k1 + k2) / T;

        // Rotate M2, M3 by -beta2, -beta3 respectively.
        double c2 = cos(beta2), s2 = sin(beta2);
        double c3 = cos(beta3), s3 = sin(beta3);
        // R(-beta)(x,y) = (x*cos(beta) + y*sin(beta), -x*sin(beta) + y*cos(beta))
        double m2rx = x2 * c2 + y2 * s2;
        double m2ry = -x2 * s2 + y2 * c2;
        double m3rx = x3 * c3 + y3 * s3;
        double m3ry = -x3 * s3 + y3 * c3;

        double sq1 = x1 * x1 + y1 * y1;
        double sq2 = x2 * x2 + y2 * y2;
        double sq3 = x3 * x3 + y3 * y3;

        // (m2r - M1) . z = (sq2 - sq1) / 2
        // (m3r - M1) . z = (sq3 - sq1) / 2
        double a1 = m2rx - x1, b1 = m2ry - y1, rhs1 = (sq2 - sq1) / 2.0;
        double a2 = m3rx - x1, b2 = m3ry - y1, rhs2 = (sq3 - sq1) / 2.0;

        double det = a1 * b2 - a2 * b1;
        double zx, zy;
        if (fabs(det) < 1e-9) {
            // Degenerate (shouldn't happen per problem guarantee); fall back
            // to using whichever single equation is available together with
            // an arbitrary orthogonal choice is unsafe, so just use a
            // least-squares-ish fallback: pick the non-degenerate one alone
            // won't determine z fully. As a last resort, average using both
            // rows via normal equations.
            double A11 = a1*a1+a2*a2, A12=a1*b1+a2*b2, A22=b1*b1+b2*b2;
            double B1 = a1*rhs1+a2*rhs2, B2 = b1*rhs1+b2*rhs2;
            double d2 = A11*A22 - A12*A12;
            if (fabs(d2) < 1e-12) { zx = 0; zy = 0; }
            else { zx = (B1*A22 - B2*A12)/d2; zy = (A11*B2 - A12*B1)/d2; }
        } else {
            zx = (rhs1 * b2 - rhs2 * b1) / det;
            zy = (a1 * rhs2 - a2 * rhs1) / det;
        }

        double r = sqrt(zx * zx + zy * zy);
        int ans = (int)floor(r + 0.5);
        printf("%d\n", ans);
    }
    return 0;
}
