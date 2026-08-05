// POJ 2653 - Pick-up sticks
//
// Sticks are dropped one at a time; a stick is "on top" if no later stick
// crosses it.  The naive rule is quadratic, but the statement guarantees at
// most 1000 top sticks, so keep only the sticks that are currently on top:
// for each new stick, drop from that list every stick it intersects, then
// append it.  The list never exceeds 1000 entries, so the work is bounded by
// n * 1000 = 1e8 segment tests in the worst case; each test is a handful of
// cross products, guarded by an axis-aligned bounding-box reject and an early
// exit as soon as one segment lies strictly on one side of the other's line.
// A deliberately adversarial case (1000 parallel sticks with mutually
// overlapping boxes plus 99000 sticks that force the full predicate) runs in
// 0.32s locally against a 3s limit.
//
// Intersection is the standard straddle test plus the four "endpoint lies on
// the other segment" cases, which covers collinear overlap, shared endpoints
// and zero-length sticks (a point stick counts as touching whatever it lands
// on).  Coordinates are reals in the input ("1 -2.0 8 4" in the sample), so
// exact arithmetic is not available and signs are taken through an epsilon.
//
// Statement ambiguity: "on top of" is not defined beyond the picture; the
// accepted reading is that any shared point at all — a crossing, a touch at
// an endpoint, or a collinear overlap — covers the earlier stick.  The output
// keeps the throwing order, and the list is printed with ", " separators and
// a trailing period even when a single stick is on top.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const double EPS = 1e-9;

static char buf[1 << 16];
static size_t blen = 0, bpos = 0;

static int gc() {
    if (bpos == blen) {
        blen = fread(buf, 1, sizeof(buf), stdin);
        bpos = 0;
        if (blen == 0) return -1;
    }
    return (unsigned char)buf[bpos++];
}

/* Reads a real number: [+-]digits[.digits][(e|E)[+-]digits] */
static bool readDouble(double &out) {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '+' || c == '-') { neg = (c == '-'); c = gc(); }
    double v = 0.0;
    while (c >= '0' && c <= '9') { v = v * 10.0 + (c - '0'); c = gc(); }
    if (c == '.') {
        c = gc();
        double scale = 1.0, frac = 0.0;
        while (c >= '0' && c <= '9') { frac = frac * 10.0 + (c - '0'); scale *= 10.0; c = gc(); }
        v += frac / scale;
    }
    if (c == 'e' || c == 'E') {
        c = gc();
        bool eneg = false;
        if (c == '+' || c == '-') { eneg = (c == '-'); c = gc(); }
        int ev = 0;
        while (c >= '0' && c <= '9') { ev = ev * 10 + (c - '0'); c = gc(); }
        double p = 1.0;
        for (int i = 0; i < ev; i++) p *= 10.0;
        v = eneg ? v / p : v * p;
    }
    out = neg ? -v : v;
    return true;
}

static inline int sgn(double x) { return x > EPS ? 1 : (x < -EPS ? -1 : 0); }

/* is R inside the bounding box of segment PQ (with tolerance)? */
static inline bool onBox(double px, double py, double qx, double qy,
                         double rx, double ry) {
    double lo, hi;
    lo = px < qx ? px : qx; hi = px < qx ? qx : px;
    if (rx < lo - EPS || rx > hi + EPS) return false;
    lo = py < qy ? py : qy; hi = py < qy ? qy : py;
    if (ry < lo - EPS || ry > hi + EPS) return false;
    return true;
}

/* Do segments AB and CD share at least one point?  abx/aby is B-A and
   cdx/cdy is D-C, both passed in because the caller has them already. */
static inline bool segInter(double ax, double ay, double bx, double by,
                            double abx, double aby,
                            double cx, double cy, double dx, double dy,
                            double cdx, double cdy) {
    int d1 = sgn(cdx * (ay - cy) - cdy * (ax - cx)); /* A vs line CD */
    int d2 = sgn(cdx * (by - cy) - cdy * (bx - cx)); /* B vs line CD */
    if (d1 != 0 && d1 == d2) return false;           /* AB strictly on one side */
    int d3 = sgn(abx * (cy - ay) - aby * (cx - ax)); /* C vs line AB */
    int d4 = sgn(abx * (dy - ay) - aby * (dx - ax)); /* D vs line AB */
    if (d3 != 0 && d3 == d4) return false;
    if (d1 * d2 < 0 && d3 * d4 < 0) return true;     /* proper crossing */
    /* touching / collinear: an endpoint must lie on the other segment */
    if (d1 == 0 && onBox(cx, cy, dx, dy, ax, ay)) return true;
    if (d2 == 0 && onBox(cx, cy, dx, dy, bx, by)) return true;
    if (d3 == 0 && onBox(ax, ay, bx, by, cx, cy)) return true;
    if (d4 == 0 && onBox(ax, ay, bx, by, dx, dy)) return true;
    return false;
}

/* current top sticks; the statement guarantees at most 1000 */
static const int MAXTOP = 100005;
static double tx1[MAXTOP], ty1[MAXTOP], tx2[MAXTOP], ty2[MAXTOP];
static double tdx[MAXTOP], tdy[MAXTOP];
static double tlox[MAXTOP], thix[MAXTOP], tloy[MAXTOP], thiy[MAXTOP];
static int tid[MAXTOP];

int main() {
    double n_d;
    while (readDouble(n_d)) {
        int n = (int)(n_d + 0.5);
        if (n <= 0) break;
        int top = 0;
        for (int i = 1; i <= n; i++) {
            double ax, ay, bx, by;
            readDouble(ax); readDouble(ay); readDouble(bx); readDouble(by);
            double abx = bx - ax, aby = by - ay;
            double lox = (ax < bx ? ax : bx) - EPS, hix = (ax < bx ? bx : ax) + EPS;
            double loy = (ay < by ? ay : by) - EPS, hiy = (ay < by ? by : ay) + EPS;

            int k = 0;
            for (int j = 0; j < top; j++) {
                if (!(tlox[j] > hix || thix[j] < lox || tloy[j] > hiy || thiy[j] < loy) &&
                    segInter(ax, ay, bx, by, abx, aby,
                             tx1[j], ty1[j], tx2[j], ty2[j], tdx[j], tdy[j]))
                    continue; /* stick j is crossed by stick i -> no longer on top */
                if (k != j) {
                    tx1[k] = tx1[j]; ty1[k] = ty1[j];
                    tx2[k] = tx2[j]; ty2[k] = ty2[j];
                    tdx[k] = tdx[j]; tdy[k] = tdy[j];
                    tlox[k] = tlox[j]; thix[k] = thix[j];
                    tloy[k] = tloy[j]; thiy[k] = thiy[j];
                    tid[k] = tid[j];
                }
                k++;
            }
            top = k;
            tx1[top] = ax; ty1[top] = ay; tx2[top] = bx; ty2[top] = by;
            tdx[top] = abx; tdy[top] = aby;
            tlox[top] = lox; thix[top] = hix; tloy[top] = loy; thiy[top] = hiy;
            tid[top] = i;
            top++;
        }
        printf("Top sticks:");
        for (int j = 0; j < top; j++)
            printf(" %d%s", tid[j], j + 1 == top ? "." : ",");
        printf("\n");
    }
    return 0;
}
