// POJ 2507 - Crossed ladders
// Model: claude-sonnet-5
// Approach: for street width w, the two ladders (lengths x and y) reach heights
// h1 = sqrt(x^2 - w^2) and h2 = sqrt(y^2 - w^2) on the opposing walls; they cross
// at height h1*h2/(h1+h2). As w grows from 0 to min(x,y), h1 and h2 both shrink,
// so this crossing height decreases monotonically from xy/(x+y) down to 0 -- a
// unique root exists for any valid c. Binary search on w over [0, min(x,y)] for a
// FIXED number of iterations (not an epsilon-terminated while loop): the discuss
// board reports that comparing against a tight epsilon can loop forever on some
// inputs due to floating point rounding, which reads as TLE.
#include <cstdio>
#include <cmath>

int main() {
    double x, y, c;
    while (scanf("%lf %lf %lf", &x, &y, &c) == 3) {
        double lo = 0.0, hi = (x < y ? x : y);
        for (int iter = 0; iter < 200; ++iter) {
            double mid = (lo + hi) / 2.0;
            double h1 = sqrt(x * x - mid * mid);
            double h2 = sqrt(y * y - mid * mid);
            double cc = h1 * h2 / (h1 + h2);
            if (cc > c) lo = mid; else hi = mid;
        }
        printf("%.3f\n", (lo + hi) / 2.0);
    }
    return 0;
}
