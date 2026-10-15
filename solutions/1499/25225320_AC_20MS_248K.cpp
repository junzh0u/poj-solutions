// POJ 1499 - Supercomputer Selection, The Sequel
// Model: gpt-6.1-sol
// Sum three tetrahedra for each adjacent-axis, adjacent-year sector.
// The cyclic orientation is fixed by the figure and sample: the mixed
// product uses the later axis i and earlier axis i+1.
#include <cstdio>
#include <cmath>
int main() {
    int n, y;
    if (scanf("%d%d", &n, &y) != 2) return 0;
    double a[10][100];
    double factor = n < 3 ? 0.0 : sin(2.0 * acos(-1.0) / n) / 6.0;
    double best = -1.0;
    int answer = 0;
    for (int number = 1; ; ++number) {
        bool zero = true;
        for (int i = 0; i < n; ++i) {
            if (scanf("%lf", &a[0][i]) != 1) return 0;
            if (a[0][i] != 0.0) zero = false;
        }
        if (zero) break;
        for (int t = 1; t < y; ++t)
            for (int i = 0; i < n; ++i) scanf("%lf", &a[t][i]);
        double sum = 0.0;
        for (int t = 1; t < y; ++t)
            for (int i = 0; i < n; ++i) {
                int j = (i + 1) % n;
                sum += a[t-1][i]*a[t-1][j] + a[t][i]*a[t][j]
                     + a[t][i]*a[t-1][j];
            }
        double volume = sum * factor;
        if (volume > best + 1e-9) { best = volume; answer = number; }
    }
    if (answer) printf("%d %.2f\n", answer, best);
    return 0;
}
