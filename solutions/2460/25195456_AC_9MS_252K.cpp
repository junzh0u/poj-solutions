// POJ 2460 - Brownie Points I
// Model: claude-sonnet-5
// The point through which the lines are drawn is the point at the middle
// *position* of the input sequence for this test case (index (n-1)/2, 0-based,
// n is guaranteed odd) -- NOT a geometric/statistical median of the coordinates.
// This is confirmed both by the sample (center = 6th of 11 input points = (1,-3))
// and by the discuss board, which flags this exact misreading as the problem's
// main trap ("是输入序列中间的那个点，而不是平面中间那个点").
// A point crossed by either line (same x or same y as the center point) scores
// for nobody. Otherwise a point is Stan's (top-right/bottom-left, x and y on
// the same side of the center) or Ollie's (top-left/bottom-right, opposite
// sides). The board warns that computing (x-cx)*(y-cy) to compare signs can
// overflow; this avoids both that and even a plain subtraction overflow by
// comparing each coordinate against the center directly (no arithmetic).
#include <cstdio>

static int X[200005], Y[200005];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 0; i < n; ++i) {
            scanf("%d %d", &X[i], &Y[i]);
        }
        int c = (n - 1) / 2;
        int px = X[c], py = Y[c];
        int stan = 0, ollie = 0;
        for (int i = 0; i < n; ++i) {
            if (i == c) continue;
            if (X[i] == px || Y[i] == py) continue;
            bool xRight = X[i] > px;
            bool yUp = Y[i] > py;
            if (xRight == yUp) ++stan;
            else ++ollie;
        }
        printf("%d %d\n", stan, ollie);
    }
    return 0;
}
