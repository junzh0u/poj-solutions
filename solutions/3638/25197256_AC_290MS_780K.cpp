// POJ 3638 - Moogle
// Model: gpt-5.6-terra
// Dynamic programming over the stored house numbers.  For every possible
// segment, compute its total interpolation error directly from a scaled
// numerator, avoiding a separately rounded interpolated coordinate.

#include <cmath>
#include <cstdio>

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int h, c;
        std::scanf("%d%d", &h, &c);
        double x[200];
        int i, j, k, used;
        for (i = 0; i < h; ++i) std::scanf("%lf", &x[i]);

        double cost[200][200];
        for (i = 0; i < h; ++i) {
            for (j = 0; j < h; ++j) cost[i][j] = 0.0;
        }
        for (i = 0; i < h; ++i) {
            for (j = i + 1; j < h; ++j) {
                double sum = 0.0;
                double gap = (double)(j - i);
                for (k = i + 1; k < j; ++k) {
                    double scaled = x[k] * gap - x[i] * (j - k)
                        - x[j] * (k - i);
                    sum += std::fabs(scaled) / gap;
                }
                cost[i][j] = sum;
            }
        }

        const double inf = 1e100;
        double dp[201][200];
        for (used = 0; used <= c; ++used) {
            for (j = 0; j < h; ++j) dp[used][j] = inf;
        }
        dp[1][0] = 0.0;
        for (used = 2; used <= c; ++used) {
            for (j = used - 1; j < h; ++j) {
                for (i = used - 2; i < j; ++i) {
                    double candidate = dp[used - 1][i] + cost[i][j];
                    if (candidate < dp[used][j]) dp[used][j] = candidate;
                }
            }
        }
        std::printf("%.4f\n", dp[c][h - 1] / h);
    }
    return 0;
}
