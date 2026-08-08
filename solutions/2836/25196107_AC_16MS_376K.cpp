// POJ 2836 - Rectangular Covering
// Model: claude-fable-5
//
// Bitmask DP over pair-spanned rectangles. Each rectangle is the bounding box
// of a point pair, credited with every point lying inside that closed bbox
// (board case: the 4 corners of a unit square cost 1, not 2). A zero dimension
// is clamped to 1 since degenerate rectangles are not allowed but integral
// dimensions are required. Verified equal to an exact DP over bounding boxes
// of arbitrary point subsets on thousands of random instances, so the pair
// restriction loses nothing.
// dp[mask] = min area covering `mask`; pick the lowest uncovered point i and
// relax over every pair rectangle whose cover mask contains i -- i need not be
// one of the two defining points (it can lie strictly inside the rectangle;
// restricting to pairs through i is a real bug, +1 on ~0.2% of random cases).
#include <stdio.h>

int x[15], y[15];
int area[15][15], cover[15][15];
int dp[1 << 15];

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n > 0) {
        int i, j, k, s;
        for (i = 0; i < n; i++) scanf("%d %d", &x[i], &y[i]);
        for (i = 0; i < n; i++) for (j = 0; j < n; j++) {
            if (i == j) continue;
            int x1 = x[i] < x[j] ? x[i] : x[j];
            int x2 = x[i] + x[j] - x1;
            int y1 = y[i] < y[j] ? y[i] : y[j];
            int y2 = y[i] + y[j] - y1;
            int dx = x2 - x1, dy = y2 - y1;
            area[i][j] = (dx ? dx : 1) * (dy ? dy : 1);
            int m = 0;
            for (k = 0; k < n; k++)
                if (x[k] >= x1 && x[k] <= x2 && y[k] >= y1 && y[k] <= y2)
                    m |= 1 << k;
            cover[i][j] = m;
        }
        int full = (1 << n) - 1;
        const int INF = 0x3f3f3f3f;
        for (s = 0; s <= full; s++) dp[s] = INF;
        dp[0] = 0;
        for (s = 0; s < full; s++) {
            if (dp[s] == INF) continue;
            int lo = 0;
            while ((s >> lo) & 1) lo++;
            for (i = 0; i < n; i++) for (j = i + 1; j < n; j++) {
                if (!((cover[i][j] >> lo) & 1)) continue;
                int t = s | cover[i][j];
                if (dp[s] + area[i][j] < dp[t]) dp[t] = dp[s] + area[i][j];
            }
        }
        printf("%d\n", dp[full]);
    }
    return 0;
}
