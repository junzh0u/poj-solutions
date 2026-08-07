// POJ 2926 - Requirements
// Model: claude-opus-5
//
// Maximum Manhattan (L1) distance between any two of N points in 5 dimensions.
//
// |x_i - y_i| = max over s_i in {+1,-1} of s_i*(x_i - y_i), so
//   sum_i |x_i - y_i| = max over s in {+1,-1}^5 of ( <s,x> - <s,y> ).
// Taking the max over all pairs (X,Y) and all sign vectors s commutes, so the
// answer is  max_s ( max_p <s,p> - min_p <s,p> ).  Each wrong sign choice only
// underestimates the true |.| sum, so no combination can overshoot the answer.
// That is 32 sign vectors (16 by the s/-s symmetry, which leaves max-min
// unchanged; 32 is kept for clarity) and one streaming pass: O(32*5*N).
//
// Notes:
//  - The discuss board reports negative coordinates in the judge data even
//    though the statement says "non-negative", so the running max/min start at
//    +/-1e300 rather than 0.
//  - Input is a single data set (leading N), not read to EOF.
//  - Values are read with %lf into double; float loses the needed precision.

#include <cstdio>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    double best[32], worst[32];
    for (int m = 0; m < 32; ++m) {
        best[m] = -1e300;
        worst[m] = 1e300;
    }

    for (int i = 0; i < n; ++i) {
        double v[5];
        for (int j = 0; j < 5; ++j)
            if (scanf("%lf", &v[j]) != 1) v[j] = 0.0;
        for (int m = 0; m < 32; ++m) {
            double s = 0.0;
            for (int j = 0; j < 5; ++j)
                s += (m & (1 << j)) ? v[j] : -v[j];
            if (s > best[m]) best[m] = s;
            if (s < worst[m]) worst[m] = s;
        }
    }

    double ans = 0.0;
    for (int m = 0; m < 32; ++m) {
        double d = best[m] - worst[m];
        if (d > ans) ans = d;
    }

    printf("%.2f\n", ans);
    return 0;
}
