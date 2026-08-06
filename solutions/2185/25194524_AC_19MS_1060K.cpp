// POJ 2185 - Milking Grid
//
// Find the smallest-area rectangle that tiles an R x C grid (R <= 10000,
// C <= 75), where the tiling may be truncated at the right and bottom edges:
// the unit's dimensions need not divide R and C.
//
// The grid is tiled by an h x w unit exactly when g[i][j] == g[i%h][j%w] for
// every cell, which is the same as saying w is a *period* of every row and h
// is a period of every column.  The two conditions are independent, so the
// smallest area is (smallest valid w) * (smallest valid h) and each dimension
// can be minimised on its own.
//
// Each dimension is one KMP failure function over compound symbols:
//   - horizontal: treat the C columns as a string of C symbols (two symbols
//     are equal when the columns match in all R rows).  The minimal period of
//     a string of length n is n - fail[n], so w = C - fail[C].
//   - vertical: treat the R rows as a string of R symbols, h = R - fail[R].
// Answer = w * h.  Symbol comparisons cost O(R) resp. O(C), and KMP does O(n)
// of them amortised, so the whole thing is O(R*C).
//
// Two things the statement leaves easy to get wrong:
//   - because the tiling may be cut off, the period need NOT divide the
//     dimension -- no divisibility test (unlike POJ 1961).
//   - a grid with no repetition at all has period equal to the full
//     dimension; n - fail[n] already yields that, so no clamping is needed.
#include <cstdio>
#include <cstring>

static const int MAXR = 10005;
static const int MAXC = 80;

static char g[MAXR][MAXC];
static int fr[MAXR + 1];
static int fc[MAXC + 1];
static int R, C;

static bool sameRow(int a, int b) {
    return memcmp(g[a], g[b], (size_t)C) == 0;
}

static bool sameCol(int a, int b) {
    for (int i = 0; i < R; ++i)
        if (g[i][a] != g[i][b]) return false;
    return true;
}

int main() {
    if (scanf("%d %d", &R, &C) != 2) return 0;
    for (int i = 0; i < R; ++i) scanf("%s", g[i]);

    /* minimal period of the sequence of columns = minimal horizontal period */
    fc[0] = 0;
    fc[1] = 0;
    for (int i = 1; i < C; ++i) {
        int k = fc[i];
        while (k > 0 && !sameCol(i, k)) k = fc[k];
        if (sameCol(i, k)) ++k;
        fc[i + 1] = k;
    }
    int w = C - fc[C];

    /* minimal period of the sequence of rows = minimal vertical period */
    fr[0] = 0;
    fr[1] = 0;
    for (int i = 1; i < R; ++i) {
        int k = fr[i];
        while (k > 0 && !sameRow(i, k)) k = fr[k];
        if (sameRow(i, k)) ++k;
        fr[i + 1] = k;
    }
    int h = R - fr[R];

    printf("%d\n", w * h);
    return 0;
}
