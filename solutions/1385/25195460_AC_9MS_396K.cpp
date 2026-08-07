// POJ 1385 - Lifting the Stone
// Model: claude-sonnet-5
// Approach: streaming polygon centroid of gravity via the shoelace-weighted
// formula: with cross_i = x_i*y_{i+1} - x_{i+1}*y_i,
//   6A  = sum(cross_i)
//   Cx  = sum((x_i+x_{i+1})*cross_i) / (3 * sum(cross_i))
//   Cy  = sum((y_i+y_{i+1})*cross_i) / (3 * sum(cross_i))
// N goes up to 1e6 against only a 10000K memory limit, so points are never
// stored in an array -- only the first point and the running previous point
// are kept, and the sums are accumulated as each point streams in.
// Accumulation uses long double: each cross_i is an exact integer well
// inside a double's 53-bit mantissa, but the weighted sum over up to 1e6
// edges can in principle exceed exact-double range, and the discuss board
// warns about this directly ("floating point error accumulation -- even a
// tiny error times 1000000 is a big number"), so the wider mantissa is a
// cheap safety margin; the final divide is cast down to plain double only
// for printing. The board also flags two more pitfalls that were followed:
// output can round to "-0.00" (fixed by clamping |v|<0.005 to +0.0 before
// printing), and one thread reports %.2lf misbehaving under G++ where %.2f
// works, so the final printf uses %f rather than %lf/%Lf.
#include <cstdio>
#include <cmath>

static char buf[1 << 20];
static int bufLen = 0, bufPos = 0;

static inline int gc() {
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen == 0) return -1;
    }
    return buf[bufPos++];
}

static inline int readInt() {
    int c = gc();
    while (c != '-' && (c < '0' || c > '9')) c = gc();
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x * sign;
}

int main() {
    int T = readInt();
    while (T--) {
        int n = readInt();
        int x0 = readInt(), y0 = readInt();
        int px = x0, py = y0;
        long double areaSum = 0.0L, cxSum = 0.0L, cySum = 0.0L;
        for (int i = 1; i < n; i++) {
            int x = readInt(), y = readInt();
            long double cross = (long double)px * y - (long double)x * py;
            areaSum += cross;
            cxSum += (long double)(px + x) * cross;
            cySum += (long double)(py + y) * cross;
            px = x; py = y;
        }
        long double cross = (long double)px * y0 - (long double)x0 * py;
        areaSum += cross;
        cxSum += (long double)(px + x0) * cross;
        cySum += (long double)(py + y0) * cross;

        double cx = (double)(cxSum / (3.0L * areaSum));
        double cy = (double)(cySum / (3.0L * areaSum));
        if (fabs(cx) < 0.005) cx = 0.0;
        if (fabs(cy) < 0.005) cy = 0.0;
        printf("%.2f %.2f\n", cx, cy);
    }
    return 0;
}
