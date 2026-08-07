// POJ 2738 - Two Ends
// Model: claude-opus-5
//
// Interval DP on the score difference.  dp[i][j] = best achievable
// (player 1's total - player 2's total) over the remaining row a[i..j], with
// player 1 to move; j-i+1 is always even at such a state, so only even-length
// intervals are ever needed.
//
// Player 1 is free, player 2 is greedy: she removes the larger end, and on a
// tie the left one ("always take the larger end.  If there is a tie, remove
// the left end." -- so the comparison is a[left] >= a[right]).  Because the
// opponent's reply is forced, each of player 1's two choices leads to exactly
// one successor state:
//
//   take a[i]:  greedy then faces a[i+1..j]
//                 a[i+1] >= a[j] -> a[i] - a[i+1] + dp[i+2][j]
//                 else           -> a[i] - a[j]   + dp[i+1][j-1]
//   take a[j]:  greedy then faces a[i..j-1]
//                 a[i] >= a[j-1] -> a[j] - a[i]   + dp[i+1][j-1]
//                 else           -> a[j] - a[j-1] + dp[i][j-2]
//
// dp of an empty interval is 0, which makes the length-2 case fall out as
// |a[i]-a[j]| automatically.  O(n^2) time, O(n^2) memory (n <= 1000, so the
// table is ~4 MB against a 65536K limit).  The answer for a game is dp[0][n-1];
// the sum bound of 1,000,000 keeps everything inside int.
//
// Input holds multiple games, one per line, terminated by a line "0".

#include <cstdio>

static const int MAXN = 1000;
static int a[MAXN];
static int dp[MAXN][MAXN];

int main() {
    int n;
    int game = 1;
    while (scanf("%d", &n) == 1 && n != 0) {
        int i;
        for (i = 0; i < n; ++i) scanf("%d", &a[i]);

        int len;
        for (len = 2; len <= n; len += 2) {
            for (i = 0; i + len - 1 < n; ++i) {
                int j = i + len - 1;
                int va, vb;

                /* player 1 takes the left card a[i] */
                if (a[i + 1] >= a[j])
                    va = a[i] - a[i + 1] + (i + 2 <= j ? dp[i + 2][j] : 0);
                else
                    va = a[i] - a[j] + (i + 1 <= j - 1 ? dp[i + 1][j - 1] : 0);

                /* player 1 takes the right card a[j] */
                if (a[i] >= a[j - 1])
                    vb = a[j] - a[i] + (i + 1 <= j - 1 ? dp[i + 1][j - 1] : 0);
                else
                    vb = a[j] - a[j - 1] + (i <= j - 2 ? dp[i][j - 2] : 0);

                dp[i][j] = va > vb ? va : vb;
            }
        }

        printf("In game %d, the greedy strategy might lose by as many as %d points.\n",
               game++, dp[0][n - 1]);
    }
    return 0;
}
