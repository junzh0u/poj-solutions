// POJ 3275 - Ranking the Cows
// Model: claude-opus-5
//
// The C queries must be chosen up front, and must pin down the order no matter
// how they come back.  Two total orders consistent with the known comparisons
// are indistinguishable unless some queried pair separates them, and for every
// pair {u,v} incomparable under the known relation there are two consistent
// orders differing only in the u/v swap.  So the query set must contain exactly
// the incomparable pairs, and asking all of them is enough:
//     C = n*(n-1)/2 - (# pairs comparable in the transitive closure).
// (Discuss-board reading: the answer is worst-case/non-adaptive, not "for one
// lucky sequence of answers" -- e.g. "4 2 / 2 1 / 3 2" is 3, not 1.)
//
// Transitive closure by Warshall over 32-bit rows: n=1000 makes plain O(n^3)
// too slow for the 2s limit (the board is full of TLEs from it), while the
// bitwise version is n^2 * n/32 ~ 16M word ops and runs in milliseconds.
// Input is read to EOF in case the judge data holds several data sets.

#include <cstdio>
#include <cstring>

static const int MAXN = 1000;
static const int W = (MAXN + 31) / 32;

static unsigned int reach[MAXN][W];

int main() {
    int n, m;
    while (scanf("%d %d", &n, &m) == 2) {
        int words = (n + 31) / 32;
        for (int i = 0; i < n; ++i)
            memset(reach[i], 0, sizeof(unsigned int) * words);
        for (int e = 0; e < m; ++e) {
            int x, y;
            if (scanf("%d %d", &x, &y) != 2) return 0;
            --x; --y;
            reach[x][y >> 5] |= (1u << (y & 31));
        }
        for (int k = 0; k < n; ++k) {
            unsigned int mask = (1u << (k & 31));
            int kw = k >> 5;
            const unsigned int *rk = reach[k];
            for (int i = 0; i < n; ++i) {
                if (reach[i][kw] & mask) {
                    unsigned int *ri = reach[i];
                    for (int w = 0; w < words; ++w) ri[w] |= rk[w];
                }
            }
        }
        int comparable = 0;
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if ((reach[i][j >> 5] & (1u << (j & 31))) ||
                    (reach[j][i >> 5] & (1u << (i & 31))))
                    ++comparable;
        int total = n * (n - 1) / 2;
        printf("%d\n", total - comparable);
    }
    return 0;
}
