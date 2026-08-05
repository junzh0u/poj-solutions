// POJ 1651 - Multiplication Puzzle
//
// A row of n cards (3 <= n <= 100, values 1..100). Removing a card scores
// left*card*right against its CURRENT neighbours; the first and last card can
// never be removed, so exactly n-2 removals happen and the two ends survive.
// Minimise the total score.
//
// Interval DP over the cards that remain. dp[i][j] = minimum points to clear
// every card strictly between i and j while i and j are still in place;
// dp[i][i+1] = 0. Split on which card k in (i,j) is removed LAST: at that
// moment everything else inside is already gone, so k's neighbours are exactly
// i and j and it scores a[i]*a[k]*a[j], independent of the order used inside
// the two halves. Hence
//     dp[i][j] = min over i<k<j of dp[i][k] + dp[k][j] + a[i]*a[k]*a[j]
// and the answer is dp[0][n-1]. O(n^3) = 1e6 steps for n = 100.
//
// The usual trap is splitting on the FIRST removal, or letting the endpoints
// move: the endpoints of every subinterval are cards that are still present,
// which is what makes the two halves independent. Statement note: the worked
// example in the problem uses 5 cards while the sample input has 6 - they are
// unrelated cases.
//
// Every card but the two ends is removed once, each removal scoring at most
// 100*100*100 = 1e6, so the total is at most 98e6 and fits a 32-bit int; no
// long long, and no %lld that POJ's C++ compiler might not honour.

#include <cstdio>

static const int INF = 0x3f3f3f3f;

int a[105];
int dp[105][105];

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        int i, j, k, len;
        for (i = 0; i < n; ++i) scanf("%d", &a[i]);
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j) dp[i][j] = 0;
        for (len = 2; len < n; ++len) {
            for (i = 0; i + len < n; ++i) {
                j = i + len;
                int best = INF;
                for (k = i + 1; k < j; ++k) {
                    int cur = dp[i][k] + dp[k][j] + a[i] * a[k] * a[j];
                    if (cur < best) best = cur;
                }
                dp[i][j] = best;
            }
        }
        printf("%d\n", dp[0][n - 1]);
    }
    return 0;
}
