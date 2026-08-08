// POJ 3675 - Telescope
// Model: claude-opus-5
//
// Area of the intersection of a simple polygon with the disk of radius R
// centred at the origin.  Fan the polygon from the origin: the answer is the
// absolute value of the signed sum, over every directed edge a->b, of the
// signed area of triangle (O, a, b) intersected with the disk.  Signed areas
// make the fan telescope correctly for concave polygons and for an origin
// outside the polygon, so no orientation or containment test is needed.
//
// For one edge, solve |a + t(b-a)| = R for t and clamp the root interval to
// [0, 1].  The clamped interval is the part of the segment inside the disk and
// contributes the plain triangle area cross(p1,p2)/2; each part outside the
// disk contributes the circular sector between its endpoints' directions,
// R^2*angle/2 with the angle signed, because the disk inside that wedge lies
// entirely within the corresponding triangle.  Clamping is what makes the
// empty and one-sided cases fall out of the same three-term expression.
//
// Statement gaps settled from the discuss board (author updog confirms the
// data is non-self-intersecting and non-degenerate, and has nasty concave
// cases): the input holds several test cases with no count, so read to EOF;
// the polygon may be given in either orientation, hence the fabs; and the
// output is not special-judged, so a result whose exact value is a multiple of
// 0.005 (possible when the polygon lies wholly inside the circle) must round
// up rather than land on the double just below it -- the 1e-8 nudge, which
// also turns a -0.0 signed sum into "0.00".

#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

double R;

struct P { double x, y; };

static double crs(const P &a, const P &b) { return a.x * b.y - a.y * b.x; }
static double dt(const P &a, const P &b) { return a.x * b.x + a.y * b.y; }

/* Signed area of the disk sector spanned by the directions of a and b. */
static double sectorArea(const P &a, const P &b) {
    double ang = atan2(crs(a, b), dt(a, b));
    return R * R * ang / 2.0;
}

/* Signed area of the intersection of triangle (O, a, b) with the disk. */
static double calc(const P &a, const P &b) {
    P d;
    d.x = b.x - a.x;
    d.y = b.y - a.y;
    double A = dt(d, d);
    if (A <= 0.0) return 0.0;
    double B = 2.0 * dt(a, d);
    double C = dt(a, a) - R * R;
    double disc = B * B - 4.0 * A * C;
    if (disc <= 0.0) return sectorArea(a, b);
    double sq = sqrt(disc);
    double t1 = (-B - sq) / (2.0 * A);
    double t2 = (-B + sq) / (2.0 * A);
    if (t1 < 0.0) t1 = 0.0;
    if (t2 > 1.0) t2 = 1.0;
    if (t1 >= t2) return sectorArea(a, b);
    P p1, p2;
    p1.x = a.x + d.x * t1;
    p1.y = a.y + d.y * t1;
    p2.x = a.x + d.x * t2;
    p2.y = a.y + d.y * t2;
    return sectorArea(a, p1) + crs(p1, p2) / 2.0 + sectorArea(p2, b);
}

static P p[1005];

int main() {
    int n;
    while (scanf("%lf", &R) == 1) {
        if (scanf("%d", &n) != 1) break;
        if (n > 1000) n = 1000;
        for (int i = 0; i < n; i++)
            if (scanf("%lf %lf", &p[i].x, &p[i].y) != 2) return 0;
        double s = 0.0;
        for (int i = 0; i < n; i++) s += calc(p[i], p[(i + 1) % n]);
        s = fabs(s);
        printf("%.2f\n", s + 1e-8);
    }
    return 0;
}
