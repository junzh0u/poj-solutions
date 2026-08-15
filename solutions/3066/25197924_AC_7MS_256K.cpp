// POJ 3066 - Maximum
// Model: claude-sonnet-5
// x_1..x_m real, -1/sqrt(a) <= x_i <= sqrt(a), sum x_i = b*sqrt(a). Maximize
// sum x_i^p, p even. x^p is convex, so the max over {box} intersect {one
// linear equality} is at a vertex: at most one variable is free, the rest
// pinned to L or U. Enumerate how many of the pinned ones sit at U; solve
// the (at most one) free variable from the equality constraint; take the
// best feasible choice. Exact boundary case (0 free variables) handled
// separately in exact arithmetic to avoid amplifying sqrt's rounding error
// through pow. Bound is ASYMMETRIC per the problem's image (not symmetric).
#include <cstdio>
#include <cmath>
using namespace std;

static double ipow(double base, int e) {
    double r = 1.0;
    for (int i = 0; i < e; ++i) r *= base;
    return r;
}

int main() {
    int m, p, a, b;
    while (scanf("%d %d %d %d", &m, &p, &a, &b) == 4) {
        double A = (double)a;
        double U = sqrt(A);
        double L = -1.0 / U;
        double S = (double)b * U;

        double Up = ipow(A, p / 2);
        double Lp = 1.0 / Up;

        double eps = 1e-6 * (1.0 + fabs(U));
        double best = -1e300;
        bool found = false;

        for (int k = 0; k <= m; ++k) {
            int rest = m - k;
            double sum = (double)k * U + (double)rest * L;
            if (sum < S - eps || sum > S + eps) continue;
            double total = (double)k * Up + (double)rest * Lp;
            if (!found || total > best) {
                best = total;
                found = true;
            }
        }

        for (int j = 0; j <= m - 1; ++j) {
            int rest = (m - 1) - j;
            double fixedSum = (double)j * U + (double)rest * L;
            double freeX = S - fixedSum;
            if (freeX < L - eps || freeX > U + eps) continue;
            if (freeX <= L + eps || freeX >= U - eps) continue;
            double total = (double)j * Up + (double)rest * Lp + ipow(freeX, p);
            if (!found || total > best) {
                best = total;
                found = true;
            }
        }

        printf("%.0f\n", best);
    }
    return 0;
}
