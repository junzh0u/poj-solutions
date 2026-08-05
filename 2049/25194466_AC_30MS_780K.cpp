// POJ 2049 - Finding Nemo
//
// Marlin starts at the point (0,0) and must reach Nemo at the float point
// (f1,f2), crossing as few doors as possible.  Walls are axis-parallel unit-grid
// segments and doors are unit-length openings cut into them, so everything lives
// on the integer lattice: a position is fully described by the unit cell it sits
// in.  Cell (x,y) is the square [x,x+1] x [y,y+1]; Marlin starts in cell (0,0)
// and Nemo is in cell (floor(f1), floor(f2)) -- the statement guarantees he is
// never on a wall or a door, so the floor is unambiguous.
//
// Each unit boundary between two adjacent cells gets a state: nothing (free),
// door (costs 1) or wall (impassable).  Walls are unrolled a unit at a time into
// hb[][] / vb[][], then the doors are applied on top, so a door always wins over
// the wall it was cut into and overlapping/touching walls are harmless.  The
// answer is then a shortest path with edge weights in {0,1}, computed by 0-1 BFS
// over a deque: a free step is pushed to the front, a door step to the back.
// Unreachable stays -1, which is exactly what the problem wants printed.
//
// Two things the statement only implies:
//  * Wall endpoints are confined to [1,199], so the bottom row, the left column,
//    row 199 and column 199 of cells carry no boundary at all.  Anything at or
//    beyond 199 in either coordinate is therefore reachable for free -- and f1/f2
//    are unbounded floats, so that case has to be short-circuited rather than
//    indexed into the grid.
//  * "t gives the length of the wall" is a count of unit segments, not a
//    coordinate, so a wall (x,y,0,t) blocks the t boundaries hb[x..x+t-1][y].
#include <cstdio>
#include <cstring>
#include <deque>

using namespace std;

/* Cells: unit square (x,y) covers [x,x+1] x [y,y+1], indices 0..N-1 (N = 200).
   hb[x][y]: the horizontal unit segment from (x,y) to (x+1,y)  -> separates cell (x,y-1) from (x,y)
   vb[x][y]: the vertical   unit segment from (x,y) to (x,y+1)  -> separates cell (x-1,y) from (x,y)
   value 0 = nothing, 1 = door (cost 1), 2 = wall (impassable) */

const int N = 200;

static int hb[N + 2][N + 2];
static int vb[N + 2][N + 2];
static int dist_[N][N];

int main() {
    int m, n;
    while (scanf("%d %d", &m, &n) == 2) {
        if (m < 0 && n < 0) break;
        memset(hb, 0, sizeof(hb));
        memset(vb, 0, sizeof(vb));
        int i;
        for (i = 0; i < m; ++i) {
            int x, y, d, t;
            scanf("%d %d %d %d", &x, &y, &d, &t);
            int k;
            for (k = 0; k < t; ++k) {
                if (d == 0) {
                    if (x + k >= 0 && x + k <= N + 1 && y >= 0 && y <= N + 1) hb[x + k][y] = 2;
                } else {
                    if (x >= 0 && x <= N + 1 && y + k >= 0 && y + k <= N + 1) vb[x][y + k] = 2;
                }
            }
        }
        for (i = 0; i < n; ++i) {
            int x, y, d;
            scanf("%d %d %d", &x, &y, &d);
            if (d == 0) {
                if (x >= 0 && x <= N + 1 && y >= 0 && y <= N + 1) hb[x][y] = 1;
            } else {
                if (x >= 0 && x <= N + 1 && y >= 0 && y <= N + 1) vb[x][y] = 1;
            }
        }
        double f1, f2;
        scanf("%lf %lf", &f1, &f2);

        int tx = (int)f1;
        int ty = (int)f2;
        if (f1 < 0) tx = 0;
        if (f2 < 0) ty = 0;
        if (tx >= N - 1 || ty >= N - 1) {
            /* walls live inside [1,199], so the outermost row/column of cells is
               always reachable from (0,0) without crossing anything */
            printf("0\n");
            continue;
        }

        int x, y;
        for (x = 0; x < N; ++x)
            for (y = 0; y < N; ++y) dist_[x][y] = -1;

        deque<int> dq;
        dist_[0][0] = 0;
        dq.push_back(0);
        while (!dq.empty()) {
            int cur = dq.front();
            dq.pop_front();
            int cx = cur / N, cy = cur % N;
            int dcur = dist_[cx][cy];
            int k;
            for (k = 0; k < 4; ++k) {
                int nx = cx, ny = cy, w;
                if (k == 0) { nx = cx + 1; w = vb[cx + 1][cy]; }
                else if (k == 1) { nx = cx - 1; w = vb[cx][cy]; }
                else if (k == 2) { ny = cy + 1; w = hb[cx][cy + 1]; }
                else { ny = cy - 1; w = hb[cx][cy]; }
                if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
                if (w == 2) continue;
                int nd = dcur + (w == 1 ? 1 : 0);
                if (dist_[nx][ny] == -1 || nd < dist_[nx][ny]) {
                    dist_[nx][ny] = nd;
                    if (w == 1) dq.push_back(nx * N + ny);
                    else dq.push_front(nx * N + ny);
                }
            }
        }
        printf("%d\n", dist_[tx][ty]);
    }
    return 0;
}
