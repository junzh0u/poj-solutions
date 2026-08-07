// POJ 2264 - Advanced Fruits
// Model: claude-opus-5
//
// Shortest common supersequence of two names, reconstructed.
//
// Statement reading: it says "contains both names ... as sub-strings", but its
// own example settles the ambiguity the other way -- "applear" is said to
// contain "pear" as "apPlEAR", i.e. as a *subsequence*, not a contiguous
// substring.  (A contiguous reading would make "appleach" impossible for
// apple/peach.)  So: shortest string having both names as subsequences.
//
// Algorithm: |SCS(a,b)| = |a| + |b| - |LCS(a,b)|, so fill the standard O(nm)
// LCS table, then walk back from (n,m): on a matching pair emit the shared
// character and step diagonally, otherwise step whichever way the LCS table
// says loses nothing and emit the character stepped over.  Whatever prefix of
// one name is left over when the other runs out is emitted as-is.  Output is
// built in reverse and printed backwards.  n,m <= 100, so this is trivial in
// time and memory; input is read to EOF (multiple data sets).
//
// Special judge: any shortest answer is accepted, so the answer need not match
// the sample's choice character for character (this program prints "peachr"
// where the sample shows "pearch"; both are length 6 and both valid).
//
// Verified locally against an independently derived reference: a BFS over
// states (i,j) = "characters of each name matched so far", built straight from
// the statement's words with no LCS in its derivation.  All 15876 pairs of
// binary strings up to length 6 and all 14400 pairs of ternary strings up to
// length 4 agree, plus ~9200 random pairs over several alphabets; every answer
// was checked the way the special judge would (both names are subsequences)
// and its length checked against the BFS optimum.

#include <cstdio>
#include <cstring>

static char a[105], b[105];
static short dp[105][105];
static char out[210];

int main() {
    while (scanf("%s %s", a, b) == 2) {
        int n = (int)strlen(a), m = (int)strlen(b);
        int i, j, k, t;
        for (i = 0; i <= n; ++i) dp[i][0] = 0;
        for (j = 0; j <= m; ++j) dp[0][j] = 0;
        for (i = 1; i <= n; ++i)
            for (j = 1; j <= m; ++j) {
                if (a[i - 1] == b[j - 1]) dp[i][j] = (short)(dp[i - 1][j - 1] + 1);
                else dp[i][j] = dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
            }
        i = n; j = m; k = 0;
        while (i > 0 && j > 0) {
            if (a[i - 1] == b[j - 1]) { out[k++] = a[i - 1]; --i; --j; }
            else if (dp[i - 1][j] >= dp[i][j - 1]) { out[k++] = a[i - 1]; --i; }
            else { out[k++] = b[j - 1]; --j; }
        }
        while (i > 0) out[k++] = a[--i];
        while (j > 0) out[k++] = b[--j];
        for (t = k - 1; t >= 0; --t) putchar(out[t]);
        putchar('\n');
    }
    return 0;
}
