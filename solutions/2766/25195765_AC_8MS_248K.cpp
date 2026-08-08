// POJ 2766 - Laserbox
// Model: claude-opus-5
//
// Direct simulation. x is the column and y the row, with y = 0 the bottom edge
// and y = n+1 the top, x = 0 the start (west end) of a row and x = n+1 its end.
// So a laser given as (a, 0) fires north, (a, n+1) south, (0, b) east and
// (n+1, b) west; the tested y-coordinate is what disambiguates, since a legal
// laser has exactly one coordinate equal to 0 or n+1 and the other in 1..n.
// Walk the beam one cell at a time, turning right at every right-turner
// ((dx,dy) -> (dy,-dx), i.e. N->E->S->W->N), and print the first cell reached
// outside 1..n in either axis. n <= 50 and r <= 50, so this is trivial.
//
// On "0 0": the beam can never actually be caught, so that branch is a
// formality kept for the statement's sake. The step map
// (turner, incoming direction) -> (next turner, direction after turning) is
// injective -- the direction before the turn is determined by the one after it,
// and the predecessor turner is the nearest one back along that ray -- so a
// trajectory can only repeat a state if it started on the cycle. A beam launched
// from the border enters at the first turner along its ray, which by definition
// has no turner behind it and hence no predecessor state, so it cannot lie on a
// cycle. Cycles do exist (four turners at the corners of a rectangle form one),
// they are simply unreachable from outside; the discuss board's one substantive
// remark is that no test case produces 0 0, which agrees. The loop cap below is
// ~5.6e5 steps against a true worst case of 4*50 states * 51 cells ~= 1.0e4.

#include <cstdio>
#include <cstring>

static bool grid[64][64];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int n, r;
        scanf("%d %d", &n, &r);
        memset(grid, 0, sizeof(grid));
        for (int i = 0; i < r; ++i) {
            int x, y;
            scanf("%d %d", &x, &y);
            if (x >= 1 && x <= n && y >= 1 && y <= n) grid[x][y] = true;
        }
        int lx, ly;
        scanf("%d %d", &lx, &ly);

        int x = lx, y = ly, dx = 0, dy = 0;
        if (ly == 0)          { dx = 0;  dy = 1;  }   /* bottom of a column: north */
        else if (ly == n + 1) { dx = 0;  dy = -1; }   /* top of a column:    south */
        else if (lx == 0)     { dx = 1;  dy = 0;  }   /* start of a row:     east  */
        else                  { dx = -1; dy = 0;  }   /* end of a row:       west  */

        int steps = 0, cap = 4 * (n + 2) * (n + 2) * (n + 2) + 100;
        bool out = false;
        while (steps++ < cap) {
            x += dx; y += dy;
            if (x < 1 || x > n || y < 1 || y > n) { out = true; break; }
            if (grid[x][y]) { int ndx = dy, ndy = -dx; dx = ndx; dy = ndy; }
        }
        if (out) printf("%d %d\n", x, y);
        else printf("0 0\n");
    }
    return 0;
}
