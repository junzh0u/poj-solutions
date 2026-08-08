// POJ 3460 - Booksort
// Model: claude-opus-5
//
// A transposition takes out a block X and slides the adjacent block Y (on
// either side of the hole) into it: XY -> YX for two adjacent blocks.  Asking
// for the minimum number of transpositions to sort, capped at "5 or more".
//
// IDA* over the permutation.  With a sentinel a[n] = n+1, let cnt be the number
// of i in [0,n-1] with a[i+1] != a[i]+1 ("breakpoints").  cnt == 0 iff sorted:
// every adjacency correct forces a[n-1] = n and then a[0] = 1 downward.  One
// transposition cuts the array in exactly three places (before X, between X and
// Y, after Y), so it repairs at most 3 breakpoints, giving the admissible
// heuristic ceil(cnt/3); prune when 3*(maxd-dep) < cnt.  Iterative deepening
// runs maxd = 0..4, so an already sorted shelf answers 0 without a special case
// (the discuss board reports repeated WA from omitting that; the official
// sample contains no sorted shelf and cannot catch it).
//
// Only "slide Y in from the right" is enumerated -- block [i..j] moved to sit
// after k -- because sliding the left neighbour into the hole is the same
// exchange with the roles of X and Y swapped.  Verified against a reference
// that enumerates both directions explicitly.
//
// No statement ambiguity: the board's worked chain 54321 -> 32541 -> 34125 ->
// 12345 confirms the adjacent-block-exchange reading against sample case 2.

#include <cstdio>

int n;

static int diffs(const int *s) {
    int c = 0;
    for (int i = 0; i < n; ++i)
        if (s[i + 1] != s[i] + 1) ++c;
    return c;
}

static bool dfs(int dep, int maxd, const int *s) {
    int c = diffs(s);
    if (c == 0) return true;
    if (3 * (maxd - dep) < c) return false;

    int t[17];
    t[n] = n + 1;
    for (int i = 0; i < n; ++i)
        for (int j = i; j < n; ++j)
            for (int k = j + 1; k < n; ++k) {
                int p = 0;
                for (int x = 0; x < i; ++x) t[p++] = s[x];
                for (int x = j + 1; x <= k; ++x) t[p++] = s[x];
                for (int x = i; x <= j; ++x) t[p++] = s[x];
                for (int x = k + 1; x < n; ++x) t[p++] = s[x];
                if (dfs(dep + 1, maxd, t)) return true;
            }
    return false;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        if (scanf("%d", &n) != 1) break;
        int a[17];
        for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
        a[n] = n + 1;
        int ans = -1;
        for (int d = 0; d <= 4; ++d)
            if (dfs(0, d, a)) { ans = d; break; }
        if (ans < 0) printf("5 or more\n");
        else printf("%d\n", ans);
    }
    return 0;
}
