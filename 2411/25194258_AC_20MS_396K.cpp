// POJ 2411 - Mondriaan's Dream
//
// Count the domino (2x1) tilings of an h x w board, 1 <= h,w <= 11, for a
// sequence of cases ending with "0 0". Tilings are counted as oriented, so
// symmetric ones are distinct.
//
// Broken-profile bitmask DP over rows. dp[i][mask] is the number of ways to
// tile rows 0..i-1 completely such that `mask` marks the cells of row i that
// are already occupied by vertical dominoes protruding down from row i-1.
// The row transition is a small DFS across the w columns: a column already
// covered by the incoming mask is skipped; an empty column either starts a
// vertical domino (setting its bit in the outgoing mask) or pairs with the
// next column as a horizontal domino. The answer is dp[h][0] -- the last row
// must leave nothing protruding past the bottom edge.
//
// The number of cases is unbounded, so all 11x11 answers are precomputed once
// (a few milliseconds) and each query is a table lookup.
//
// The maximum answer, 11x10 = 3852472573499, overflows 32 bits, hence the
// long long accumulator and %lld. Boards of odd area (e.g. 11x11, or any
// dimension 1 against an odd one) correctly come out 0.
#include <cstdio>
#include <cstring>

static int W;
static long long *ndp;

/* Fill row `col..W-1`, given `cur` = cells of this row already taken by
   vertical dominoes protruding from the row above, accumulating into `nxt`
   = cells of the next row taken by verticals started here. */
static void dfs(int col, int cur, int nxt, long long val)
{
    if (col >= W) { ndp[nxt] += val; return; }
    if (cur & (1 << col)) { dfs(col + 1, cur, nxt, val); return; }
    /* vertical domino: occupies (r,col) and (r+1,col) */
    dfs(col + 1, cur, nxt | (1 << col), val);
    /* horizontal domino: occupies (r,col) and (r,col+1) */
    if (col + 1 < W && !(cur & (1 << (col + 1))))
        dfs(col + 2, cur, nxt, val);
}

static long long ans[12][12];

static long long solve(int h, int w)
{
    static long long dp[12][2048];
    int full, i, m;
    W = w;
    full = 1 << w;
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;
    for (i = 0; i < h; ++i) {
        ndp = dp[i + 1];
        for (m = 0; m < full; ++m)
            if (dp[i][m]) dfs(0, m, 0, dp[i][m]);
    }
    return dp[h][0];
}

int main()
{
    int h, w;
    for (h = 1; h <= 11; ++h)
        for (w = 1; w <= 11; ++w)
            ans[h][w] = solve(h, w);
    while (scanf("%d %d", &h, &w) == 2 && (h || w))
        printf("%lld\n", ans[h][w]);
    return 0;
}
