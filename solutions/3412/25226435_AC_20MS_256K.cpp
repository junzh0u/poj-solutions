// POJ 3412 - Ray
// Model: gpt-6-astra
// Snell's law preserves N*sin(angle). Test every slice for total
// reflection, then recover the final angle directly to avoid accumulated error.
// Equality is allowed by the stated formula; clamp roundoff at asin's boundary.
#include <cstdio>
#include <cmath>
#include <algorithm>
int main() {
    int k;
    if (scanf("%d", &k) != 1) return 0;
    double n[100], a;
    for (int i = 0; i < k; ++i) scanf("%lf", &n[i]);
    scanf("%lf", &a);
    double p = n[0] * sin(a);
    for (int i = 1; i < k; ++i) {
        if (p > n[i] * (1.0 + 1e-12)) {
            puts("NO");
            return 0;
        }
    }
    double answer = k == 1 ? a : asin(std::min(1.0, p / n[k-1]));
    printf("%.10g\n", answer);
    return 0;
}
