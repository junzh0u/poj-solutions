// POJ 1106 - Transmitters
//
// A transmitter at T covers a half-disc of radius r that may be rotated
// freely; maximise the number of the N (<= 150) given points it covers.
//
// Two independent filters:
//   1. Range. Translate every point to v = P - T and keep only those with
//      |v|^2 <= r^2. Coordinates are integers, so |v|^2 is an integer and
//      exact in a double; r is real, so the comparison is done against
//      r*r with a 1e-6 slack (points exactly on the arc count as covered,
//      and r*r is only accurate to ~1e-10 here).
//   2. Half-plane. Whatever survives the range filter is covered iff it
//      lies in the closed half-plane through T bounded by the diameter.
//
// Only O(n) rotations matter: take an optimal half-plane and rotate it
// counter-clockwise until its leading edge meets a point. Nothing is lost
// on the way (the leading edge only gains points, the trailing edge only
// sheds points once it passes them, and it is stopped before it does), so
// some optimum has a point exactly on its boundary ray. Hence: for every
// surviving v_i, count the v_j with cross(v_i, v_j) >= 0, and take the
// maximum. O(n^2) per case, ~22k operations at n = 150.
//
// The cross products stay exact: the components are integers below 1001 in
// absolute value, so every product and difference is representable, and the
// ">= 0" test needs no epsilon. That matters because ties are the whole
// problem -- points on the dividing line are inside the semicircle, as are
// points exactly at distance r.
//
// Input ends at a line with a negative radius, whose x and y are garbage
// and which is not followed by a point count. Points out of range simply
// vanish, so a transmitter that reaches nothing prints 0.

#include <cstdio>

int main() {
    double tx, ty, r;
    while (scanf("%lf %lf %lf", &tx, &ty, &r) == 3) {
        if (r < 0) break;
        int n;
        if (scanf("%d", &n) != 1) break;
        static double vx[200], vy[200];
        int m = 0;
        double rr = r * r + 1e-6;
        for (int i = 0; i < n; ++i) {
            double px, py;
            if (scanf("%lf %lf", &px, &py) != 2) return 0;
            double dx = px - tx, dy = py - ty;
            if (dx * dx + dy * dy <= rr) {
                vx[m] = dx;
                vy[m] = dy;
                ++m;
            }
        }
        int best = 0;
        for (int i = 0; i < m; ++i) {
            int cnt = 0;
            for (int j = 0; j < m; ++j) {
                if (vx[i] * vy[j] - vy[i] * vx[j] >= 0) ++cnt;
            }
            if (cnt > best) best = cnt;
        }
        printf("%d\n", best);
    }
    return 0;
}
