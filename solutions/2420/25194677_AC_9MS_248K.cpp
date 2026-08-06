// POJ 2420 - A Star not a Tree?
// Model: gpt-5.6-terra
// Minimize the sum of Euclidean distances to the hub.  The objective is convex,
// so a shrinking eight-direction pattern search reaches its geometric median.
// The requested total is rounded to the nearest millimetre.
#include <cmath>
#include <cstdio>

struct Point {
    double x;
    double y;
};

int n;
Point point[100];

double total_length(double x, double y) {
    double result = 0.0;
    int i;
    for (i = 0; i < n; ++i) {
        double dx = x - point[i].x;
        double dy = y - point[i].y;
        result += std::sqrt(dx * dx + dy * dy);
    }
    return result;
}

int main() {
    static const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    static const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    double x = 0.0;
    double y = 0.0;
    double step = 10000.0;
    double best;
    int i;

    if (std::scanf("%d", &n) != 1) {
        return 0;
    }
    for (i = 0; i < n; ++i) {
        std::scanf("%lf%lf", &point[i].x, &point[i].y);
        x += point[i].x;
        y += point[i].y;
    }
    x /= n;
    y /= n;
    best = total_length(x, y);

    while (step > 1e-7) {
        bool improved = false;
        for (i = 0; i < 8; ++i) {
            double nx = x + dx[i] * step;
            double ny = y + dy[i] * step;
            double candidate = total_length(nx, ny);
            if (candidate < best) {
                x = nx;
                y = ny;
                best = candidate;
                improved = true;
                break;
            }
        }
        if (!improved) {
            step *= 0.5;
        }
    }

    std::printf("%d\n", static_cast<int>(best + 0.5));
    return 0;
}
