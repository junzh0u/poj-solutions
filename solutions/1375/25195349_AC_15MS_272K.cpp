// POJ 1375 - Intervals
// Model: claude-opus-5
//
// A point light at B=(bx,by) shines past circles with cy+r < by; report the
// maximal intervals of the x-axis that lie in their shadow.
//
// Each circle's shadow is the intersection of the tangent cone from B with y=0.
// With (dx,dy) = C-B (so dy<0) and L = sqrt(dx^2+dy^2-r^2), rotating the unit
// vector along BC by +-asin(r/d) gives the two extreme rays; dropping the
// common 1/d^2 factor their directions are (dx*L -+ dy*r, +-dx*r + dy*L), and a
// ray (ux,uy) with uy<0 meets y=0 at x = bx - by*ux/uy:
//     x1 = bx - by*(dx*L - dy*r)/(dx*r + dy*L)
//     x2 = bx - by*(dx*L + dy*r)/(dy*L - dx*r)
// Both rays really do point downward exactly because cy+r < by (the cone's half
// angle asin(r/d) is strictly smaller than the depression angle asin((by-cy)/d)
// of BC), so the denominators (which are uy*d^2) are nonzero and no case
// analysis is needed.  r=0 needs no special case either: L=d and the interval
// degenerates to a point.
//
// Then sort by left endpoint and sweep, tracking the running maximum right
// endpoint -- a later interval can be nested inside an earlier one, so
// comparing only against the previous interval's right end is wrong.  Touching
// intervals are merged: the shared point is still unlit.
//
// Output notes, all from the discuss board, each of which cost somebody a WA:
//  * The judge rounds half AWAY FROM ZERO: -617.975 must print as -617.98,
//    where correctly rounding the nearest double gives -617.97.  So each value
//    is nudged outward by EPS_OUT before printing.  EPS_OUT is ~5e5 times
//    smaller than the 0.005 rounding quantum but far larger than this
//    computation's error, so it only decides genuine boundary cases.
//  * printf("%lf") is unreliable on POJ's compilers; "%f" is the portable
//    spelling for a double and is what is used here.
//  * "-0.00" is normalized to "0.00".
//  * G++ (language 0) is reported to TLE on this problem's I/O where C++
//    (language 4) runs in ~79MS, so this uses scanf/printf, not iostreams.
//
// Written for POJ's C++ (VC6): C headers unqualified, no reused for-scope
// loop variable.

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm>

const double EPS_MERGE = 1e-9; /* gaps below this are unresolvable at 2dp */
const double EPS_OUT = 1e-8;   /* forces half-away-from-zero rounding */

struct Interval {
    double lo, hi;
};

bool byLo(const Interval &a, const Interval &b) {
    if (a.lo != b.lo) return a.lo < b.lo;
    return a.hi < b.hi;
}

static void emit(double x) {
    char buf[512];
    x += (x < 0.0) ? -EPS_OUT : EPS_OUT;
    sprintf(buf, "%.2f", x);
    if (strcmp(buf, "-0.00") == 0) strcpy(buf, "0.00");
    fputs(buf, stdout);
}

static void emitPair(double lo, double hi) {
    emit(lo);
    fputc(' ', stdout);
    emit(hi);
    fputc('\n', stdout);
}

int main() {
    static Interval iv[512];
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        double bx, by;
        scanf("%lf %lf", &bx, &by);
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            double cx, cy, r;
            scanf("%lf %lf %lf", &cx, &cy, &r);
            double dx = cx - bx, dy = cy - by;
            double L = sqrt(dx * dx + dy * dy - r * r);
            double x1 = bx - by * (dx * L - dy * r) / (dx * r + dy * L);
            double x2 = bx - by * (dx * L + dy * r) / (dy * L - dx * r);
            iv[cnt].lo = (x1 < x2) ? x1 : x2;
            iv[cnt].hi = (x1 < x2) ? x2 : x1;
            ++cnt;
        }
        std::sort(iv, iv + cnt, byLo);
        double lo = iv[0].lo, hi = iv[0].hi;
        for (int k = 1; k < cnt; ++k) {
            if (iv[k].lo <= hi + EPS_MERGE) {
                if (iv[k].hi > hi) hi = iv[k].hi;
            } else {
                emitPair(lo, hi);
                lo = iv[k].lo;
                hi = iv[k].hi;
            }
        }
        emitPair(lo, hi);
        fputc('\n', stdout);
    }
    return 0;
}
