// POJ 1197 - Depot
// Model: gpt-6.1-sol
// Row insertion is the Robinson-Schensted correspondence. For a fixed
// insertion tableau, arrival permutations correspond bijectively to standard
// recording tableaux of its shape. Count their left/up-respecting cell orders.
// Identification values do not affect the count for a valid placement.
#include <cstdio>
int dp[1 << 13];
int main() {
    int rows, n = 0, cell[13][13], need[13];
    if (scanf("%d", &rows) != 1) return 0;
    for (int r = 0; r < rows; ++r) {
        int m; scanf("%d", &m);
        for (int c = 0; c < m; ++c) {
            int value; scanf("%d", &value);
            cell[r][c] = n;
            need[n] = 0;
            if (r) need[n] |= 1 << cell[r-1][c];
            if (c) need[n] |= 1 << cell[r][c-1];
            ++n;
        }
    }
    dp[0] = 1;
    for (int mask = 0; mask < (1 << n); ++mask)
        if (dp[mask])
            for (int i = 0; i < n; ++i)
                if (!(mask & (1 << i)) && (mask & need[i]) == need[i])
                    dp[mask | (1 << i)] += dp[mask];
    printf("%d\n", dp[(1 << n)-1]);
    return 0;
}
