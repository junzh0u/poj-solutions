// POJ 2284 - That Nice Euler Circuit
// Model: gpt-5.6-terra
// Split every drawn segment at all of its intersections, then apply Euler's
// formula to the resulting connected planar graph.  Collinear contacts are
// handled as endpoint-on-segment intersections; overlapping segments are
// excluded by the statement.
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

struct Fraction {
    long long n, d;
    Fraction(long long nn = 0, long long dd = 1) : n(nn), d(dd) {
        if (d < 0) n = -n, d = -d;
        long long a = n < 0 ? -n : n, b = d;
        while (b) { long long t = a % b; a = b; b = t; }
        if (a) n /= a, d /= a;
    }
    bool operator<(const Fraction &o) const { return n * o.d < o.n * d; }
};
struct Point {
    long long x, y;
};
struct RPoint {
    Fraction x, y;
    bool operator<(const RPoint &o) const {
        if (x < o.x) return true;
        if (o.x < x) return false;
        return y < o.y;
    }
};
struct Segment { Point a, b; };

long long cross(const Point &a, const Point &b, const Point &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
bool onSegment(const Point &p, const Segment &s) {
    return cross(s.a, s.b, p) == 0 &&
           min(s.a.x, s.b.x) <= p.x && p.x <= max(s.a.x, s.b.x) &&
           min(s.a.y, s.b.y) <= p.y && p.y <= max(s.a.y, s.b.y);
}
RPoint integerPoint(const Point &p) { RPoint r; r.x = Fraction(p.x); r.y = Fraction(p.y); return r; }
RPoint intersection(const Segment &a, const Segment &b, long long den) {
    long long rx = a.b.x - a.a.x, ry = a.b.y - a.a.y;
    long long sx = b.b.x - b.a.x, sy = b.b.y - b.a.y;
    long long num = (b.a.x - a.a.x) * sy - (b.a.y - a.a.y) * sx;
    RPoint p;
    p.x = Fraction(a.a.x * den + rx * num, den);
    p.y = Fraction(a.a.y * den + ry * num, den);
    return p;
}
void addPoint(vector< set<RPoint> > &cuts, set<RPoint> &vertices, int i, int j, const RPoint &p) {
    cuts[i].insert(p); cuts[j].insert(p); vertices.insert(p);
}

int main() {
    ios::sync_with_stdio(false);
    int n, tc = 0;
    while (cin >> n && n) {
        vector<Point> p(n);
        int i;
        for (i = 0; i < n; ++i) cin >> p[i].x >> p[i].y;
        int m = n - 1;
        vector<Segment> seg(m);
        vector< set<RPoint> > cuts(m);
        set<RPoint> vertices;
        for (i = 0; i < m; ++i) {
            seg[i].a = p[i]; seg[i].b = p[i + 1];
            RPoint u = integerPoint(seg[i].a), v = integerPoint(seg[i].b);
            cuts[i].insert(u); cuts[i].insert(v);
            vertices.insert(u); vertices.insert(v);
        }
        for (i = 0; i < m; ++i) for (int j = i + 1; j < m; ++j) {
            long long den = (seg[i].b.x - seg[i].a.x) * (seg[j].b.y - seg[j].a.y) -
                            (seg[i].b.y - seg[i].a.y) * (seg[j].b.x - seg[j].a.x);
            if (den != 0) {
                long long n1 = (seg[j].a.x - seg[i].a.x) * (seg[j].b.y - seg[j].a.y) -
                               (seg[j].a.y - seg[i].a.y) * (seg[j].b.x - seg[j].a.x);
                long long n2 = (seg[j].a.x - seg[i].a.x) * (seg[i].b.y - seg[i].a.y) -
                               (seg[j].a.y - seg[i].a.y) * (seg[i].b.x - seg[i].a.x);
                if ((den > 0 ? 0 <= n1 && n1 <= den : den <= n1 && n1 <= 0) &&
                    (den > 0 ? 0 <= n2 && n2 <= den : den <= n2 && n2 <= 0))
                    addPoint(cuts, vertices, i, j, intersection(seg[i], seg[j], den));
            } else if (cross(seg[i].a, seg[i].b, seg[j].a) == 0) {
                if (onSegment(seg[i].a, seg[j])) addPoint(cuts, vertices, i, j, integerPoint(seg[i].a));
                if (onSegment(seg[i].b, seg[j])) addPoint(cuts, vertices, i, j, integerPoint(seg[i].b));
                if (onSegment(seg[j].a, seg[i])) addPoint(cuts, vertices, i, j, integerPoint(seg[j].a));
                if (onSegment(seg[j].b, seg[i])) addPoint(cuts, vertices, i, j, integerPoint(seg[j].b));
            }
        }
        long long edges = 0;
        for (i = 0; i < m; ++i) edges += (long long)cuts[i].size() - 1;
        cout << "Case " << ++tc << ": There are " << edges - (long long)vertices.size() + 2 << " pieces.\n";
    }
    return 0;
}
