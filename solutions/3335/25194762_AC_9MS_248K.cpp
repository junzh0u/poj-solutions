// POJ 3335 - Rotating Scoreboard
// Model: gpt-5.6-terra
// Intersect the inward half-plane of every polygon edge.  Their common region
// is the polygon kernel, precisely the set of points visible from all boundary
// points.  Sutherland-Hodgman clipping handles n <= 100 directly.  Boundary
// points are kept because a tangent line of sight is explicitly allowed.
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

const long double EPS = 1e-10L;

struct Point {
    long double x, y;
    Point() : x(0), y(0) {}
    Point(long double xx, long double yy) : x(xx), y(yy) {}
};

Point operator+(const Point &a, const Point &b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point &a, const Point &b) {
    return Point(a.x - b.x, a.y - b.y);
}

Point operator*(const Point &a, long double t) {
    return Point(a.x * t, a.y * t);
}

long double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

long double side(const Point &a, const Point &b, const Point &p) {
    return cross(b - a, p - a);
}

vector<Point> clip(const vector<Point> &poly, const Point &a, const Point &b,
                   int orientation) {
    vector<Point> result;
    int m = (int)poly.size();
    int i;
    for (i = 0; i < m; ++i) {
        Point s = poly[i];
        Point e = poly[(i + 1) % m];
        long double ds = orientation * side(a, b, s);
        long double de = orientation * side(a, b, e);
        bool inS = ds >= -EPS;
        bool inE = de >= -EPS;
        if (inS && inE) {
            result.push_back(e);
        } else if (inS && !inE) {
            result.push_back(s + (e - s) * (ds / (ds - de)));
        } else if (!inS && inE) {
            result.push_back(s + (e - s) * (ds / (ds - de)));
            result.push_back(e);
        }
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int tests;
    cin >> tests;
    while (tests--) {
        int n;
        cin >> n;
        vector<Point> vertices(n);
        long double minX = 0, maxX = 0, minY = 0, maxY = 0;
        int i;
        for (i = 0; i < n; ++i) {
            cin >> vertices[i].x >> vertices[i].y;
            if (i == 0) {
                minX = maxX = vertices[i].x;
                minY = maxY = vertices[i].y;
            } else {
                minX = min(minX, vertices[i].x);
                maxX = max(maxX, vertices[i].x);
                minY = min(minY, vertices[i].y);
                maxY = max(maxY, vertices[i].y);
            }
        }

        long double twiceArea = 0;
        for (i = 0; i < n; ++i)
            twiceArea += cross(vertices[i], vertices[(i + 1) % n]);
        int orientation = twiceArea >= 0 ? 1 : -1;

        vector<Point> kernel;
        kernel.push_back(Point(minX, minY));
        kernel.push_back(Point(maxX, minY));
        kernel.push_back(Point(maxX, maxY));
        kernel.push_back(Point(minX, maxY));
        for (i = 0; i < n && !kernel.empty(); ++i)
            kernel = clip(kernel, vertices[i], vertices[(i + 1) % n], orientation);

        cout << (kernel.empty() ? "NO" : "YES") << '\n';
    }
    return 0;
}
