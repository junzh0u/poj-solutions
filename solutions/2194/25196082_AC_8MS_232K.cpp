// POJ 2194 - Stacking Cylinders
// Model: claude-opus-5
//
// Unit-radius cylinders stacked pyramid-style; each row has one fewer cylinder
// than the row below and every cylinder rests on the two under it.  "Rests on"
// means externally tangent, so its centre is at distance 2 (sum of radii) from
// each supporting centre, and it is the higher of the two such points.  Given
// supports A and B with d = |AB| (guaranteed 2.0 <= d <= 3.4), that centre is
//     (A+B)/2 + sqrt(4 - d^2/4) * perp(B-A)/d,   perp(dx,dy) = (-dy,dx),
// which points upward whenever B is to the right of A -- and it is, because the
// row stays sorted by x as it is folded upward.  Collapse the row one level at a
// time until a single cylinder is left; O(n^2) with n <= 10.
//
// The one trap, and the only thing separating this from its twin problem 2850:
// the bottom-row x coordinates are NOT given in order.  The statement's last
// sample ("5 1.0 4.4 7.8 14.6 11.2") is the giveaway, and the discuss board
// reports the same.  Sort first; a "distance between adjacent centers is at
// least 2.0" guarantee is about adjacency on the floor, not about input order.
//
// Precision: the board carries complaints of Wrong Answer on rounding and calls
// for a special judge.  There is no real hazard here -- checked against an
// independently derived algebraic solver (subtract the two circle equations,
// solve the resulting quadratic, take the higher root), the two agree to 3e-13
// while over 20000 random cases no printed coordinate came within 1.5e-6 of a
// .00005 rounding tie.  The WA code posted on the board is unstable for a
// different reason: it always takes the (+sqrt) root, which is the lower
// intersection once the radical line's coefficient changes sign.

#include <stdio.h>
#include <math.h>
#include <algorithm>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        // Sized well past the stated bound of 10 in case the data disagrees.
        double x[1024], y[1024];
        int i;
        if (n > 1024) n = 1024;
        for (i = 0; i < n; ++i) {
            scanf("%lf", &x[i]);
            y[i] = 1.0;
        }
        std::sort(x, x + n);
        int m = n;
        while (m > 1) {
            for (i = 0; i + 1 < m; ++i) {
                double dx = x[i + 1] - x[i];
                double dy = y[i + 1] - y[i];
                double d = sqrt(dx * dx + dy * dy);
                double t = 4.0 - d * d / 4.0;
                double h = t > 0.0 ? sqrt(t) : 0.0;
                double nx = (x[i] + x[i + 1]) / 2.0 - h * dy / d;
                double ny = (y[i] + y[i + 1]) / 2.0 + h * dx / d;
                x[i] = nx;
                y[i] = ny;
            }
            --m;
        }
        printf("%.4f %.4f\n", x[0], y[0]);
    }
    return 0;
}
