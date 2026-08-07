// POJ 1408 - Fishnet
// Model: claude-opus-5
//
// A unit square frame carries n pegs per edge.  Thread i joins (a_i,0) to
// (b_i,1); thread i also joins (0,c_i) to (1,d_i).  Adding the four frame
// edges gives n+2 "vertical" segments and n+2 "horizontal" ones, cutting the
// square into (n+1)^2 quadrilateral meshes.  Report the largest mesh area.
//
// The left/right frame edges are just the degenerate vertical threads with
// a=b=0 and a=b=1, and the bottom/top edges the horizontal ones with c=d=0
// and c=d=1, so all n+2 lines can be handled by one formula.
//
// Intersection of vertical line (A,0)-(B,1) with horizontal line (0,C)-(1,D):
// a point on the first is (A+(B-A)t, t), on the second (s, C+(D-C)s).  Then
//   s = A+(B-A)t  and  t = C+(D-C)s
//   => t = (C + (D-C)*A) / (1 - (D-C)*(B-A)),  s = A + (B-A)*t.
// The denominator is 1 - (D-C)(B-A) with |D-C|<1 and |B-A|<1, so it lies in
// (0,2) and never vanishes -- no epsilon or slope special-casing is needed.
//
// Each mesh is the quadrilateral P[i][j], P[i+1][j], P[i+1][j+1], P[i][j+1];
// its area comes from the shoelace formula.  With n<=30 there are at most
// 32*32 = 1024 grid points and 31*31 meshes, so brute force is ample.
//
// Notes: the pegs are given in increasing order along each edge and the
// threads pair up by index, so the coordinates must NOT be sorted -- the
// pairing (a_i,b_i) is part of the input.  Input is a sequence of sub-problems
// terminated by a line "0".  The discuss board reports "%.6lf" drawing WA on
// this judge where "%.6f" is accepted, and reports language 4 (C++) accepting
// code that 0 (G++) rejects; both are honoured here.

#include <cstdio>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        double A[32], B[32], C[32], D[32];
        // index 0 = left/bottom frame edge, 1..n = threads, n+1 = right/top
        A[0] = B[0] = 0.0;
        A[n + 1] = B[n + 1] = 1.0;
        C[0] = D[0] = 0.0;
        C[n + 1] = D[n + 1] = 1.0;
        int i, j;
        for (i = 1; i <= n; ++i) scanf("%lf", &A[i]);
        for (i = 1; i <= n; ++i) scanf("%lf", &B[i]);
        for (i = 1; i <= n; ++i) scanf("%lf", &C[i]);
        for (i = 1; i <= n; ++i) scanf("%lf", &D[i]);

        double px[32][32], py[32][32];
        for (i = 0; i <= n + 1; ++i) {
            for (j = 0; j <= n + 1; ++j) {
                double t = (C[j] + (D[j] - C[j]) * A[i]) /
                           (1.0 - (D[j] - C[j]) * (B[i] - A[i]));
                px[i][j] = A[i] + (B[i] - A[i]) * t;
                py[i][j] = t;
            }
        }

        double best = 0.0;
        for (i = 0; i <= n; ++i) {
            for (j = 0; j <= n; ++j) {
                double x1 = px[i][j], y1 = py[i][j];
                double x2 = px[i + 1][j], y2 = py[i + 1][j];
                double x3 = px[i + 1][j + 1], y3 = py[i + 1][j + 1];
                double x4 = px[i][j + 1], y4 = py[i][j + 1];
                double s = x1 * y2 - x2 * y1 + x2 * y3 - x3 * y2 +
                           x3 * y4 - x4 * y3 + x4 * y1 - x1 * y4;
                if (s < 0.0) s = -s;
                s *= 0.5;
                if (s > best) best = s;
            }
        }
        printf("%.6f\n", best);
    }
    return 0;
}
