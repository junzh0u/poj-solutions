// POJ 2626 - Chess
// Model: claude-opus-5
//
// Pick 15 players to play white and 15 other players to play black out of
// n (30..1000) candidates, maximizing the sum of the white abilities of the
// whites plus the black abilities of the blacks.  Nobody may play both.
//
// 0/1 knapsack in two dimensions: dp[i][j] = best value using the players
// read so far with i of them on white and j on black.  Each new player is
// folded in by scanning i and j downward, so dp[i-1][j] and dp[i][j-1] are
// still last round's values and the player is used at most once (scanning
// upward instead would let one player fill both a white and a black board
// -- it gives 2895 on the sample instead of 2506).  Unreachable states are
// held at NEG so a partial team can never be mistaken for a complete one.
// Answer is dp[15][15]; at most 30*100 = 3000, so int is plenty.
//
// The statement is unambiguous; the input is one data set read to EOF.
#include <cstdio>

static const int K = 15;          /* 15 boards of each colour */
static const int NEG = -1000000;  /* unreachable state */

int main() {
    int dp[K + 1][K + 1];
    int w, b, i, j;

    for (i = 0; i <= K; i++)
        for (j = 0; j <= K; j++)
            dp[i][j] = NEG;
    dp[0][0] = 0;

    while (scanf("%d %d", &w, &b) == 2) {
        for (i = K; i >= 0; i--) {
            for (j = K; j >= 0; j--) {
                int best = dp[i][j];
                if (i > 0 && dp[i - 1][j] != NEG && dp[i - 1][j] + w > best)
                    best = dp[i - 1][j] + w;
                if (j > 0 && dp[i][j - 1] != NEG && dp[i][j - 1] + b > best)
                    best = dp[i][j - 1] + b;
                dp[i][j] = best;
            }
        }
    }

    printf("%d\n", dp[K][K]);
    return 0;
}
