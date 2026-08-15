// POJ 1648 - Countryside Highway
// Model: claude-sonnet-5
// Approach: The highway is the segment from (0,W) to (100N,E). For each of the
// N columns i (x in [100i,100(i+1)]) the segment's y-value is a monotone linear
// function of x, so its y-range over that column is [min(Y0,Y1),max(Y0,Y1)]
// (kept as an integer numerator Y(x)=W*L+(E-W)*x scaled by L=100N to avoid
// floating point). An estate (i,j) is "crossed" (touching counts, per the
// discuss board: touching a grid corner counts all 4 surrounding estates) iff
// the segment shares at least one point with the closed square [100i,100(i+1)]
// x [100j,100(j+1)]; that is exactly the closed-interval overlap test between
// the column's y-range and the row's y-range, computed here with an O(1)
// integer formula per column (no floating point anywhere).
// Ambiguity: statement says "just touches ... still considered a crossing";
// the discuss board (message 14845) confirms a line passing exactly through an
// interior grid corner counts all 4 surrounding estates, which the exact
// closed-interval-overlap formulation reproduces automatically (verified
// against a literal O(N^2) segment-vs-rectangle brute force).
#include <cstdio>
#include <algorithm>
using namespace std;

int main(){
    int n, w, e;
    while (scanf("%d %d %d", &n, &w, &e) == 3) {
        long L = 100L * n;
        long count = 0;
        for (int i = 0; i < n; i++) {
            long x0 = 100L * i;
            long x1 = 100L * (i + 1);
            long Y0 = (long)w * L + (long)(e - w) * x0;
            long Y1 = (long)w * L + (long)(e - w) * x1;
            long ylo = min(Y0, Y1);
            long yhi = max(Y0, Y1);
            long denom = 100L * L;
            long jmax = yhi / denom;
            if (jmax > n - 1) jmax = n - 1;
            long jmin = (ylo + denom - 1) / denom - 1;
            if (jmin < 0) jmin = 0;
            if (jmax >= jmin) count += (jmax - jmin + 1);
        }
        printf("%ld\n", count);
    }
    return 0;
}
