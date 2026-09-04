// POJ 1371 - Tin Cutter
// Model: claude-sonnet-5
//
// Approach: coordinate compression + "doubled grid" flood fill.
// All cuts are axis-aligned segments. Compress the distinct x and y
// coordinates that appear as endpoints, add one padding coordinate on
// each side (guaranteed to be outside every cut), then build a grid
// where each original coordinate maps to an even index and the gap
// between two consecutive coordinates maps to an odd index. Marking a
// cut then blocks a contiguous run of grid cells along one row/column,
// including the endpoint cells and every gap cell strictly between
// them (this correctly represents the whole closed segment, endpoints
// included). Flood fill from the padding border (guaranteed open)
// reaches everything connected to the outside; every other cell is
// "removed" (its material fell out).
//
// Ambiguity (settled from the statement's own example figure,
// images/1371_1.jpg, since the two given samples don't distinguish
// the reading): a "hole" is an OPENING in the remaining plate, not a
// fallen piece. Two enclosed regions that only touch at a single
// point (e.g. two squares sharing one corner) are two separate holes,
// but two enclosed regions separated by a cut that has removed
// material on *both* sides (e.g. two full rectangle outlines crossing
// like a plus, or two overlapping rectangle outlines) are the SAME
// hole, because that internal cut no longer borders any remaining
// material and is invisible in the final plate's shape - even though
// it did separate the fallen material into more than one physical
// piece. The figure's four cases (two touching squares = 2 holes; two
// squares joined by a bridge cut = 2 holes; two full rectangles
// crossing in a plus = 1 hole; two overlapping rectangle outlines = 1
// hole) all confirm this and were reproduced locally before
// submitting. Concretely: in the doubled grid, never step through a
// blocked VERTEX cell (both compressed indices even - a real cut
// vertex, where regions should stay separate even if they touch), but
// DO step through a blocked EDGE cell (exactly one index even - a
// piece of a cut's interior) whenever it is unreached, which merges
// the two flanking area cells into one hole. A first submission using
// plain "count connected components of unreached cells, walls
// included" (equivalent to E-V+C via Euler's formula on the planar
// arrangement) got Wrong Answer, which is what exposed the need for
// this distinction - both a from-scratch differential reference and
// the grid method agreed with each other under the wrong reading, so
// only the figure caught it.
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

const int MAXN = 100;
const int MAXD = 2 * (2 * MAXN + 2) + 5; // doubled grid dimension bound

bool blocked[MAXD][MAXD];
bool visited[MAXD][MAXD];
int rows, cols;

int qx[MAXD * MAXD], qy[MAXD * MAXD];

void floodFromOutside() {
    int head = 0, tail = 0;
    qx[tail] = 0; qy[tail] = 0; tail++;
    visited[0][0] = true;
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    while (head < tail) {
        int r = qx[head], c = qy[head]; head++;
        for (int d = 0; d < 4; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (visited[nr][nc] || blocked[nr][nc]) continue;
            visited[nr][nc] = true;
            qx[tail] = nr; qy[tail] = nc; tail++;
        }
    }
}

bool visited2[MAXD][MAXD];

// A cell is a "vertex" cell iff both compressed coordinates are even
// (it sits at an actual grid point, i.e. an original or padding
// coordinate on both axes). Everything else (exactly one index even)
// is a piece of a cut's interior ("edge" cell) or, when both indices
// are odd, a genuine 2D area cell.
inline bool isVertex(int r, int c) { return (r % 2 == 0) && (c % 2 == 0); }

int countHoles() {
    int holes = 0;
    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (visited[r][c] || visited2[r][c]) continue;
            if (isVertex(r, c) && blocked[r][c]) continue; // never start from a cut vertex
            // BFS this component: pass through anything unreached, except
            // a blocked vertex cell (that would wrongly bridge regions
            // that only touch at a point, e.g. two squares sharing a
            // corner).
            int head = 0, tail = 0;
            qx[tail] = r; qy[tail] = c; tail++;
            visited2[r][c] = true;
            bool hasArea = false;
            while (head < tail) {
                int cr = qx[head], cc = qy[head]; head++;
                if ((cr & 1) && (cc & 1)) hasArea = true;
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dx[d], nc = cc + dy[d];
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                    if (visited[nr][nc] || visited2[nr][nc]) continue;
                    if (isVertex(nr, nc) && blocked[nr][nc]) continue;
                    visited2[nr][nc] = true;
                    qx[tail] = nr; qy[tail] = nc; tail++;
                }
            }
            if (hasArea) holes++;
        }
    }
    return holes;
}

int idxOf(const vector<int>& v, int val) {
    // binary search for exact value
    int lo = 0, hi = (int)v.size() - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (v[mid] == val) return mid;
        else if (v[mid] < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1; // should not happen
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<int> x1s(n), y1s(n), x2s(n), y2s(n);
        vector<int> xs, ys;
        for (int i = 0; i < n; i++) {
            scanf("%d %d %d %d", &x1s[i], &y1s[i], &x2s[i], &y2s[i]);
            xs.push_back(x1s[i]);
            xs.push_back(x2s[i]);
            ys.push_back(y1s[i]);
            ys.push_back(y2s[i]);
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        // add padding on both sides
        vector<int> xsE, ysE;
        xsE.push_back(xs.front() - 1);
        for (size_t i = 0; i < xs.size(); i++) xsE.push_back(xs[i]);
        xsE.push_back(xs.back() + 1);

        ysE.push_back(ys.front() - 1);
        for (size_t i = 0; i < ys.size(); i++) ysE.push_back(ys[i]);
        ysE.push_back(ys.back() + 1);

        int nx = (int)xsE.size();
        int ny = (int)ysE.size();
        rows = 2 * ny - 1;
        cols = 2 * nx - 1;

        for (int r = 0; r < rows; r++) {
            memset(blocked[r], 0, sizeof(bool) * cols);
            memset(visited[r], 0, sizeof(bool) * cols);
            memset(visited2[r], 0, sizeof(bool) * cols);
        }

        for (int i = 0; i < n; i++) {
            int ix1 = idxOf(xsE, x1s[i]);
            int ix2 = idxOf(xsE, x2s[i]);
            int iy1 = idxOf(ysE, y1s[i]);
            int iy2 = idxOf(ysE, y2s[i]);
            if (y1s[i] == y2s[i]) {
                // horizontal
                int row = 2 * iy1;
                int c1 = 2 * min(ix1, ix2);
                int c2 = 2 * max(ix1, ix2);
                for (int c = c1; c <= c2; c++) blocked[row][c] = true;
            } else {
                // vertical
                int col = 2 * ix1;
                int r1 = 2 * min(iy1, iy2);
                int r2 = 2 * max(iy1, iy2);
                for (int r = r1; r <= r2; r++) blocked[r][col] = true;
            }
        }

        floodFromOutside();
        int holes = countHoles();
        printf("%d\n", holes);
    }
    return 0;
}
