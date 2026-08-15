// POJ 2715 - Paint Mix
// Model: claude-sonnet-5
//
// Track x_n = deviation of black-paint amount in the black pail from its
// equilibrium value.  Because total black paint is conserved, the pour
// process is an affine map on (b_n, w_n) with one eigenvalue 1 (the
// conserved total) and one eigenvalue r < 1; x_n decays geometrically as
// x_n = x0 * r^n with a closed form for r.  The black-pail ratio exceeds
// B/W by an amount that grows monotonically with x_n (and is 0 in the
// limit), the white-pail ratio falls short of B/W the same way, so each
// tolerance condition reduces to a linear threshold on x_n; solving both
// exactly gives thresholds x1 (black pail) and x2 (white pail), and the
// answer is the least n with x0*r^n < min(x1,x2).  Solved via logarithms
// then nudged by direct verification to kill any boundary rounding.
//
// Statement requires EACH pail's ratio to be within tolerance (confirmed
// on the discuss board, message 136193, after a WA from checking only
// one pail) -- both conditions must hold simultaneously.
#include <cstdio>
#include <cmath>

int main() {
    int B, W, C;
    while (scanf("%d %d %d", &B, &W, &C) == 3) {
        if (B == 0 && W == 0 && C == 0) break;

        long double Vb = 16.0L * B;
        long double Vw = 16.0L * W;
        long double Cc = (long double)C;

        long double r = (1.0L - Cc / Vb) * (Vw / (Vw + Cc));

        long double sum = (long double)(B + W);
        long double d = Vb * W / sum; // = x0
        long double e = Vw * W / sum;
        long double x0 = d;
        long double T = (long double)B / (long double)W;
        long double tol = 1e-5L;

        long double x1 = tol * d / (1.0L + T + tol);
        long double x2 = tol * e / (1.0L + T - tol);
        long double xth = (x1 < x2) ? x1 : x2;

        long double L = logl(xth / x0) / logl(r);
        long n = (long)floorl(L) + 1;
        if (n < 1) n = 1;

        // Verify / nudge to kill any boundary floating error.
        // condition(n): x0 * r^n < xth
        for (;;) {
            long double xn = x0 * powl(r, (long double)n);
            if (xn < xth) break;
            n++;
        }
        while (n > 1) {
            long double xn_prev = x0 * powl(r, (long double)(n - 1));
            if (xn_prev < xth) n--; else break;
        }

        printf("%ld\n", n);
    }
    return 0;
}
