// POJ 2976 - Dropping tests
//
// Choose which k of the n tests to drop to maximize the cumulative average
// (sum of kept a_i) / (sum of kept b_i). Greedy by individual ratio, or by
// dropping the largest denominators, both fail because the objective is a
// weighted average, not a simple per-item score.
//
// Standard trick: binary search on the answer x. For a candidate x, define
// d_i = a_i - x*b_i for every test. A subset of size n-k has average >= x
// iff the sum of its d_i is >= 0, and for a fixed subset size that sum is
// maximized by keeping the n-k tests with the largest d_i (each d_i is
// independent of which other tests are chosen). So feasibility of x is
// checked by sorting d_i descending and summing the top n-k: if that sum
// is >= 0, x is achievable and we can raise the search's lower bound,
// otherwise lower the upper bound. After enough halvings (60, far beyond
// double's ~52 bits of precision) the lower bound converges to the true
// maximum ratio, which is then printed as a rounded percentage.
//
// The statement guarantees the true answer is never within 0.001 of a
// x.5 rounding boundary, so plain printf("%.0f") rounding is safe -
// verified with an exhaustive drop-k brute force for n <= 10 (flagging
// and excluding artificial exact-half ties that only arise from small
// hand-picked integers, which do round differently under printf's
// round-half-to-even vs. round-half-up, but never occur in judge data).
#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1000;
double a[MAXN], b[MAXN], d[MAXN];
int n, k;

bool cmpDesc(double x, double y) { return x > y; }

int main() {
    while (scanf("%d %d", &n, &k) == 2 && (n || k)) {
        for (int i = 0; i < n; i++) scanf("%lf", &a[i]);
        for (int i = 0; i < n; i++) scanf("%lf", &b[i]);

        double lo = 0.0, hi = 1.0;
        for (int iter = 0; iter < 60; iter++) {
            double mid = (lo + hi) / 2.0;
            for (int i = 0; i < n; i++) d[i] = a[i] - mid * b[i];
            sort(d, d + n, cmpDesc);
            double sum = 0.0;
            for (int i = 0; i < n - k; i++) sum += d[i];
            if (sum >= 0.0) lo = mid; else hi = mid;
        }
        printf("%.0f\n", lo * 100.0);
    }
    return 0;
}
