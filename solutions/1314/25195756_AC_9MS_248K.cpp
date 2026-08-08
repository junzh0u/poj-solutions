// POJ 1314 - Finding Rectangles
// Model: claude-opus-5
//
// Find every axis-parallel rectangle whose four corners are input points.
// Instead of enumerating 4-subsets, enumerate the diagonal: each rectangle is
// determined by its upper-left and lower-right corners, so for every ordered
// pair (i, j) with px[i] < px[j] and py[i] > py[j], look up the two remaining
// corners (px[j], py[i]) and (px[i], py[j]) in a 50x50 occupancy grid keyed by
// label.  Coordinates are integers below 50, so the grid is exact and the whole
// scan is O(n^2) per set with n <= 26.  Each rectangle is produced exactly once
// (one diagonal pair), so no de-duplication is needed.
//
// Orientation: larger y is up.  The sample fixes this directly -- rectangle
// DECB has D(2,3) upper left and C(3,1) lower right -- and figure 1314_1.jpg
// agrees.  Corners are emitted clockwise from the upper left: UL, UR, LR, LL.
//
// Output shape, which is what this problem's discuss board is entirely about
// (repeated Presentation Errors, "missed one space"): "Point set N:" then, if
// there are none, " No rectangles" on that same line; otherwise the rectangles
// follow starting on the next line, each preceded by a blank, ten per line, and
// no trailing blank.  Every set ends with a newline.
//
// The rectangles must be listed in alphabetical order, and that ordering is the
// one requirement the official sample cannot test: on all four sample sets the
// natural enumeration order already happens to be sorted, so dropping the sort
// reproduces the sample byte-for-byte.  Separating case (kept as test_data):
// points B(2,0) C(0,1) H(0,0) N(1,1) R(1,0) S(2,1) must print
// "CNRH CSBH NSBR", while enumeration order gives "CSBH CNRH NSBR".
//
// Sorting uses qsort/strcmp on fixed char buffers rather than
// sort(vector<string>), which the board reports POJ's C++ compiler mishandling.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static char grid[64][64];
static char lbl[32];
static int px[32], py[32];
static char rect[1024][8]; /* a 5x5 lattice maximises this at 100 */
static int nrect;

static int cmpstr(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

int main() {
    int n, setno = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        int i, j;
        ++setno;
        memset(grid, 0, sizeof(grid));
        for (i = 0; i < n; ++i) {
            char c;
            int x, y;
            scanf(" %c %d %d", &c, &x, &y);
            lbl[i] = c;
            px[i] = x;
            py[i] = y;
            grid[x][y] = c;
        }
        nrect = 0;
        /* i = upper-left corner, j = lower-right corner */
        for (i = 0; i < n; ++i) {
            for (j = 0; j < n; ++j) {
                if (px[i] >= px[j] || py[i] <= py[j]) continue;
                char ur = grid[px[j]][py[i]]; /* upper right */
                char ll = grid[px[i]][py[j]]; /* lower left  */
                if (ur == 0 || ll == 0) continue;
                rect[nrect][0] = lbl[i];
                rect[nrect][1] = ur;
                rect[nrect][2] = lbl[j];
                rect[nrect][3] = ll;
                rect[nrect][4] = '\0';
                ++nrect;
            }
        }
        qsort(rect, nrect, sizeof(rect[0]), cmpstr);
        printf("Point set %d:", setno);
        if (nrect == 0) {
            printf(" No rectangles");
        } else {
            for (i = 0; i < nrect; ++i) {
                if (i % 10 == 0) printf("\n");
                printf(" %s", rect[i]);
            }
        }
        printf("\n");
    }
    return 0;
}
