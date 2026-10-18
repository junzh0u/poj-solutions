// POJ 3103 - Cutting a Block
// Model: gpt-6-astra
// Cut perpendicular to the x axis into n congruent rectangular slabs.
// Any valid partition is allowed; the sample partitions are not unique.
// Compute each boundary directly to avoid accumulated rounding error.
#include <cstdio>
int main() {
    int x, y, z, n;
    if (std::scanf("%d%d%d%d", &x, &y, &z, &n) != 4) return 0;
    for (int i = 0; i < n; ++i)
        std::printf("%.8f %.8f %.8f %.8f %.8f %.8f\n",
                    double(x) * i / n, 0.0, 0.0,
                    double(x) * (i + 1) / n, double(y), double(z));
    return 0;
}
