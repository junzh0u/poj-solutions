// POJ 2225 - Asteroids!
// Model: claude-opus-5
//
// Plain 3-D breadth-first search on an N x N x N grid (N <= 10, up to 100 data
// sets, read to EOF), six axis-aligned neighbours, unit cost.  At most 1000
// cells per set, so the whole run is trivial; the only thing that can go wrong
// here is the indexing.
//
// Coordinate convention (the one trap, and the board's most repeated WA):
// the slice list arrives slice-major, then row, then column, i.e. the r-th line
// of the z-th block gives grid[z][r][*].  But a *point* "A B C" is given as
// column, row, slice -- the statement says so explicitly ("The first coordinate
// ... indicates the column ... second ... row ... third ... slice"), so a point
// (A,B,C) addresses grid[C][B][A].  Reading a point as (row, column, slice)
// instead is equivalent to transposing the field without transposing the query,
// and all three official samples are invariant under an x/y swap, so the sample
// cannot catch it.  Verified instead on hand-built asymmetric cases whose
// answers differ between the two readings.
//
// Start and target are guaranteed to be in empty space.

#include <cstdio>
#include <cstring>

static char grid[10][10][16];
static int dist_[10][10][10];
static int qx[1000], qy[1000], qz[1000];

int main() {
    char tok[64];
    while (scanf("%63s", tok) == 1) {
        if (strcmp(tok, "START") != 0) continue;
        int n;
        if (scanf("%d", &n) != 1) break;

        int z, r, i;
        for (z = 0; z < n; ++z)
            for (r = 0; r < n; ++r)
                if (scanf("%15s", grid[z][r]) != 1) return 0;

        int a, b, c, d, e, f;
        if (scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f) != 6) return 0;
        /* consume the "END" line */
        if (scanf("%63s", tok) != 1) return 0;

        /* (col, row, slice) -> grid[slice][row][col] */
        int sx = a, sy = b, sz = c;   /* x = column, y = row, z = slice */
        int tx = d, ty = e, tz = f;

        for (z = 0; z < n; ++z)
            for (r = 0; r < n; ++r)
                for (i = 0; i < n; ++i)
                    dist_[z][r][i] = -1;

        int ans = -1;
        if (grid[sz][sy][sx] != 'X' && grid[tz][ty][tx] != 'X') {
            int head = 0, tail = 0;
            dist_[sz][sy][sx] = 0;
            qx[tail] = sx; qy[tail] = sy; qz[tail] = sz; ++tail;
            static const int dx[6] = { 1, -1, 0, 0, 0, 0 };
            static const int dy[6] = { 0, 0, 1, -1, 0, 0 };
            static const int dz[6] = { 0, 0, 0, 0, 1, -1 };
            while (head < tail) {
                int cx = qx[head], cy = qy[head], cz = qz[head];
                ++head;
                if (cx == tx && cy == ty && cz == tz) { ans = dist_[cz][cy][cx]; break; }
                for (i = 0; i < 6; ++i) {
                    int nx = cx + dx[i], ny = cy + dy[i], nz = cz + dz[i];
                    if (nx < 0 || nx >= n || ny < 0 || ny >= n || nz < 0 || nz >= n) continue;
                    if (grid[nz][ny][nx] == 'X') continue;
                    if (dist_[nz][ny][nx] != -1) continue;
                    dist_[nz][ny][nx] = dist_[cz][cy][cx] + 1;
                    qx[tail] = nx; qy[tail] = ny; qz[tail] = nz; ++tail;
                }
            }
        }

        if (ans < 0) printf("NO ROUTE\n");
        else printf("%d %d\n", n, ans);
    }
    return 0;
}
