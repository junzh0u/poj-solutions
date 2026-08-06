// POJ 3130 - How I Mathematician Wonder What You Are!
// Model: claude-sonnet-5
// Approach: The polygon is star-shaped iff its kernel (intersection of the
// interior half-planes defined by extending every edge) is non-empty. Since
// the polygon is given CCW, each edge (V[i], V[i+1]) defines the half-plane
// { P : cross(V[i+1]-V[i], P-V[i]) >= 0 }. Intersect all n half-planes by
// clipping a large bounding polygon successively (Sutherland-Hodgman style),
// starting from the polygon's own bounding box (kernel subset of polygon, so
// no need for a larger box). If the resulting polygon has at least one
// vertex, the kernel is non-empty (possibly a point or segment) -> 1,
// otherwise -> 0.
// Ambiguity note: the kernel can degenerate to a single point or a line
// segment (zero area) and still count as star-shaped (a single center still
// satisfies the definition), so the test is "polygon non-empty", not
// "positive area".
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

const double EPS = 1e-8;

struct Pt {
    double x, y;
    Pt() : x(0), y(0) {}
    Pt(double x_, double y_) : x(x_), y(y_) {}
    Pt operator-(const Pt& o) const { return Pt(x - o.x, y - o.y); }
    Pt operator+(const Pt& o) const { return Pt(x + o.x, y + o.y); }
    Pt operator*(double t) const { return Pt(x * t, y * t); }
};

double cross(const Pt& o, const Pt& a, const Pt& b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

// Keep the part of poly on the left of directed line p->q (cross >= -EPS).
vector<Pt> clip(const vector<Pt>& poly, const Pt& p, const Pt& q) {
    vector<Pt> res;
    int n = (int)poly.size();
    if (n == 0) return res;
    for (int i = 0; i < n; ++i) {
        const Pt& A = poly[i];
        const Pt& B = poly[(i + 1) % n];
        double cA = cross(p, q, A);
        double cB = cross(p, q, B);
        bool inA = cA >= -EPS;
        bool inB = cB >= -EPS;
        if (inA) res.push_back(A);
        if (inA != inB) {
            double t = cA / (cA - cB);
            res.push_back(A + (B - A) * t);
        }
    }
    return res;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<Pt> v(n);
        double minx = 1e18, maxx = -1e18, miny = 1e18, maxy = -1e18;
        for (int i = 0; i < n; ++i) {
            double x, y;
            scanf("%lf %lf", &x, &y);
            v[i] = Pt(x, y);
            if (x < minx) minx = x;
            if (x > maxx) maxx = x;
            if (y < miny) miny = y;
            if (y > maxy) maxy = y;
        }
        // Start with a bounding box big enough to contain the whole polygon
        // (kernel is a subset of the polygon itself).
        double lo_x = minx - 1.0, hi_x = maxx + 1.0;
        double lo_y = miny - 1.0, hi_y = maxy + 1.0;
        vector<Pt> poly;
        poly.push_back(Pt(lo_x, lo_y));
        poly.push_back(Pt(hi_x, lo_y));
        poly.push_back(Pt(hi_x, hi_y));
        poly.push_back(Pt(lo_x, hi_y));

        for (int i = 0; i < n && !poly.empty(); ++i) {
            Pt A = v[i];
            Pt B = v[(i + 1) % n];
            poly = clip(poly, A, B);
        }

        printf("%d\n", poly.empty() ? 0 : 1);
    }
    return 0;
}
