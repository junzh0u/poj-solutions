// POJ 3587 - The Biggest Cake
// Model: gpt-5.6-terra
// For each possible triangle vertex, sort all other points by polar angle.
// The maximum circumradius using that vertex is attained by two consecutive
// rays in this circular order. Evaluate those O(n) candidates with
// R = |AB| |AC| |BC| / (2 |AB x AC|), avoiding Heron's cancellation.
// The discussion board supplies the adjacent-ray reduction and notes that
// no three input points are collinear; an exact zero-area guard is retained.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

struct Point {
    double x;
    double y;
};

struct Ray {
    Point p;
    double angle;
};

static bool byAngle(const Ray &a, const Ray &b) {
    return a.angle < b.angle;
}

static double distance(const Point &a, const Point &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

static double circumradius(const Point &a, const Point &b, const Point &c) {
    double ux = b.x - a.x;
    double uy = b.y - a.y;
    double vx = c.x - a.x;
    double vy = c.y - a.y;
    double twiceArea = fabs(ux * vy - uy * vx);
    // Decimal collinear coordinates need a scale-aware guard: their binary
    // products need not cancel bit-for-bit.
    if (twiceArea <= 1e-12 * (fabs(ux * vy) + fabs(uy * vx) + 1.0)) return 0.0;
    return distance(a, b) * distance(a, c) * distance(b, c) / (2.0 * twiceArea);
}

int main() {
    int tests;
    if (scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int n;
        scanf("%d", &n);
        std::vector<Point> points(n);
        int i;
        for (i = 0; i < n; ++i) scanf("%lf%lf", &points[i].x, &points[i].y);

        double answer = 0.0;
        for (i = 0; i < n; ++i) {
            std::vector<Ray> rays;
            int j;
            for (j = 0; j < n; ++j) {
                if (j == i) continue;
                Ray ray;
                ray.p = points[j];
                ray.angle = atan2(points[j].y - points[i].y, points[j].x - points[i].x);
                rays.push_back(ray);
            }
            std::sort(rays.begin(), rays.end(), byAngle);
            for (j = 0; j < n - 1; ++j) {
                double candidate = circumradius(points[i], rays[j].p, rays[(j + 1) % (n - 1)].p);
                if (candidate > answer) answer = candidate;
            }
        }
        printf("%.3f\n", answer);
    }
    return 0;
}
