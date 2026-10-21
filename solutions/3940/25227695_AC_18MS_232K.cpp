// POJ 3940 - Grey Area
// Model: gpt-6-astra
// Count fixed-width intervals, retaining empty intervals from zero onward.
// Normalize heights by the maximum count and darkness by the last index.
// Even the white rightmost bar participates in determining maximum height.
#include <cstdio>
#include <algorithm>
int main() {
    int n, w;
    while (std::scanf("%d%d", &n, &w) == 2 && (n || w)) {
        int count[101] = {0}, last = 0, height = 0;
        for (int i = 0; i < n; ++i) {
            int v;
            std::scanf("%d", &v);
            int bin = v / w;
            ++count[bin];
            last = std::max(last, bin);
            height = std::max(height, count[bin]);
        }
        int weighted = 0;
        for (int i = 0; i <= last; ++i)
            weighted += count[i] * (last - i);
        double ink = 0.01 + double(weighted) / (last * height);
        std::printf("%.12f\n", ink);
    }
    return 0;
}
