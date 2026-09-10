// POJ 3920 - RIPOFF
// Model: claude-sonnet-5
//
// Approach: dp[k][i] = max rebate achievable while standing on board
// square i (1<=i<=N) having used exactly k turns so far (dp[0][0]=0 is
// the starting position "just before the board", 0 turns used).
// dp[k][i] = value[i] + max over d=1..S of dp[k-1][i-d] (i-d>=0).
// The answer is the best dp[k][i] (0<=k<=T-1, 0<=i<=N) from which one
// more turn of at most S spaces reaches past the end, i.e. reaches at
// least N+1: (N+1)-i<=S; that final turn brings the running total to
// k+1<=T turns used and adds no further value since the game is over
// once off the board.
// Unreachable states are -infinity.
//
// Ambiguity check: the sample confirms only squares actually landed on
// (not skipped-over squares) contribute their value, and the final
// move off the board need not be exact -- both match this DP directly.
#include <cstdio>
#include <algorithm>
using namespace std;

const int NEG = -1000000000;

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int s, t;
        scanf("%d %d", &s, &t);
        static int value[205];
        for (int i = 1; i <= n; ++i) scanf("%d", &value[i]);

        // dp[k][i]
        static int dp[205][205];
        for (int k = 0; k <= t; ++k)
            for (int i = 0; i <= n; ++i)
                dp[k][i] = NEG;
        dp[0][0] = 0;

        for (int k = 1; k <= t; ++k) {
            for (int i = 1; i <= n; ++i) {
                int lo = max(0, i - s);
                int m = NEG;
                for (int prev = lo; prev <= i - 1; ++prev) {
                    if (dp[k - 1][prev] > m) m = dp[k - 1][prev];
                }
                if (m > NEG) dp[k][i] = m + value[i];
            }
        }

        // For every k from 0..t-1 (on-board states reached after k turns), if the
        // remaining distance to N+1 is (n+1-i) <= s, one more turn (k+1<=t)
        // finishes the game.
        int best = NEG;
        for (int k = 0; k <= t - 1; ++k) {
            for (int i = 0; i <= n; ++i) {
                if (dp[k][i] <= NEG) continue;
                if (n + 1 - i <= s) {
                    if (dp[k][i] > best) best = dp[k][i];
                }
            }
        }

        printf("%d\n", best);
    }
    return 0;
}
