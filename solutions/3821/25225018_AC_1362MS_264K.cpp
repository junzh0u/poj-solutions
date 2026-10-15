// POJ 3821 - Clickomania
// Model: gpt-6.1-sol
// Interval grammar DP: concatenate solvable strings, or surround one/two
// solvable interiors with two/three copies of the same letter.
// Empty intervals are solvable. Time O(n^3), space O(n^2).
#include <cstdio>
#include <cstring>
char s[155];
bool dp[155][155];
int main() {
    while (scanf("%150s", s) == 1) {
        int n = (int)strlen(s);
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i <= n; ++i) dp[i][i] = true;
        for (int len = 2; len <= n; ++len) {
            for (int l = 0; l + len <= n; ++l) {
                int r = l + len;
                if (s[l] == s[r-1]) {
                    dp[l][r] = dp[l+1][r-1];
                    for (int k = l+1; k < r-1 && !dp[l][r]; ++k)
                        if (s[k] == s[l] && dp[l+1][k] && dp[k+1][r-1])
                            dp[l][r] = true;
                }
                for (int k = l+2; k <= r-2 && !dp[l][r]; ++k)
                    if (dp[l][k] && dp[k][r]) dp[l][r] = true;
            }
        }
        puts(dp[0][n] ? "solvable" : "unsolvable");
    }
    return 0;
}
