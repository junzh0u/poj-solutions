// POJ 3934 - Queue
// Model: claude-sonnet-5
//
// Kids stand in a line; two kids see each other iff everyone strictly
// between them is shorter than both. Build the line by inserting kids
// from tallest to shortest into the current arrangement of already-
// placed (taller) kids. Inserting the i-th kid (1-indexed by insertion
// order, i from 1..n) into an arrangement of i-1 kids has i possible
// slots: the 2 end slots each create exactly 1 new visible pair (with
// the current end neighbor), and the i-2 interior slots each create
// exactly 2 new visible pairs (with both neighbors of that gap) while
// leaving the pair that used to be adjacent there still visible
// forever after (already counted when that pair first became
// adjacent). So:
//   f[1][0] = 1
//   f[i][j] = 2*f[i-1][j-1] + (i-2)*f[i-1][j-2]   (mod 9937)
// Answer to a query (n,m) is f[n][m], or 0 if m exceeds the maximum
// possible number of visible pairs for n kids (2n-3 for n>=2, 0 for
// n==1). Verified against an accepted Pascal solution posted on the
// problem's discuss board, which uses the identical recurrence.

#include <cstdio>
#include <cstring>

const int MOD = 9937;
const int MAXN = 80;
const int MAXM = 160; // safely above the true max of 2*80-3=157

static int f[MAXN + 1][MAXM + 1];

int main() {
    memset(f, 0, sizeof(f));
    f[1][0] = 1;
    for (int i = 2; i <= MAXN; ++i) {
        for (int j = 0; j <= MAXM; ++j) {
            int v = 0;
            if (j - 1 >= 0) v += 2 * f[i - 1][j - 1];
            if (j - 2 >= 0 && i - 2 >= 0) v += (i - 2) * f[i - 1][j - 2];
            f[i][j] = v % MOD;
        }
    }

    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        if (n == 0 && m == 0) break;
        int ans = 0;
        if (m >= 0 && m <= MAXM) ans = f[n][m];
        printf("%d\n", ans);
    }
    return 0;
}
