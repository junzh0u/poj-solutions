// POJ 3805 - Separate Points
// Model: claude-sonnet-5
// Approach: Two point sets (black/white, all points distinct) admit a straight
// line strictly separating them (touching no point) iff their convex hulls,
// as closed convex regions, are disjoint (standard strict-separation theorem
// for compact convex sets). So compute the convex hull of each color and test
// whether the two convex hulls intersect (share any point, including a
// boundary touch) using exact integer arithmetic: containment of any hull
// vertex of one set inside the other hull (inclusive of boundary), plus
// pairwise segment intersection of hull edges (handles proper crossing,
// touching at endpoints, and collinear overlap). Hulls of size 1 (a single
// point) or 2 (a segment) are handled as degenerate polygons throughout.
// No statement ambiguity beyond confirming, via the given samples (notably
// the two collinear-points cases and the two crossing-diagonals case), that
// hull touching must count as "not separable".
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;

struct Point {
    ll x, y;
};

bool operator==(const Point &a, const Point &b) {
    return a.x == b.x && a.y == b.y;
}

bool cmpPoint(const Point &a, const Point &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

ll cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

vector<Point> convexHull(vector<Point> pts) {
    sort(pts.begin(), pts.end(), cmpPoint);
    int n = (int)pts.size();
    if (n <= 2) return pts;
    vector<Point> hull(2 * n);
    int k = 0;
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }
    int lower = k + 1;
    for (int i = n - 2; i >= 0; i--) {
        while (k >= lower && cross(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
        hull[k++] = pts[i];
    }
    hull.resize(k - 1);
    return hull;
}

bool onSegment(const Point &a, const Point &b, const Point &p) {
    if (cross(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

// Standard robust segment intersection test (includes touching / overlap).
bool segInter(const Point &a, const Point &b, const Point &c, const Point &d) {
    ll d1 = cross(c, d, a);
    ll d2 = cross(c, d, b);
    ll d3 = cross(a, b, c);
    ll d4 = cross(a, b, d);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
        return true;
    if (d1 == 0 && onSegment(c, d, a)) return true;
    if (d2 == 0 && onSegment(c, d, b)) return true;
    if (d3 == 0 && onSegment(a, b, c)) return true;
    if (d4 == 0 && onSegment(a, b, d)) return true;
    return false;
}

vector<pair<Point, Point> > getEdges(const vector<Point> &h) {
    vector<pair<Point, Point> > edges;
    int n = (int)h.size();
    if (n == 2) {
        edges.push_back(make_pair(h[0], h[1]));
    } else if (n >= 3) {
        for (int i = 0; i < n; i++)
            edges.push_back(make_pair(h[i], h[(i + 1) % n]));
    }
    return edges;
}

// Inclusive point-in-convex-hull test (hull assumed CCW, from convexHull()).
bool pointInConvex(const vector<Point> &h, const Point &p) {
    int n = (int)h.size();
    if (n == 1) return p == h[0];
    if (n == 2) return onSegment(h[0], h[1], p);
    for (int i = 0; i < n; i++) {
        if (cross(h[i], h[(i + 1) % n], p) < 0) return false;
    }
    return true;
}

bool hullsIntersect(const vector<Point> &A, const vector<Point> &B) {
    for (size_t i = 0; i < A.size(); i++)
        if (pointInConvex(B, A[i])) return true;
    for (size_t i = 0; i < B.size(); i++)
        if (pointInConvex(A, B[i])) return true;
    vector<pair<Point, Point> > eA = getEdges(A), eB = getEdges(B);
    for (size_t i = 0; i < eA.size(); i++)
        for (size_t j = 0; j < eB.size(); j++)
            if (segInter(eA[i].first, eA[i].second, eB[j].first, eB[j].second))
                return true;
    return false;
}

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        vector<Point> black(n), white(m);
        for (int i = 0; i < n; i++) scanf("%lld %lld", &black[i].x, &black[i].y);
        for (int i = 0; i < m; i++) scanf("%lld %lld", &white[i].x, &white[i].y);
        vector<Point> hb = convexHull(black);
        vector<Point> hw = convexHull(white);
        bool inter = hullsIntersect(hb, hw);
        printf(inter ? "NO\n" : "YES\n");
    }
    return 0;
}
