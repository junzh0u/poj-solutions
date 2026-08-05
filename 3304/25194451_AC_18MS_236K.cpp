// POJ 3304 - Segments
//
// The projections of all n segments onto a line L share a common point p iff
// the line M through p perpendicular to L meets every segment: the preimage of
// p under the projection onto L is exactly that perpendicular line.  So the
// question "does some direction project all segments onto a common point?" is
// the same as "does some line stab all n segments?".
//
// Such a stabbing line, if one exists, can always be moved onto two segment
// endpoints: translate it perpendicular to itself until it is about to leave
// some segment -- at that moment it touches an endpoint P -- then rotate it
// about P until it is about to leave some segment again, which puts a second
// endpoint Q on it.  (If the rotation never has to stop, every segment already
// contains P, and then P together with any other endpoint gives a valid line.)
// So it suffices to test the O(n^2) lines through pairs of the 2n endpoints
// against all n segments: O(n^3), 2M sign tests at n = 100.
//
// A line hits a segment iff its two endpoints do not lie strictly on the same
// side, i.e. sgn(cross) * sgn(cross) <= 0.
//
// Ambiguities / traps in the statement:
//  * coordinates are *real*, so the side tests need an epsilon -- the statement
//    fixes it by declaring a == b when |a - b| < 1e-8.
//  * two endpoints closer than that epsilon do not define a direction and must
//    not be used as a candidate pair.  If no candidate pair survives, every
//    segment is the same single point and the answer is "Yes!".
//  * n = 1 (and n = 2) is a real input and is always "Yes!": pick any point on
//    each segment and take the line through them.  Handled up front so a
//    degenerate zero-length segment cannot fall through the pair enumeration.
//  * nothing bounds T, but the judge data is small; the answer strings carry
//    an exclamation mark.
#include <cstdio>
#include <cmath>

const double EPS = 1e-8;

struct P { double x, y; };

static P a[105], b[105];
static int n;

static int sgn(double v) {
    if (v > EPS) return 1;
    if (v < -EPS) return -1;
    return 0;
}

/* cross of (q-p) x (r-p) */
static double cross(const P &p, const P &q, const P &r) {
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
}

static bool lineHitsAll(const P &p, const P &q) {
    for (int i = 0; i < n; ++i) {
        int s1 = sgn(cross(p, q, a[i]));
        int s2 = sgn(cross(p, q, b[i]));
        if (s1 * s2 > 0) return false;
    }
    return true;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        scanf("%d", &n);
        for (int i = 0; i < n; ++i)
            scanf("%lf %lf %lf %lf", &a[i].x, &a[i].y, &b[i].x, &b[i].y);

        bool ok = false;
        if (n <= 2) {
            /* one or two segments: pick a point on each, the line through
               them meets both (any line through the point if they coincide) */
            ok = true;
        } else {
            bool anyPair = false;
            P pts[205];
            int m = 0;
            for (int i = 0; i < n; ++i) { pts[m++] = a[i]; pts[m++] = b[i]; }
            for (int i = 0; i < m && !ok; ++i)
                for (int j = i + 1; j < m && !ok; ++j) {
                    double dx = pts[i].x - pts[j].x, dy = pts[i].y - pts[j].y;
                    if (fabs(dx) < EPS && fabs(dy) < EPS) continue;
                    anyPair = true;
                    if (lineHitsAll(pts[i], pts[j])) ok = true;
                }
            /* every endpoint coincides: all segments are the same point */
            if (!anyPair) ok = true;
        }
        printf(ok ? "Yes!\n" : "No!\n");
    }
    return 0;
}
