// POJ 2124 - Crankshaft
// Model: claude-sonnet-5
// Approach: crankshaft = stack of N parallel polygon plates (equal
// material/thickness) whose projections onto OXY overlap without their
// masses overlapping, since the plates sit in distinct parallel planes.
// So total center of mass = area-weighted average of each polygon's own
// centroid, weight = |signed area|. Verified vs sample: areas 4 and 1,
// centroids (0,0) and (-0.5,-0.5) -> (-0.1,-0.1), matches exactly.
// Ambiguity: vertices given clockwise (negative shoelace signed area);
// the centroid formula Cx=(1/6A)*sum((xi+xj)*cross_i) still works
// unchanged since dividing by the (negative) signed area self-corrects,
// so only the mass weight needs fabs().
#include <cstdio>
#include <cmath>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    double totalArea = 0.0, sumCx = 0.0, sumCy = 0.0;
    for (int p = 0; p < n; ++p) {
        int v;
        scanf("%d", &v);
        static double x[205], y[205];
        for (int i = 0; i < v; ++i) {
            scanf("%lf %lf", &x[i], &y[i]);
        }
        double area2 = 0.0;
        double cx = 0.0, cy = 0.0;
        for (int i = 0; i < v; ++i) {
            int j = (i + 1) % v;
            double cross = x[i] * y[j] - x[j] * y[i];
            area2 += cross;
            cx += (x[i] + x[j]) * cross;
            cy += (y[i] + y[j]) * cross;
        }
        double area = fabs(area2) / 2.0;
        cx /= 3.0 * area2;
        cy /= 3.0 * area2;
        totalArea += area;
        sumCx += cx * area;
        sumCy += cy * area;
    }
    printf("%.4f %.4f\n", sumCx / totalArea, sumCy / totalArea);
    return 0;
}
