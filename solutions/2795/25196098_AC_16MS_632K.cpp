// POJ 2795 - Exploring Pyramids
// Model: claude-fable-5
// Interval DP over the Euler tour: f(i,j) = number of ordered rooted trees
// whose DFS tour (print color on every visit of a node) equals s[i..j].
// f(i,i) = 1; f(i,j) = 0 if s[i] != s[j]; otherwise
//   f(i,j) = sum over k in {i+2..j, s[k]==s[i]} of f(i+1,k-1) * f(k,j),
// where s[i+1..k-1] is the first child's tour and s[k..j] the root with the
// remaining children. Answer f(0,n-1) mod 1e9 (even-length tapes give 0
// automatically since every subproblem keeps odd length). Products of two
// residues < 1e9 need 64-bit intermediates, hence long long / language G++.
// Statement gives no case count, so tapes are read to EOF.
#include <cstdio>
#include <cstring>

static int f[301][301];
static char s[310];
static const long long MOD = 1000000000LL;

int main() {
    while (scanf("%300s", s) == 1) {
        int n = (int)strlen(s);
        for (int i = 0; i < n; i++) f[i][i] = 1;
        for (int len = 3; len <= n; len += 2) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                long long acc = 0;
                if (s[i] == s[j]) {
                    for (int k = i + 2; k <= j; k += 2) {
                        if (s[k] == s[i] && f[i + 1][k - 1]) {
                            acc += (long long)f[i + 1][k - 1] * f[k][j] % MOD;
                            if (acc >= MOD) acc -= MOD;
                        }
                    }
                }
                f[i][j] = (int)acc;
            }
        }
        int ans;
        if (n % 2 == 0) ans = 0;
        else ans = f[0][n - 1];
        printf("%d\n", ans);
    }
    return 0;
}
