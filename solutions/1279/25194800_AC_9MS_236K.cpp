// POJ 1279 - Art Gallery
// Model: gpt-5.6-terra
// Intersect the interior half-plane of every directed polygon edge.  The
// supplied boundary order may be clockwise or counterclockwise, so its signed
// area selects the interior side.  Successive polygon clipping keeps the
// kernel; an empty or line-only kernel has area zero.
#include <cstdio>
#include <vector>
#include <cmath>

struct Point {
    long double x, y;
    Point() : x(0), y(0) {}
    Point(long double xx, long double yy) : x(xx), y(yy) {}
};

static const long double EPS = 1e-12L;

static Point operator+(const Point &a, const Point &b) {
    return Point(a.x + b.x, a.y + b.y);
}

static Point operator-(const Point &a, const Point &b) {
    return Point(a.x - b.x, a.y - b.y);
}

static Point operator*(const Point &a, long double k) {
    return Point(a.x * k, a.y * k);
}

static long double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

static long double side(const Point &a, const Point &b, const Point &p) {
    return cross(b - a, p - a);
}

static std::vector<Point> clip(const std::vector<Point> &poly,
                               const Point &a, const Point &b,
                               int orientation) {
    std::vector<Point> result;
    int n = (int)poly.size();
    int i;
    for (i = 0; i < n; ++i) {
        Point p = poly[i];
        Point q = poly[(i + 1) % n];
        long double sp = side(a, b, p) * orientation;
        long double sq = side(a, b, q) * orientation;
        bool inP = sp >= -EPS;
        bool inQ = sq >= -EPS;
        if (inP) result.push_back(p);
        if (inP != inQ) {
            long double t = sp / (sp - sq);
            result.push_back(p + (q - p) * t);
        }
    }
    return result;
}

static long double area(const std::vector<Point> &poly) {
    long double twice = 0;
    int n = (int)poly.size();
    int i;
    for (i = 0; i < n; ++i) twice += cross(poly[i], poly[(i + 1) % n]);
    return fabsl(twice) / 2;
}

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        int n, i;
        std::scanf("%d", &n);
        std::vector<Point> vertices(n);
        long double signedTwice = 0;
        for (i = 0; i < n; ++i) {
            int x, y;
            std::scanf("%d%d", &x, &y);
            vertices[i] = Point((long double)x, (long double)y);
        }
        for (i = 0; i < n; ++i) {
            signedTwice += cross(vertices[i], vertices[(i + 1) % n]);
        }
        int orientation = signedTwice >= 0 ? 1 : -1;
        const long double B = 1000000.0L;
        std::vector<Point> kernel;
        kernel.push_back(Point(-B, -B));
        kernel.push_back(Point(B, -B));
        kernel.push_back(Point(B, B));
        kernel.push_back(Point(-B, B));
        for (i = 0; i < n && !kernel.empty(); ++i) {
            kernel = clip(kernel, vertices[i], vertices[(i + 1) % n], orientation);
        }
        long double answer = area(kernel);
        if (answer < 0.0005L) answer = 0;
        std::printf("%.2Lf\n", answer);
    }
    return 0;
}
