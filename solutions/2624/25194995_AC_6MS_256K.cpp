// POJ 2624 - 4th Point
// Model: claude-sonnet-5
// Approach: Each input line gives two adjacent sides of a parallelogram as
// point pairs (P1,P2) and (P3,P4). The two sides share one common vertex,
// but (per the problem's discuss board) that shared vertex is not always
// the "middle" pair (P2,P3) as in the sample -- it can be any of the four
// pairings (P1,P3),(P1,P4),(P2,P3),(P2,P4). Find which pair coincides
// (compare with a small epsilon), then the 4th point equals
// other1 + other2 - shared, where other1/other2 are the non-shared
// endpoints of side 1 and side 2 respectively (parallelogram diagonal
// midpoint identity: opposite vertices' midpoints coincide).
// Ambiguity handled: statement's own gloss (the discuss board) is what
// settles that the shared vertex isn't fixed to the middle pair; this is
// the ambiguity the sample alone could not settle, since the sample only
// ever exercises the P2==P3 case.
#include <cstdio>
#include <cmath>

struct Pt { double x, y; };

static bool same(const Pt &a, const Pt &b) {
    return fabs(a.x - b.x) < 1e-6 && fabs(a.y - b.y) < 1e-6;
}

static double fixZero(double v) {
    if (fabs(v) < 0.0005) return 0.0;
    return v;
}

int main() {
    double x1, y1, x2, y2, x3, y3, x4, y4;
    while (scanf("%lf %lf %lf %lf %lf %lf %lf %lf",
                  &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4) == 8) {
        Pt p1 = {x1, y1}, p2 = {x2, y2}, p3 = {x3, y3}, p4 = {x4, y4};
        Pt ans;
        bool found = true;
        if (same(p1, p3)) {
            ans.x = p2.x + p4.x - p1.x;
            ans.y = p2.y + p4.y - p1.y;
        } else if (same(p1, p4)) {
            ans.x = p2.x + p3.x - p1.x;
            ans.y = p2.y + p3.y - p1.y;
        } else if (same(p2, p3)) {
            ans.x = p1.x + p4.x - p2.x;
            ans.y = p1.y + p4.y - p2.y;
        } else if (same(p2, p4)) {
            ans.x = p1.x + p3.x - p2.x;
            ans.y = p1.y + p3.y - p2.y;
        } else {
            // Fallback: no exact match found (shouldn't happen on valid
            // data) -- pick the closest pair by distance.
            found = false;
            double best = 1e18;
            double bx = 0.0, by = 0.0;
            double d;
            d = (p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y);
            if (d < best) { best = d; bx = p2.x + p4.x - p1.x; by = p2.y + p4.y - p1.y; }
            d = (p1.x - p4.x) * (p1.x - p4.x) + (p1.y - p4.y) * (p1.y - p4.y);
            if (d < best) { best = d; bx = p2.x + p3.x - p1.x; by = p2.y + p3.y - p1.y; }
            d = (p2.x - p3.x) * (p2.x - p3.x) + (p2.y - p3.y) * (p2.y - p3.y);
            if (d < best) { best = d; bx = p1.x + p4.x - p2.x; by = p1.y + p4.y - p2.y; }
            d = (p2.x - p4.x) * (p2.x - p4.x) + (p2.y - p4.y) * (p2.y - p4.y);
            if (d < best) { best = d; bx = p1.x + p3.x - p2.x; by = p1.y + p3.y - p2.y; }
            ans.x = bx;
            ans.y = by;
        }
        (void)found;
        printf("%.3f %.3f\n", fixZero(ans.x), fixZero(ans.y));
    }
    return 0;
}
