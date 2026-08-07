// POJ 2641 - Billiard
// Model: claude-sonnet-5
//
// Approach: unfold the reflections. A ball bouncing inside an a x b
// rectangle, launched from the exact center, traces a straight line in
// the "unfolded" plane tiled by mirrored copies of the rectangle. Because
// the launch point is the center of the cell, reflecting it across any
// cell boundary lands it back on the center of the neighboring cell (a
// center is a fixed point of the reflection up to relabeling), so the
// center of every tile in the infinite grid is an image of the start
// point, regardless of orientation.
//
// After m bounces off the vertical sides the path has crossed m vertical
// grid lines, landing in the tile that is m cells over horizontally, so
// the horizontal displacement in the unfolded plane is dx = m*a. Likewise
// n bounces off the horizontal sides give dy = n*b. The ball returns to
// (an image of) its start after traveling the straight-line distance
// dist = sqrt(dx^2 + dy^2) in the unfolded plane, which equals the true
// path length in the folded table. Speed is distance over time:
// v = dist / s. The launch angle from the horizontal is the direction of
// that straight segment: A = atan2(dy, dx), which already lands in
// [0, 90] degrees since a, b, m, n > 0 makes dx, dy > 0.
//
// Verified against all three sample lines exactly (45.00/141.42,
// 33.69/144.22, 3.09/7967.81). Statement's "five nonnegative integers"
// vs. "positive integers not greater than 10000" only conflict on the
// all-zero terminator line; real data lines have every field >= 1, so
// dx, dy are always strictly positive and atan2 never sees the origin.
// Board flagged C's hypot() as occasionally flaky and recommended
// avoiding cout<<setprecision in favor of printf for this precision-
// sensitive output; used sqrt(dx*dx+dy*dy) directly and printf here.

#include <cstdio>
#include <cmath>

int main() {
    double a, b, s, m, n;
    while (scanf("%lf %lf %lf %lf %lf", &a, &b, &s, &m, &n) == 5) {
        if (a == 0 && b == 0 && s == 0 && m == 0 && n == 0) break;
        double dx = a * m;
        double dy = b * n;
        double dist = sqrt(dx * dx + dy * dy);
        double v = dist / s;
        double ang = atan2(dy, dx) * 180.0 / acos(-1.0);
        printf("%.2f %.2f\n", ang, v);
    }
    return 0;
}
