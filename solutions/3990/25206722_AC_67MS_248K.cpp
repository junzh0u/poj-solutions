// POJ 3990 - Fermat Point in Quadrangle
// Model: claude-sonnet-5
// Approach: The "quadrangle Fermat point" is just the geometric median of the
// four given points (minimizing the sum of Euclidean distances); the order of
// the vertices and whether they form a convex/concave/degenerate quadrilateral
// is irrelevant to this minimization. The sum-of-distances function is convex,
// so we find its minimum with a deterministic pattern-search / hill-climbing
// scheme: start at the centroid, repeatedly try moving by a fixed step in 8
// directions, take any improving move, and otherwise shrink the step. This is
// the standard technique for Fermat-point style problems (cf. POJ 2420) and
// converges to the true minimum regardless of point configuration, including
// coincident or collinear points.
#include <cstdio>
#include <cmath>
using namespace std;

static double px[4], py[4];

double totalDist(double cx, double cy) {
    double s = 0.0;
    for (int i = 0; i < 4; i++) {
        double dx = cx - px[i];
        double dy = cy - py[i];
        s += sqrt(dx * dx + dy * dy);
    }
    return s;
}

int main() {
    static const double DX[8] = {1, -1, 0, 0, 1, 1, -1, -1};
    static const double DY[8] = {0, 0, 1, -1, 1, -1, 1, -1};

    while (true) {
        bool allEnd = true;
        double x[4], y[4];
        for (int i = 0; i < 4; i++) {
            if (scanf("%lf %lf", &x[i], &y[i]) != 2) return 0;
            if (x[i] != -1.0 || y[i] != -1.0) allEnd = false;
        }
        if (allEnd) break;
        for (int i = 0; i < 4; i++) { px[i] = x[i]; py[i] = y[i]; }

        double cx = 0, cy = 0;
        for (int i = 0; i < 4; i++) { cx += px[i]; cy += py[i]; }
        cx /= 4.0; cy /= 4.0;

        double cur = totalDist(cx, cy);
        double step = 1024.0;
        while (step > 1e-10) {
            bool improved = false;
            for (int i = 0; i < 8; i++) {
                double nx = cx + DX[i] * step;
                double ny = cy + DY[i] * step;
                double nd = totalDist(nx, ny);
                if (nd < cur - 1e-13) {
                    cur = nd;
                    cx = nx; cy = ny;
                    improved = true;
                }
            }
            if (!improved) step *= 0.7;
        }
        printf("%.4f\n", cur);
    }
    return 0;
}
