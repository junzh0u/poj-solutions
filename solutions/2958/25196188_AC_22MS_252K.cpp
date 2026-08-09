// POJ 2958 - Pizza delivery
// Model: claude-sonnet-5
// Approach: Manhattan distance cost decomposes additively into independent
// x and y components: sum_p w_p*(|px-cx|+|py-cy|) = sum_col colSum[col]*|col-cx|
// + sum_row rowSum[row]*|row-cy|. Minimize each 1D weighted-sum-of-distances
// term independently (weighted median) by brute-force over all candidate
// positions (x,y <= 100, so O(x^2+y^2) per test case is trivial).
// Statement/board note: discuss board (msg 177368) confirms the x/y split;
// one post (msg 142390) claims sample 2's answer should be 202, but this is
// unconfirmed/unreplied and disagrees with the official sample output (162),
// which this solution reproduces exactly, so it is disregarded as user error.
#include <cstdio>
#include <cstdlib>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n--) {
        int x, y;
        scanf("%d %d", &x, &y);
        long long colSum[101];
        long long rowSum[101];
        for (int i = 1; i <= x; i++) colSum[i] = 0;
        for (int j = 1; j <= y; j++) rowSum[j] = 0;
        for (int row = 1; row <= y; row++) {
            for (int col = 1; col <= x; col++) {
                int d;
                scanf("%d", &d);
                colSum[col] += d;
                rowSum[row] += d;
            }
        }
        long long bestX = -1;
        for (int cx = 1; cx <= x; cx++) {
            long long cost = 0;
            for (int col = 1; col <= x; col++) {
                long long diff = col - cx;
                if (diff < 0) diff = -diff;
                cost += colSum[col] * diff;
            }
            if (bestX < 0 || cost < bestX) bestX = cost;
        }
        long long bestY = -1;
        for (int cy = 1; cy <= y; cy++) {
            long long cost = 0;
            for (int row = 1; row <= y; row++) {
                long long diff = row - cy;
                if (diff < 0) diff = -diff;
                cost += rowSum[row] * diff;
            }
            if (bestY < 0 || cost < bestY) bestY = cost;
        }
        printf("%lld blocks\n", bestX + bestY);
    }
    return 0;
}
