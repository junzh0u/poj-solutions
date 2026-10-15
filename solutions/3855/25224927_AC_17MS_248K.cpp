// POJ 3855 - Blast the Enemy!
// Model: gpt-6.1-sol
// Sum signed triangle areas and first moments along the polygon boundary.
// Signed contributions handle concavity and either vertex orientation.
#include <cstdio>
#include <cmath>
int main() {
    int n, stage = 0;
    while (std::scanf("%d", &n) == 1 && n) {
        int x[101], y[101];
        for (int i = 0; i < n; ++i) std::scanf("%d%d", &x[i], &y[i]);
        x[n] = x[0]; y[n] = y[0];
        int area = 0, mx = 0, my = 0;
        for (int i = 0; i < n; ++i) {
            int cross = x[i] * y[i+1] - x[i+1] * y[i];
            area += cross;
            mx += (x[i] + x[i+1]) * cross;
            my += (y[i] + y[i+1]) * cross;
        }
        double cx = double(mx) / (3.0 * area);
        double cy = double(my) / (3.0 * area);
        if (std::fabs(cx) < 0.0000005) cx = 0;
        if (std::fabs(cy) < 0.0000005) cy = 0;
        std::printf("Stage #%d: %.6f %.6f\n", ++stage, cx, cy);
    }
    return 0;
}
