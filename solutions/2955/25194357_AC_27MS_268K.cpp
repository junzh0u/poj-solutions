// POJ 2955 - Brackets
//
// Longest regular-brackets subsequence via interval DP.
// dp[i][j] = length of the longest regular subsequence within s[i..j].
// For each interval, the answer is the max of two independent options:
//   1) pair the two ends if they match ('(' with ')', '[' with ']'):
//      dp[i+1][j-1] + 2
//   2) split the interval at some k and combine two independent regular
//      subsequences: max over k of dp[i][k] + dp[k+1][j]
// Both must be considered and maxed, not just the pairing option: the DP
// is superadditive under splitting (concatenating two regular sequences
// is regular), so dp[i][k] + dp[k+1][j] can exceed dp[i+1][j-1] + 2 even
// when s[i] and s[j] match -- e.g. "()()" pairs poorly end-to-end but
// splits perfectly. Filled by increasing interval length; O(n^3) per
// line, trivially fast for n <= 100.
//
// Input is terminated by a line "end", not processed.
#include <cstdio>
#include <cstring>
using namespace std;

int dp[105][105];
char s[105];

bool match(char a, char b) {
    return (a == '(' && b == ')') || (a == '[' && b == ']');
}

int main() {
    while (scanf("%s", s) == 1) {
        if (strcmp(s, "end") == 0) break;
        int n = strlen(s);
        memset(dp, 0, sizeof(dp));
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i + len - 1 < n; ++i) {
                int j = i + len - 1;
                int best = 0;
                if (match(s[i], s[j])) {
                    best = dp[i+1][j-1] + 2;
                }
                for (int k = i; k < j; ++k) {
                    int cand = dp[i][k] + dp[k+1][j];
                    if (cand > best) best = cand;
                }
                dp[i][j] = best;
            }
        }
        printf("%d\n", n > 0 ? dp[0][n-1] : 0);
    }
    return 0;
}
