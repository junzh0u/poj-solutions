// POJ 1173 - Bar Codes
// Model: claude-sonnet-5
// Approach: BC(n,k,m) is the set of compositions of n into k parts each in
// [1,m], mapped to alternating dark/light run-length strings starting dark.
// ways[i][j] = number of compositions of i into j parts each <= m (DP,
// O(n*k*m)). Total count = ways[n][k].
// To rank a given symbol: parse it into k run widths. Walk the bars left to
// right; for a dark (odd-indexed, 1-based) bar, any smaller width at that
// position yields a lexicographically smaller string (an earlier switch to
// '0'); for a light bar, any *larger* width yields a lexicographically
// smaller string (delays the next '1' longer). For each bar, sum
// ways[remaining_length_after_alt][remaining_bars_after_this] over the
// alternative widths that produce a smaller string, and accumulate into the
// rank. No statement ambiguity: n,k,m <= 33 keeps counts within a 32-bit int
// (max is C(32,16) = 601080390, confirmed on the discuss board).
#include <cstdio>
#include <cstring>

static int ways[34][34]; // ways[i][j]: compositions of i into j parts, each in [1,m]

int main() {
    int n, k, m;
    if (scanf("%d %d %d", &n, &k, &m) != 3) return 0;

    memset(ways, 0, sizeof(ways));
    ways[0][0] = 1;
    for (int j = 1; j <= k; ++j) {
        for (int i = 1; i <= n; ++i) {
            int s = 0;
            int hi = i < m ? i : m;
            for (int w = 1; w <= hi; ++w) {
                s += ways[i - w][j - 1];
            }
            ways[i][j] = s;
        }
    }

    printf("%d\n", ways[n][k]);

    int s;
    if (scanf("%d", &s) != 1) return 0;
    static char buf[64];
    for (int q = 0; q < s; ++q) {
        scanf("%s", buf);
        int len = (int)strlen(buf);
        // parse into run widths
        int widths[34];
        int cnt = 0;
        int i = 0;
        while (i < len) {
            char c = buf[i];
            int j = i;
            while (j < len && buf[j] == c) ++j;
            widths[cnt++] = j - i;
            i = j;
        }

        int rank = 0;
        int remLen = n;
        int remBars = k;
        for (int idx = 0; idx < cnt; ++idx) {
            int w = widths[idx];
            bool dark = (idx % 2 == 0);
            if (dark) {
                for (int w2 = 1; w2 < w; ++w2) {
                    int left = remLen - w2;
                    int barsLeft = remBars - 1;
                    if (left >= 0 && left <= n && barsLeft >= 0)
                        rank += ways[left][barsLeft];
                }
            } else {
                for (int w2 = w + 1; w2 <= m; ++w2) {
                    int left = remLen - w2;
                    int barsLeft = remBars - 1;
                    if (left >= 0 && left <= n && barsLeft >= 0)
                        rank += ways[left][barsLeft];
                }
            }
            remLen -= w;
            remBars -= 1;
        }
        printf("%d\n", rank);
    }

    return 0;
}
