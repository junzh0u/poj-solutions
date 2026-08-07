// POJ 1934 - Trip
// Model: claude-opus-5
//
// Print every distinct longest common subsequence of two strings (1..80 lower
// case letters), one per line, in ascending order.  The statement guarantees at
// least one non-empty answer and at most 1000 distinct ones.
//
// Approach: suffix LCS table plus a next-occurrence table, then a DFS that
// emits the answers directly in sorted order with no post-hoc sort or dedupe.
//
//   lcs[i][j] = LCS length of A[i..] and B[j..]
//   na[i][c]  = smallest k >= i with A[k] == 'a'+c  (n if none); nb likewise
//
// From state (i, j) with L characters still to place, try each letter c in
// ascending order and jump to its *first* occurrence ni/nj in the two suffixes.
// Take the branch only when lcs[ni+1][nj+1] == L-1, i.e. the letter can start an
// optimal completion.  That test makes every branch productive, so the DFS
// visits at most (#answers * length) nodes -- no memo or pruning table needed.
//
// The two properties that matter fall out of the construction:
//   * distinctness -- forcing the first occurrence makes the map from answer
//     string to DFS path a bijection, so no answer is produced twice;
//   * ordering -- all answers share the same length and letters are tried in
//     ascending order, so the DFS emits them lexicographically.
//
// No statement ambiguity.  Verified against the official sample, the five test
// cases posted on the discuss board (message 140291), and ~1100 randomized
// trials against an exhaustive subsequence-enumeration reference written from
// the statement's own wording.

#include <cstdio>
#include <cstring>

static char A[128], B[128];
static int n, m;
static int lcs[85][85];
static int na[85][26], nb[85][26];
static char out[128];

static void dfs(int i, int j, int L, int depth)
{
    if (L == 0) {
        out[depth] = '\0';
        printf("%s\n", out);
        return;
    }
    for (int c = 0; c < 26; ++c) {
        int ni = na[i][c], nj = nb[j][c];
        if (ni == n || nj == m) continue;
        if (lcs[ni + 1][nj + 1] != L - 1) continue;
        out[depth] = (char)('a' + c);
        dfs(ni + 1, nj + 1, L - 1, depth + 1);
    }
}

int main()
{
    if (scanf("%s", A) != 1) return 0;
    if (scanf("%s", B) != 1) return 0;
    n = (int)strlen(A);
    m = (int)strlen(B);

    for (int j = 0; j <= m; ++j) lcs[n][j] = 0;
    for (int i = 0; i <= n; ++i) lcs[i][m] = 0;
    for (int i = n - 1; i >= 0; --i)
        for (int j = m - 1; j >= 0; --j) {
            if (A[i] == B[j]) lcs[i][j] = lcs[i + 1][j + 1] + 1;
            else lcs[i][j] = lcs[i + 1][j] > lcs[i][j + 1] ? lcs[i + 1][j] : lcs[i][j + 1];
        }

    for (int c = 0; c < 26; ++c) { na[n][c] = n; nb[m][c] = m; }
    for (int i = n - 1; i >= 0; --i)
        for (int c = 0; c < 26; ++c) na[i][c] = (A[i] - 'a' == c) ? i : na[i + 1][c];
    for (int j = m - 1; j >= 0; --j)
        for (int c = 0; c < 26; ++c) nb[j][c] = (B[j] - 'a' == c) ? j : nb[j + 1][c];

    dfs(0, 0, lcs[0][0], 0);
    return 0;
}
