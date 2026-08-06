// POJ 3254 - Corn Fields
//
// Count the ways to choose a set of squares on an M x N board (M, N <= 12) so
// that no chosen square is infertile and no two chosen squares share an edge.
// The empty choice counts, and the answer is taken modulo 100,000,000 (1e8,
// not a prime and not 1e9+7 - the statement writes it as "100,000,000").
//
// Profile-based (broken-profile-free) bitmask DP over rows. A row's plan is a
// bitmask of the columns planted in it; only the masks with no two adjacent
// bits set are legal, and for N = 12 there are just 377 of them (Fibonacci),
// so they are enumerated once into ok[]. A mask is placeable on row i iff
// (mask & ~fertile[i]) == 0, i.e. it plants nothing on an infertile square.
// dp[i][mask] = number of ways to fill rows 0..i with row i planted as mask;
// the transition sums dp[i-1][prev] over every legal prev with prev & mask == 0
// (that is the vertical-adjacency constraint). The answer is the sum over the
// last row's states, which already includes the all-empty arrangement because
// mask 0 is a legal state of every row.
//
// Cost is O(M * 377^2) ~ 1.7M additions in the worst case; nothing here needs
// more than 32-bit ints since every value is kept reduced mod 1e8.

#include <cstdio>
#include <cstring>

static const int MOD = 100000000;

int main() {
    int m, n;
    if (scanf("%d %d", &m, &n) != 2) return 0;
    int full = 1 << n;
    int fertile[13];
    for (int i = 0; i < m; ++i) {
        int mask = 0;
        for (int j = 0; j < n; ++j) {
            int v;
            if (scanf("%d", &v) != 1) v = 0;
            if (v) mask |= 1 << j;
        }
        fertile[i] = mask;
    }

    /* enumerate row patterns with no two horizontally adjacent cells */
    int ok[4096];
    int cnt = 0;
    for (int s = 0; s < full; ++s)
        if (!(s & (s << 1))) ok[cnt++] = s;

    static int cur[4096], nxt[4096];
    for (int k = 0; k < cnt; ++k) cur[k] = 0;
    for (int k = 0; k < cnt; ++k)
        if ((ok[k] & ~fertile[0]) == 0) cur[k] = 1;

    for (int i = 1; i < m; ++i) {
        for (int k = 0; k < cnt; ++k) nxt[k] = 0;
        for (int k = 0; k < cnt; ++k) {
            int s = ok[k];
            if (s & ~fertile[i]) continue;
            int sum = 0;
            for (int p = 0; p < cnt; ++p) {
                if (cur[p] == 0) continue;
                if (ok[p] & s) continue;
                sum += cur[p];
                if (sum >= MOD) sum -= MOD;
            }
            nxt[k] = sum;
        }
        for (int k = 0; k < cnt; ++k) cur[k] = nxt[k];
    }

    int ans = 0;
    for (int k = 0; k < cnt; ++k) {
        ans += cur[k];
        if (ans >= MOD) ans -= MOD;
    }
    printf("%d\n", ans);
    return 0;
}
