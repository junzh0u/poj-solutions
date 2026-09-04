// POJ 3082 - 'Roid Rage
// Model: claude-sonnet-5
//
// For every pair of polygons, decide whether they intersect, where
// "intersect" means sharing any interior point or any boundary point
// (touching at a vertex or along an edge counts). Approach: test every
// edge of polygon i against every edge of polygon j with a standard
// robust segment-intersection test (proper crossing via orientation
// signs, plus collinear-overlap handling via onSegment). If no pair of
// edges touches or crosses, the polygons are either fully disjoint or
// one is fully nested inside the other (they cannot partially overlap
// without their boundaries crossing, and the problem guarantees each
// polygon is simple/non-self-intersecting). That nested case is
// detected with a point-in-polygon test on one vertex of each polygon
// against the other. Coordinates are small non-negative integers
// (0..100), so all arithmetic is done in integers (no epsilon issues)
// except the final point-in-polygon ray cast, whose divisions stay
// exact in double for these magnitudes.
//
// No statement ambiguity found; matches the sample exactly.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef int ll;

struct Point {
    ll x, y;
};

static ll cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

static int sgn(ll v) {
    if (v > 0) return 1;
    if (v < 0) return -1;
    return 0;
}

// assumes p, q, r are collinear; checks whether q lies on segment pr
static bool onSegment(const Point &p, const Point &q, const Point &r) {
    return min(p.x, r.x) <= q.x && q.x <= max(p.x, r.x) &&
           min(p.y, r.y) <= q.y && q.y <= max(p.y, r.y);
}

static bool segIntersect(const Point &p1, const Point &p2,
                          const Point &p3, const Point &p4) {
    ll d1 = cross(p3, p4, p1);
    ll d2 = cross(p3, p4, p2);
    ll d3 = cross(p1, p2, p3);
    ll d4 = cross(p1, p2, p4);

    if (((sgn(d1) > 0 && sgn(d2) < 0) || (sgn(d1) < 0 && sgn(d2) > 0)) &&
        ((sgn(d3) > 0 && sgn(d4) < 0) || (sgn(d3) < 0 && sgn(d4) > 0)))
        return true;

    if (d1 == 0 && onSegment(p3, p1, p4)) return true;
    if (d2 == 0 && onSegment(p3, p2, p4)) return true;
    if (d3 == 0 && onSegment(p1, p3, p2)) return true;
    if (d4 == 0 && onSegment(p1, p4, p2)) return true;

    return false;
}

static bool pointInPolygon(const Point &p, const vector<Point> &poly) {
    size_t n = poly.size();
    bool inside = false;
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const Point &pi = poly[i];
        const Point &pj = poly[j];
        bool cond = (pi.y > p.y) != (pj.y > p.y);
        if (cond) {
            double xint = (double)(pj.x - pi.x) * (double)(p.y - pi.y) /
                              (double)(pj.y - pi.y) +
                          (double)pi.x;
            if ((double)p.x < xint) inside = !inside;
        }
    }
    return inside;
}

static bool polygonsIntersect(const vector<Point> &A, const vector<Point> &B) {
    size_t na = A.size(), nb = B.size();
    for (size_t i = 0; i < na; ++i) {
        Point a1 = A[i];
        Point a2 = A[(i + 1) % na];
        for (size_t j = 0; j < nb; ++j) {
            Point b1 = B[j];
            Point b2 = B[(j + 1) % nb];
            if (segIntersect(a1, a2, b1, b2)) return true;
        }
    }
    if (pointInPolygon(A[0], B)) return true;
    if (pointInPolygon(B[0], A)) return true;
    return false;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int tc = 1; tc <= n; ++tc) {
        int m;
        scanf("%d", &m);
        vector<vector<Point> > polys(m);
        for (int i = 0; i < m; ++i) {
            int v;
            scanf("%d", &v);
            polys[i].resize(v);
            for (int k = 0; k < v; ++k) {
                int x, y;
                scanf("%d,%d", &x, &y);
                polys[i][k].x = x;
                polys[i][k].y = y;
            }
        }

        printf("Data Set #%d\n", tc);
        bool any = false;
        for (int i = 0; i < m; ++i) {
            for (int j = i + 1; j < m; ++j) {
                if (polygonsIntersect(polys[i], polys[j])) {
                    printf("%d %d\n", i + 1, j + 1);
                    any = true;
                }
            }
        }
        if (!any) printf("no collisions\n");
    }
    return 0;
}
