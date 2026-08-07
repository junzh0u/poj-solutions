// POJ 2540 - Hotter Colder
// Model: claude-opus-5
//
// The object lies somewhere in the square [0,10]x[0,10].  Player A starts at
// (0,0); every input line gives a new position and compares its distance to
// the object against the distance from the *previous* position.  Each answer
// is therefore a half-plane bounded by the perpendicular bisector of the two
// consecutive positions:
//
//   |P-cur|^2 - |P-prev|^2 = 2*P.(prev-cur) + |cur|^2 - |prev|^2 = f(P)
//   Hotter -> f(P) <= 0, Colder -> f(P) >= 0, Same -> f(P) == 0.
//
// So keep a convex polygon (initially the square) and clip it with each
// half-plane via Sutherland-Hodgman, printing its area after every line.
// "Same" clips with both half-planes, collapsing the region onto a segment
// (area 0.00), which is what the judge expects; once the region is empty or
// degenerate it stays that way, so all later lines print 0.00 too.
//
// Degenerate case, raised on the discuss board: cur == prev.  The bisector is
// then undefined (f == 0 identically).  "Same" is vacuously true and imposes
// no constraint (so e.g. "10 10 Colder" / "10 10 Same" prints 50.00 twice, as
// one board thread argues), while "Hotter"/"Colder" say the distance both did
// and did not change -- a contradiction, so no region exists and 0.00 is
// printed from there on.  Handled explicitly, since the general formula would
// silently keep everything.  A reply on that thread says the judge data holds
// no repeated position at all, so this only has to be self-consistent.
//
// Strict vs. non-strict inequalities do not change any area, so <= is used
// throughout for the non-degenerate half-planes.
//
// Note on the official sample: it is symmetric under reflection through the
// square's centre, so it prints 50.00/37.50/12.50/0.00 under the *swapped*
// reading of Hotter/Colder too, and cannot validate which side is kept.  The
// side here was pinned instead against an exact rational reference driven
// directly by the statement's distance comparison, and by hand on an
// asymmetric case: from (0,0) to (1,1) with "Hotter" leaves 99.50, not 0.50.

#include <cstdio>
#include <cstring>
#include <cmath>

static const double EPS = 1e-9;

struct Pt {
    double x, y;
};

static Pt poly[512], buf[512];
static int n;

// Keep the part of the polygon with a*x + b*y + c <= 0.
static void clipHalf(double a, double b, double c)
{
    if (n == 0) return;
    int m = 0;
    for (int i = 0; i < n; ++i) {
        Pt p = poly[i];
        Pt q = poly[(i + 1) % n];
        double fp = a * p.x + b * p.y + c;
        double fq = a * q.x + b * q.y + c;
        if (fp <= EPS) buf[m++] = p;
        if ((fp > EPS && fq < -EPS) || (fp < -EPS && fq > EPS)) {
            double t = fp / (fp - fq);
            Pt r;
            r.x = p.x + t * (q.x - p.x);
            r.y = p.y + t * (q.y - p.y);
            buf[m++] = r;
        }
    }
    n = m;
    for (int i = 0; i < n; ++i) poly[i] = buf[i];
}

static double area()
{
    if (n < 3) return 0.0;
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        s += poly[i].x * poly[j].y - poly[j].x * poly[i].y;
    }
    s = fabs(s) / 2.0;
    return s < 1e-9 ? 0.0 : s;
}

int main()
{
    n = 4;
    poly[0].x = 0.0;  poly[0].y = 0.0;
    poly[1].x = 10.0; poly[1].y = 0.0;
    poly[2].x = 10.0; poly[2].y = 10.0;
    poly[3].x = 0.0;  poly[3].y = 10.0;

    double px = 0.0, py = 0.0;
    double x, y;
    char word[64];

    while (scanf("%lf %lf %63s", &x, &y, word) == 3) {
        double dx = px - x, dy = py - y;
        int same = (strcmp(word, "Same") == 0);
        int hotter = (strcmp(word, "Hotter") == 0);

        if (fabs(dx) < 1e-12 && fabs(dy) < 1e-12) {
            /* No movement: distances are necessarily equal. */
            if (!same) n = 0;
        } else {
            double a = 2.0 * dx;
            double b = 2.0 * dy;
            double c = (x * x + y * y) - (px * px + py * py);
            if (same) {
                clipHalf(a, b, c);
                clipHalf(-a, -b, -c);
            } else if (hotter) {
                clipHalf(a, b, c);
            } else {
                clipHalf(-a, -b, -c);
            }
        }

        printf("%.2f\n", area());
        px = x;
        py = y;
    }
    return 0;
}
