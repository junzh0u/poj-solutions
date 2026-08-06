// POJ 2677 - Tour
// Model: claude-opus-5
//
// Classic bitonic euclidean TSP (CLRS 15-3). The tour goes strictly left to
// right from the leftmost point to the rightmost, then strictly right to left
// back, which is exactly a partition of the points into two x-monotone paths
// from point 0 to point n-1.
//
// dp[i][j] (i < j) = min total length of two vertex-disjoint x-monotone paths
// that together cover points 0..j, one ending at i, the other ending at j.
// Base dp[0][1] = d(0,1).  Extending to point j+1, the new point must be the
// successor of one of the two path ends:
//   dp[i][j+1] = dp[i][j] + d(j, j+1)                 (extend the path at j)
//   dp[j][j+1] = min_i ( dp[i][j] + d(i, j+1) )       (extend the path at i)
// Answer = min_i ( dp[i][n-1] + d(i, n-1) ).  O(n^2).
//
// Input notes (not in the statement, settled from the discuss board):
//  - the file holds many data sets, read until EOF;
//  - n is small (the board reports n <= 50); 1000 is allocated anyway.
// Coordinates are read as double with %lf and printed with %f -- the board's
// "use float, not double" folklore is just a %f/%lf scanf mismatch.
// n == 1 degenerates to a tour of length 0.

#include <cstdio>
#include <cmath>
#include <algorithm>

const int MAXN = 1005;

struct Point {
    double x, y;
    bool operator<(const Point &o) const { return x < o.x; }
};

Point p[MAXN];
double d[MAXN][MAXN];
double dp[MAXN][MAXN];

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        int i, j;
        for (i = 0; i < n; i++) scanf("%lf %lf", &p[i].x, &p[i].y);
        if (n <= 0) { printf("0.00\n"); continue; }
        std::sort(p, p + n);
        if (n == 1) { printf("0.00\n"); continue; }

        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                double dx = p[i].x - p[j].x, dy = p[i].y - p[j].y;
                d[i][j] = sqrt(dx * dx + dy * dy);
            }

        dp[0][1] = d[0][1];
        for (j = 1; j + 1 < n; j++) {
            double best = -1.0;
            for (i = 0; i < j; i++) {
                double cand = dp[i][j] + d[i][j + 1];
                if (best < 0.0 || cand < best) best = cand;
                dp[i][j + 1] = dp[i][j] + d[j][j + 1];
            }
            dp[j][j + 1] = best;
        }

        double ans = -1.0;
        for (i = 0; i + 1 < n; i++) {
            double cand = dp[i][n - 1] + d[i][n - 1];
            if (ans < 0.0 || cand < ans) ans = cand;
        }
        printf("%.2f\n", ans);
    }
    return 0;
}
