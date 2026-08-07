// POJ 2817 - WordStack
// Model: claude-opus-5
//
// Each of the N (<= 10) words goes on its own line, padded with leading spaces
// only, and the score counts positions where a non-space character equals the
// character directly above it on the preceding line.  The score of an adjacent
// pair of lines therefore depends only on the *difference* of their leading
// paddings, and since paddings are unbounded above, any vector of differences
// is realizable (fix the first line at 0, accumulate, then shift so the
// smallest padding is 0).  So the total decomposes into independent per-pair
// maxima and the problem is a maximum-weight Hamiltonian path on N <= 10 nodes.
//
//   best[i][j] = max over relative shifts d in [-10,10] of the number of
//                aligned equal characters between word i and word j
//                (symmetric, since d and -d swap the roles)
//   dp[mask][last] = best score using exactly the words in mask, word `last`
//                    on the bottom line;  answer = max dp[full][*]
//
// O(2^N * N^2) per test set, ~0.2 ms at N = 10.
//
// Reading note: this is NOT the longest common subsequence of the two words --
// the discuss board records that misreading, and the official sample separates
// them (rigid alignment gives 8, LCS gives 9).  Board cases confirming the
// rigid reading: {adc, abc} -> 2 and {abcdefg, aaafcee} -> 3.
//
// Input holds several test sets and ends at a non-positive N (the samples use
// 0, but negatives are legal too), or at EOF.

#include <cstdio>
#include <cstring>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n > 0) {
        char w[10][16];
        int len[10];
        for (int i = 0; i < n; i++) {
            scanf("%s", w[i]);
            len[i] = (int)strlen(w[i]);
        }

        int best[10][10];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int b = 0;
                for (int d = -10; d <= 10; d++) {
                    int c = 0;
                    for (int p = 0; p < len[i]; p++) {
                        int q = p - d;
                        if (q >= 0 && q < len[j] && w[i][p] == w[j][q]) c++;
                    }
                    if (c > b) b = c;
                }
                best[i][j] = b;
            }
        }

        int full = 1 << n;
        static int dp[1 << 10][10];
        memset(dp, 0xff, sizeof(int) * (unsigned)full * 10);  /* all -1 */
        for (int i = 0; i < n; i++) dp[1 << i][i] = 0;

        for (int m = 1; m < full; m++) {
            for (int last = 0; last < n; last++) {
                if (dp[m][last] < 0) continue;
                for (int nx = 0; nx < n; nx++) {
                    if (m & (1 << nx)) continue;
                    int nm = m | (1 << nx);
                    int v = dp[m][last] + best[last][nx];
                    if (v > dp[nm][nx]) dp[nm][nx] = v;
                }
            }
        }

        int ans = 0;
        for (int i = 0; i < n; i++)
            if (dp[full - 1][i] > ans) ans = dp[full - 1][i];
        printf("%d\n", ans);
    }
    return 0;
}
