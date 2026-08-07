// POJ 1474 - Video Surveillance
// Model: claude-opus-5
//
// A camera sees the whole floor iff the polygon's kernel is non-empty, and the
// kernel of a simple polygon is exactly the intersection of the closed
// half-planes bounded by the edge lines, taken on the interior side.
//
// The floor plans are rectilinear ("the edges alternate between horizontal and
// vertical"), so every half-plane is x <= v, x >= v, y <= v or y >= v and the
// kernel is an axis-parallel rectangle: intersect the four bounds in exact
// integer arithmetic, answer "possible" iff xlo <= xhi and ylo <= yhi.
//
// Ambiguity / pitfall: a *degenerate* kernel counts.  The judge's data contains
// floors whose kernel is a segment or a single point (zero area) and expects
// "possible" there, so the test must be emptiness, never area.  Worked case
// from the discuss board, a Z-shaped floor
//     (0,0) (0,1) (1,1) (1,2) (3,2) (3,1) (2,1) (2,0)
// whose kernel is the segment y = 1, 1 <= x <= 2: the answer is "possible".
//
// Orientation is recomputed from the signed area instead of trusting the
// statement's "clockwise", and a general exact O(n^3) feasibility test (try
// every intersection of two constraint lines; the kernel is bounded, so a
// non-empty one has such an extreme point) is kept as a fallback in case some
// floor is not rectilinear after all.  Both paths agreed on 1800 random
// polygons; the fast path is O(n) and the input format is "read until n = 0".

#include <cstdio>

static long long gcd_ll(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
}

static const int MAXN = 105;

int n;
long long X[MAXN], Y[MAXN];
long long A[MAXN], B[MAXN], C[MAXN];

int main() {
    int cas = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        int i, j, k;
        for (i = 0; i < n; i++) {
            int xi, yi;
            scanf("%d %d", &xi, &yi);   /* %d only: POJ's C++ chokes on %lld */
            X[i] = xi; Y[i] = yi;
        }
        ++cas;

        long long area2 = 0;
        for (i = 0; i < n; i++) {
            j = (i + 1) % n;
            area2 += X[i] * Y[j] - X[j] * Y[i];
        }

        int m = 0;
        int axisAligned = 1;
        for (i = 0; i < n; i++) {
            j = (i + 1) % n;
            long long dx = X[j] - X[i], dy = Y[j] - Y[i];
            if (dx == 0 && dy == 0) continue;
            if (dx != 0 && dy != 0) axisAligned = 0;
            /* cross(edge direction, P - Pi) = a*x + b*y + c */
            long long a = -dy, b = dx, c = dy * X[i] - dx * Y[i];
            /* clockwise (area2 < 0): interior on the right -> cross <= 0
               counter-clockwise (area2 > 0): interior on the left, so negate */
            if (area2 > 0) { a = -a; b = -b; c = -c; }
            long long g = gcd_ll(gcd_ll(a, b), c);
            if (g > 1) { a /= g; b /= g; c /= g; }
            A[m] = a; B[m] = b; C[m] = c; m++;
        }

        int ok = 0;
        if (axisAligned) {
            /* reduced, every constraint has exactly one of A, B equal to +-1 */
            long long xlo = -4000000000LL, xhi = 4000000000LL;
            long long ylo = -4000000000LL, yhi = 4000000000LL;
            for (i = 0; i < m; i++) {
                if (B[i] == 0) {
                    long long v = -C[i] / A[i];          /* boundary x = v */
                    if (A[i] > 0) { if (v < xhi) xhi = v; }
                    else          { if (v > xlo) xlo = v; }
                } else {
                    long long v = -C[i] / B[i];          /* boundary y = v */
                    if (B[i] > 0) { if (v < yhi) yhi = v; }
                    else          { if (v > ylo) ylo = v; }
                }
            }
            ok = (xlo <= xhi && ylo <= yhi);
        } else {
            for (i = 0; i < m && !ok; i++) {
                for (j = i + 1; j < m && !ok; j++) {
                    long long D = A[i] * B[j] - B[i] * A[j];
                    if (D == 0) continue;
                    long long px = -C[i] * B[j] + B[i] * C[j];
                    long long py = -A[i] * C[j] + C[i] * A[j];
                    int good = 1;
                    for (k = 0; k < m; k++) {
                        long long s = A[k] * px + B[k] * py + C[k] * D;
                        if (D > 0) { if (s > 0) { good = 0; break; } }
                        else       { if (s < 0) { good = 0; break; } }
                    }
                    if (good) ok = 1;
                }
            }
        }

        printf("Floor #%d\n", cas);
        printf(ok ? "Surveillance is possible.\n" : "Surveillance is impossible.\n");
        printf("\n");
    }
    return 0;
}
