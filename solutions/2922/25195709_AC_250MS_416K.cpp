// POJ 2922 - Honeymoon Hike
// Model: claude-opus-5
//
// Minimise (max - min) of the elevations on a 4-connected path from the
// top-left to the bottom-right cell of an n x n grid, n <= 100, 0 <= h <= 200.
//
// The objective depends on the path only through the closed window
// [lo, hi] that contains its elevations, so the answer is
//     min { hi - lo : (0,0) and (n-1,n-1) are connected using only cells
//                     with lo <= h <= hi }.
// Two monotonicities make this cheap.  For a fixed lo, connectivity is
// monotone in hi (raising hi only adds cells).  And F(lo) = the least
// feasible hi is non-decreasing in lo, because S(lo',hi) is a subset of
// S(lo,hi) whenever lo' > lo.  So a two-pointer sweep of lo = 0..min(h_start,
// h_end) with a never-rewound hi does at most ~2*201 flood fills of 10^4
// cells each, rather than one per (lo,hi) pair.  If some lo admits no hi
// <= 200 then no larger lo does either, so the sweep stops there.
//
// lo is capped at min(h_start, h_end) and hi starts at max(h_start, h_end),
// which makes the endpoint range checks inside the flood fill redundant;
// they are kept because they cost nothing and state the invariant.
//
// No statement ambiguity.  Output ends each scenario with a blank line as
// the statement asks.
#include <cstdio>

static int n;
static int h[105][105];
static int vis[105][105];
static int stamp;
static int stkx[10005], stky[10005];

static const int DX[4] = {-1, 1, 0, 0};
static const int DY[4] = {0, 0, -1, 1};

/* Is (0,0) joined to (n-1,n-1) through cells with lo <= h <= hi ? */
static bool connected(int lo, int hi)
{
    int top, x, y, d, nx, ny;
    if (h[0][0] < lo || h[0][0] > hi) return false;
    if (h[n - 1][n - 1] < lo || h[n - 1][n - 1] > hi) return false;
    ++stamp;
    top = 0;
    stkx[top] = 0; stky[top] = 0; ++top;
    vis[0][0] = stamp;
    while (top > 0) {
        --top;
        x = stkx[top]; y = stky[top];
        if (x == n - 1 && y == n - 1) return true;
        for (d = 0; d < 4; ++d) {
            nx = x + DX[d]; ny = y + DY[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
            if (vis[nx][ny] == stamp) continue;
            if (h[nx][ny] < lo || h[nx][ny] > hi) continue;
            vis[nx][ny] = stamp;
            stkx[top] = nx; stky[top] = ny; ++top;
        }
    }
    return false;
}

static char buf[1 << 16];
static int bufLen, bufPos;

static int gc()
{
    if (bufPos == bufLen) {
        bufLen = (int)fread(buf, 1, sizeof(buf), stdin);
        bufPos = 0;
        if (bufLen <= 0) return -1;
    }
    return (unsigned char)buf[bufPos++];
}

/* Reads one non-negative integer; returns 0 at end of input. */
static int readInt(int *out)
{
    int c, v;
    do { c = gc(); } while (c == ' ' || c == '\n' || c == '\r' || c == '\t');
    if (c < '0' || c > '9') return 0;
    v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    *out = v;
    return 1;
}

int main()
{
    int t, cse, i, j, lo, hi, hs, he, lmax, best;
    if (!readInt(&t)) return 0;
    for (cse = 1; cse <= t; ++cse) {
        if (!readInt(&n)) break;
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                if (!readInt(&h[i][j])) h[i][j] = 0;
        hs = h[0][0];
        he = h[n - 1][n - 1];
        lmax = hs < he ? hs : he;
        hi = hs > he ? hs : he;
        best = 201;
        for (lo = 0; lo <= lmax; ++lo) {
            while (hi <= 200 && !connected(lo, hi)) ++hi;
            if (hi > 200) break;
            if (hi - lo < best) best = hi - lo;
        }
        printf("Scenario #%d:\n%d\n\n", cse, best);
    }
    return 0;
}
