// POJ 1264 - SCUD Busters
// Model: gpt-5.6-terra
// Approach: build each kingdom's convex hull with the monotone chain algorithm;
// a missile destroys a kingdom iff it is on the same (inclusive) side of every
// directed hull edge.  A destroyed flag prevents duplicate missiles from adding
// its area twice.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

using namespace std;

struct Point {
    int x, y;
    Point() {}
    Point(int xx, int yy) : x(xx), y(yy) {}
};

bool operator < (const Point &a, const Point &b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
}

bool same_point(const Point &a, const Point &b) {
    return a.x == b.x && a.y == b.y;
}

long cross(const Point &a, const Point &b, const Point &c) {
    return (long)(b.x - a.x) * (c.y - a.y) -
           (long)(b.y - a.y) * (c.x - a.x);
}

vector<Point> hull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end(), same_point), p.end());
    vector<Point> h;
    int i;
    for (i = 0; i < (int)p.size(); ++i) {
        while (h.size() >= 2 && cross(h[h.size() - 2], h[h.size() - 1], p[i]) <= 0)
            h.pop_back();
        h.push_back(p[i]);
    }
    int lower = (int)h.size();
    for (i = (int)p.size() - 2; i >= 0; --i) {
        while ((int)h.size() > lower && cross(h[h.size() - 2], h[h.size() - 1], p[i]) <= 0)
            h.pop_back();
        h.push_back(p[i]);
    }
    if (h.size() > 1) h.pop_back();
    return h;
}

double area(const vector<Point> &p) {
    long twice = 0;
    int i;
    for (i = 0; i < (int)p.size(); ++i)
        twice += (long)p[i].x * p[(i + 1) % p.size()].y -
                 (long)p[i].y * p[(i + 1) % p.size()].x;
    return fabs((double)twice) / 2.0;
}

bool inside(const vector<Point> &p, const Point &q) {
    int positive = 0, negative = 0, i;
    for (i = 0; i < (int)p.size(); ++i) {
        long v = cross(p[i], p[(i + 1) % p.size()], q);
        if (v > 0) positive = 1;
        if (v < 0) negative = 1;
    }
    return !(positive && negative);
}

int main() {
    vector< vector<Point> > kingdoms;
    vector<double> kingdom_area;
    int n, i, x, y;
    while (scanf("%d", &n) == 1 && n != -1) {
        vector<Point> p;
        for (i = 0; i < n; ++i) {
            scanf("%d%d", &x, &y);
            p.push_back(Point(x, y));
        }
        p = hull(p);
        kingdoms.push_back(p);
        kingdom_area.push_back(area(p));
    }
    vector<int> destroyed(kingdoms.size(), 0);
    double answer = 0.0;
    while (scanf("%d%d", &x, &y) == 2) {
        Point missile(x, y);
        for (i = 0; i < (int)kingdoms.size(); ++i) {
            if (!destroyed[i] && inside(kingdoms[i], missile)) {
                destroyed[i] = 1;
                answer += kingdom_area[i];
            }
        }
    }
    printf("%.2f\n", answer);
    return 0;
}
