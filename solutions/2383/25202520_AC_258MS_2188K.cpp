// POJ 2383 - Circle Drawing
// Model: claude-sonnet-5
// Approach: rasterize each circle with a per-scanline range (integer sqrt of
// R^2-dy^2 gives the half-width at each row) and memset that row segment to
// the circle's color digit. Circles are painted in input order so later
// circles overwrite earlier ones, matching "after drawing N given circles".
// Statement note: R can be up to 212 (board confirms description's "<=200"
// was a typo/adjustment), and the time limit is tight enough that a naive
// per-pixel loop over the bounding box (checking every pixel's distance)
// risks TLE; the scanline+memset approach keeps work proportional to the
// number of scanlines times memset's fast bulk fill instead of a per-pixel
// branch.
#include <cstdio>
#include <cstring>
#include <cmath>

static char grid[1000][1000];

int main(){
    int Xs, Ys, N;
    if (scanf("%d %d %d", &Xs, &Ys, &N) != 3) return 0;
    for (int y = 0; y < Ys; ++y) memset(grid[y], '0', Xs);

    for (int i = 0; i < N; ++i) {
        int xc, yc, R, c;
        scanf("%d %d %d %d", &xc, &yc, &R, &c);
        char ch = (char)('0' + c);
        int R2 = R * R; /* R <= 212, so R2 <= 44944: fits comfortably in int */

        int yStart = yc - R; if (yStart < 0) yStart = 0;
        int yEnd = yc + R; if (yEnd > Ys - 1) yEnd = Ys - 1;

        for (int y = yStart; y <= yEnd; ++y) {
            int dy = y - yc;
            int rem = R2 - dy * dy;
            if (rem < 0) continue;
            int dx = (int)sqrt((double)rem);
            while (dx * dx > rem) --dx;
            while ((dx + 1) * (dx + 1) <= rem) ++dx;
            int xStart = xc - dx; if (xStart < 0) xStart = 0;
            int xEnd = xc + dx; if (xEnd > Xs - 1) xEnd = Xs - 1;
            if (xStart <= xEnd) memset(grid[y] + xStart, ch, xEnd - xStart + 1);
        }
    }

    for (int y = 0; y < Ys; ++y) {
        fwrite(grid[y], 1, Xs, stdout);
        putchar('\n');
    }
    return 0;
}
