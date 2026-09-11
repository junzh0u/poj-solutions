// POJ 3813 - Euclid
// Model: claude-sonnet-5
//
// H lies on ray AC, so angle CAB = angle HAB holds automatically for ANY
// point on that ray; the real constraint is just H = A + t*(C-A) for some
// scalar t >= 0. ABGH is a parallelogram with A,B,G,H in order, so its two
// sides from vertex A are AB and AH, giving G = B + (H - A) and
// area(ABGH) = |AB x AH| = t * |AB x AC|.
//
// Setting that equal to area(DEF) = 0.5*|DE x DF| gives
//   t = (0.5*|DE x DF|) / |AB x AC|
// which is exactly the k = S(DEF) / (2*S(ABC)) posted on the discuss board
// (message 353250); confirmed independently by matching the first sample
// case by hand (t = 4/25 = 0.16, giving H=(0,0.8), G=(5,0.8)).
//
// Input is terminated by a line of twelve exact 0.0 values; read until EOF
// and stop on that literal all-zero line.

#include <cstdio>

int main() {
    double ax, ay, bx, by, cx, cy, dx, dy, ex, ey, fx, fy;
    while (scanf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                  &ax, &ay, &bx, &by, &cx, &cy,
                  &dx, &dy, &ex, &ey, &fx, &fy) == 12) {
        if (ax == 0 && ay == 0 && bx == 0 && by == 0 && cx == 0 && cy == 0 &&
            dx == 0 && dy == 0 && ex == 0 && ey == 0 && fx == 0 && fy == 0) {
            break;
        }
        double abx = bx - ax, aby = by - ay;
        double acx = cx - ax, acy = cy - ay;
        double cross_abc = abx * acy - aby * acx;
        double dex = ex - dx, dey = ey - dy;
        double dfx = fx - dx, dfy = fy - dy;
        double cross_def = dex * dfy - dey * dfx;
        double area_def = 0.5 * (cross_def < 0 ? -cross_def : cross_def);
        double denom = cross_abc < 0 ? -cross_abc : cross_abc;
        double t = area_def / denom;
        double hx = ax + t * acx, hy = ay + t * acy;
        double gx = bx + t * acx, gy = by + t * acy;
        printf("%.3f %.3f %.3f %.3f\n", gx, gy, hx, hy);
    }
    return 0;
}
