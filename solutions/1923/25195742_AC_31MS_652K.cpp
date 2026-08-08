// POJ 1923 - Fourier's Lines
// Model: claude-opus-5
//
// Distinct lines in the plane either meet in one point or are parallel, so N
// lines split into direction classes of sizes a_1..a_k.  "Each crossing belongs
// to exactly two lines" forbids three concurrent lines, so every cross-class
// pair contributes its own distinct crossing:
//     M = C(N,2) - sum_i C(a_i,2).
// Feasibility is therefore: can N be partitioned into parts whose C(a_i,2) sum
// to S = C(N,2) - M?  Precompute dp[n][s] once (unbounded knapsack over the
// items (a, C(a,2)) for a = 1..100), then answer every case in O(1).
//
// Piece count: adding a line that meets the earlier ones in p distinct points
// splits p+1 existing regions, so regions = 1 + N + sum p_i = 1 + N + M for
// every valid arrangement.  The "at most" is vacuous -- once N and M are fixed
// and no three lines are concurrent, the count is forced.
//
// The trap: the discuss board asserts every M in [N-1, C(N,2)] is drawable.
// That claim reproduces the official sample exactly (all five cases), yet is
// already false at N=5, M=5.  Verified against exhaustive partition enumeration
// for all N <= 32 and every M, and the region formula was checked against a
// geometric sign-vector count of real arrangements for all partitions of N <= 6.

#include <cstdio>
#include <cstring>

const int MAXN = 100;
const int MAXS = MAXN * (MAXN - 1) / 2; /* 4950 */

static bool dp[MAXN + 1][MAXS + 1];

int main() {
    int a, n, s;

    memset(dp, 0, sizeof(dp));
    dp[0][0] = true;
    for (n = 1; n <= MAXN; ++n) {
        for (a = 1; a <= n; ++a) {
            int c = a * (a - 1) / 2;
            for (s = c; s <= MAXS; ++s) {
                if (dp[n - a][s - c]) dp[n][s] = true;
            }
        }
    }

    int m, cs = 0;
    while (scanf("%d %d", &n, &m) == 2 && !(n == 0 && m == 0)) {
        ++cs;
        bool ok = false;
        if (n >= 0 && n <= MAXN && m >= 0) {
            int total = n * (n - 1) / 2;
            if (m <= total && dp[n][total - m]) ok = true;
        }
        if (ok)
            printf("Case %d: %d lines with exactly %d crossings can cut the plane into %d pieces at most.\n",
                   cs, n, m, 1 + n + m);
        else
            printf("Case %d: %d lines cannot make exactly %d crossings.\n", cs, n, m);
    }
    return 0;
}
