// POJ 1410 - Intersection
//
// Decide whether a closed line segment meets an axis-aligned rectangle,
// where "meets" means they share at least one point and the rectangle is
// the four sides *plus* the area in between.
//
// Two traps in the statement:
//
//  1. "The terms top left and bottom right do not imply any ordering of
//     coordinates" -- the two given corners are an arbitrary diagonal pair,
//     so the sides must be normalised (x1..x2, y1..y2 by min/max) before any
//     containment or side test. Several of the judge's cases hand over the
//     rectangle upside down or right-to-left.
//
//  2. Because the interior counts, a segment lying wholly inside the
//     rectangle answers T while crossing none of the four sides. So the
//     side-crossing test alone is not enough: test one endpoint for
//     containment as well (if the segment crosses no side, either both
//     endpoints are inside or both are outside, so a single containment
//     hit settles it). Conversely a segment that only grazes a corner, or
//     that runs collinear along a side, also answers T -- every test here
//     is closed/inclusive.
//
// The rest is the standard segment-segment test: the two straddle tests via
// the signs of four cross products, plus the collinear fallback where a
// zero cross product is accepted only if the point lies in the other
// segment's bounding box. That fallback is what makes a zero-length segment
// (start == end) and a segment collinear with a side come out right.
//
// Coordinates are integers, so every sign test is exact integer arithmetic
// in 64 bits -- no floating point, despite the statement's warning that the
// intersection point itself need not be on the integer grid. Input is read
// with %d rather than %lld because POJ's Windows C++ runtime handles %lld
// unreliably; the values are widened to long long afterwards so the cross
// products cannot overflow.
//
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

static int sgn(ll v) { return v > 0 ? 1 : (v < 0 ? -1 : 0); }

/* cross product of (a-o) x (b-o) */
static ll cross(ll ox, ll oy, ll ax, ll ay, ll bx, ll by) {
    return (ax - ox) * (by - oy) - (ay - oy) * (bx - ox);
}

/* p is known collinear with ab; is it inside the bounding box of ab? */
static bool onSeg(ll px, ll py, ll ax, ll ay, ll bx, ll by) {
    return min(ax, bx) <= px && px <= max(ax, bx) &&
           min(ay, by) <= py && py <= max(ay, by);
}

/* closed segment ab vs closed segment cd, touching counts */
static bool segInter(ll ax, ll ay, ll bx, ll by, ll cx, ll cy, ll dx, ll dy) {
    int d1 = sgn(cross(ax, ay, bx, by, cx, cy));
    int d2 = sgn(cross(ax, ay, bx, by, dx, dy));
    int d3 = sgn(cross(cx, cy, dx, dy, ax, ay));
    int d4 = sgn(cross(cx, cy, dx, dy, bx, by));

    if (d1 * d2 < 0 && d3 * d4 < 0) return true;

    if (d1 == 0 && onSeg(cx, cy, ax, ay, bx, by)) return true;
    if (d2 == 0 && onSeg(dx, dy, ax, ay, bx, by)) return true;
    if (d3 == 0 && onSeg(ax, ay, cx, cy, dx, dy)) return true;
    if (d4 == 0 && onSeg(bx, by, cx, cy, dx, dy)) return true;
    return false;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n-- > 0) {
        int a, b, c, d, e, f, g, h;
        if (scanf("%d %d %d %d %d %d %d %d",
                  &a, &b, &c, &d, &e, &f, &g, &h) != 8) return 0;
        ll xs = a, ys = b, xe = c, ye = d, xl = e, yt = f, xr = g, yb = h;

        /* the corners are given in arbitrary order: normalise */
        ll x1 = min(xl, xr), x2 = max(xl, xr);
        ll y1 = min(yt, yb), y2 = max(yt, yb);

        bool hit = false;

        /* an endpoint inside (or on) the rectangle -- covers the segment
           lying wholly within the rectangle, which crosses no side */
        if (xs >= x1 && xs <= x2 && ys >= y1 && ys <= y2) hit = true;
        if (xe >= x1 && xe <= x2 && ye >= y1 && ye <= y2) hit = true;

        if (!hit) {
            if (segInter(xs, ys, xe, ye, x1, y1, x2, y1)) hit = true;
            else if (segInter(xs, ys, xe, ye, x2, y1, x2, y2)) hit = true;
            else if (segInter(xs, ys, xe, ye, x2, y2, x1, y2)) hit = true;
            else if (segInter(xs, ys, xe, ye, x1, y2, x1, y1)) hit = true;
        }

        printf("%c\n", hit ? 'T' : 'F');
    }
    return 0;
}
