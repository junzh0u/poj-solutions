// POJ 2978 - Colored stones
// Model: claude-opus-5
//
// Remove the fewest stones so that every colour that survives forms one
// contiguous block.  Equivalently: cut [0,m) into consecutive segments, give
// each segment a distinct colour, and keep exactly the stones of that colour
// inside it; maximise the number kept.  Any legal final row is of that shape,
// since each surviving colour occupies one interval and the intervals are
// disjoint and ordered, so the two formulations have the same optimum.
//
// dp[mask][i] = most stones kept when positions [0,i) are fully partitioned
// into blocks of the colours in mask, each colour used at most once.  With
// prefix counts the transition is O(1) per (mask, i, colour, cut), i.e.
// O(2^d * m^2 * d) <= 32 * 100^2 * 5 per case.  d is the number of *distinct*
// colours actually present rather than the declared k, so a colour outside
// {1..k} in the data cannot overflow the mask.
//
// The answer is the best dp[*][m] over every mask, not dp[full][m]: a segment
// must span at least one position, so a colour left unused at i = m can never
// be added afterwards, and requiring the full mask would reject optima that
// drop a colour entirely.
//
// Input framing: multiple test cases read to EOF, terminated by "0 0".

#include <cstdio>
#include <algorithm>

static const int MAXM = 105;
static const int MAXD = 12;   /* far more distinct colours than the stated k <= 5 */

int main() {
    static int x[MAXM], c[MAXM], pal[MAXM];
    static int pre[MAXD][MAXM];
    static int dp[1 << MAXD][MAXM];

    int m, k;
    while (scanf("%d %d", &m, &k) == 2) {
        if (m == 0 && k == 0) break;
        int i, j, t, mask;
        for (i = 0; i < m; i++) scanf("%d", &x[i]);

        for (i = 0; i < m; i++) pal[i] = x[i];
        std::sort(pal, pal + m);
        int d = (int)(std::unique(pal, pal + m) - pal);
        if (d > MAXD) d = MAXD;   /* unreachable for k <= 5; keeps the mask in range */
        for (i = 0; i < m; i++)
            c[i] = (int)(std::lower_bound(pal, pal + d, x[i]) - pal);

        for (t = 0; t < d; t++) {
            pre[t][0] = 0;
            for (i = 0; i < m; i++) pre[t][i + 1] = pre[t][i] + (c[i] == t ? 1 : 0);
        }

        int full = 1 << d;
        for (mask = 0; mask < full; mask++)
            for (i = 0; i <= m; i++) dp[mask][i] = -1;
        dp[0][0] = 0;

        int best = 0;
        for (mask = 0; mask < full; mask++) {
            for (i = 0; i <= m; i++) {
                int cur = dp[mask][i];
                if (cur < 0) continue;
                if (i == m) { if (cur > best) best = cur; continue; }
                for (t = 0; t < d; t++) {
                    if (mask & (1 << t)) continue;
                    int *dn = dp[mask | (1 << t)];
                    const int *pt = pre[t];
                    int base = cur - pt[i];
                    for (j = i + 1; j <= m; j++) {
                        int v = base + pt[j];
                        if (v > dn[j]) dn[j] = v;
                    }
                }
            }
        }
        printf("%d\n", m - best);
    }
    return 0;
}
