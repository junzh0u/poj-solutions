// POJ 3384 - Feng Shui
// Model: claude-opus-5
//
// Two carpets of radius r must lie entirely inside a convex room; maximise the
// area they cover together.  The covered area is |D1 u D2| = 2*pi*r^2 - lens(d),
// and the lens is strictly decreasing in the centre distance d (zero past 2r),
// so the whole problem is: put the two centres as far apart as possible.
//
// A disk of radius r sits inside a convex polygon exactly when its centre is at
// distance >= r from every wall, on the interior side.  So the feasible centres
// form the polygon shrunk inward by r -- an intersection of n half planes, here
// computed by clipping a large box wall by wall (Sutherland-Hodgman).  Each
// clipping line is stored as a point plus a unit direction, so the "side" value
// is the true signed distance and the epsilon is a distance tolerance.  The
// answer is then the farthest pair of vertices of that region (n <= 100, so the
// O(n^2) scan is free and avoids rotating-calipers corner cases that the discuss
// board reports as a source of WA).
//
// Degeneracies, all from the discuss board and all reproduced locally: the
// shrunk region can collapse to a single point ("4 1 / 0 0 / 0 2 / 2 2 / 2 0"
// -> "1 1 1 1"), which is why the farthest-pair scan allows i == j and starts
// from -1 instead of 0 -- otherwise nothing updates the best pair and the
// output is garbage.  Ambiguity: any two disjoint placements are equally
// optimal, so sample 1's answer (distance 5.02) and this one (6.45) are both
// correct -- above d = 2r the covered area is already 2*pi*r^2.  The output is
// ordered by x because one board post claims the judge wants it that way and
// another says it does not; ordering satisfies both readings.

#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

struct Pt {
    double x, y;
    Pt(double a = 0, double b = 0) : x(a), y(b) {}
};

/* signed distance of p from the line through a with unit direction (ux,uy) */
static double side(const Pt &p, const Pt &a, double ux, double uy) {
    return ux * (p.y - a.y) - uy * (p.x - a.x);
}

/* clip a convex polygon to the closed left half plane of that directed line */
static vector<Pt> clipHalf(const vector<Pt> &poly, const Pt &a, double ux, double uy, double eps) {
    vector<Pt> out;
    int m = (int)poly.size();
    for (int i = 0; i < m; i++) {
        const Pt &cur = poly[i];
        const Pt &nxt = poly[(i + 1) % m];
        double s1 = side(cur, a, ux, uy);
        double s2 = side(nxt, a, ux, uy);
        if (s1 > -eps) out.push_back(cur);
        if ((s1 > eps && s2 < -eps) || (s1 < -eps && s2 > eps)) {
            double t = s1 / (s1 - s2);
            out.push_back(Pt(cur.x + t * (nxt.x - cur.x), cur.y + t * (nxt.y - cur.y)));
        }
    }
    return out;
}

int main() {
    int n;
    double r;
    while (scanf("%d %lf", &n, &r) == 2) {
        vector<Pt> p(n);
        for (int i = 0; i < n; i++) scanf("%lf %lf", &p[i].x, &p[i].y);

        double a2 = 0.0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            a2 += p[i].x * p[j].y - p[j].x * p[i].y;
        }
        if (a2 < 0) reverse(p.begin(), p.end()); /* the input is clockwise; work ccw */

        vector<Pt> reg;
        double tries[4] = {1e-9, 1e-7, 1e-5, 1e-3};
        for (int t = 0; t < 4 && reg.empty(); t++) {
            double eps = tries[t];
            vector<Pt> poly;
            poly.push_back(Pt(-3000, -3000));
            poly.push_back(Pt(3000, -3000));
            poly.push_back(Pt(3000, 3000));
            poly.push_back(Pt(-3000, 3000));
            for (int i = 0; i < n && !poly.empty(); i++) {
                int j = (i + 1) % n;
                double dx = p[j].x - p[i].x, dy = p[j].y - p[i].y;
                double len = sqrt(dx * dx + dy * dy);
                double ux = dx / len, uy = dy / len;
                /* inward normal of a ccw polygon is (-uy, ux): push the wall in by r */
                Pt a(p[i].x - uy * r, p[i].y + ux * r);
                poly = clipHalf(poly, a, ux, uy, eps);
            }
            reg = poly;
        }

        double bx1 = 0, by1 = 0, bx2 = 0, by2 = 0;
        if (!reg.empty()) {
            double best = -1.0;
            /* i == j allowed, so a region that collapsed to one point still answers */
            for (size_t i = 0; i < reg.size(); i++)
                for (size_t j = i; j < reg.size(); j++) {
                    double dx = reg[i].x - reg[j].x, dy = reg[i].y - reg[j].y;
                    double d = dx * dx + dy * dy;
                    if (d > best) {
                        best = d;
                        bx1 = reg[i].x; by1 = reg[i].y;
                        bx2 = reg[j].x; by2 = reg[j].y;
                    }
                }
        }
        if (bx1 > bx2 || (bx1 == bx2 && by1 > by2)) {
            swap(bx1, bx2);
            swap(by1, by2);
        }
        if (fabs(bx1) < 1e-9) bx1 = 0.0;
        if (fabs(by1) < 1e-9) by1 = 0.0;
        if (fabs(bx2) < 1e-9) bx2 = 0.0;
        if (fabs(by2) < 1e-9) by2 = 0.0;
        printf("%.10f %.10f %.10f %.10f\n", bx1, by1, bx2, by2);
    }
    return 0;
}
