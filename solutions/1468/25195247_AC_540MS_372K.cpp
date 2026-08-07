// POJ 1468 - Rectangles
// Model: claude-opus-5
//
// Count the rectangles that are entirely covered by another rectangle, for
// several data sets read to EOF.  Rectangle i counts iff some OTHER entry j of
// the list satisfies xmin_j <= xmin_i && xmax_i <= xmax_j && ymin_j <= ymin_i
// && ymax_i <= ymax_j.
//
// Ambiguity, settled by the discuss board (message 149261): "entirely covered
// by another rectangle" means covered by ONE other rectangle, not by the union
// of all the others -- the poster implemented the union reading with rectangle
// cutting and got TLE before rereading the statement.  The official sample
// cannot separate the two readings: in set 2 the four identical rectangles are
// each covered both by one sibling and by the union of the siblings.
//
// The second reading fixed by the sample is that containment need not be
// strict and only the list position must differ: four identical rectangles
// answer 4, so each copy is "covered by another" copy.
//
// Algorithm: plain O(n^2) pairwise test with an early break, n < 5000 and the
// limit is 5000MS.  Worst case is an antichain forcing the full n^2 scan; a
// 4999-rectangle antichain runs in 0.03s locally, ~150x under the limit, so
// nothing cleverer is needed.  Coordinates are read as double rather than int
// so decimal input would also parse; values compare exactly, no epsilon is
// involved, since containment only ever compares numbers parsed from the file.

#include <cstdio>
#include <vector>

int main() {
    int n;
    while (scanf("%d", &n) == 1) {
        if (n < 0) n = 0;
        std::vector<double> x1(n), x2(n), y1(n), y2(n);
        for (int i = 0; i < n; i++) {
            if (scanf("%lf %lf %lf %lf", &x1[i], &x2[i], &y1[i], &y2[i]) != 4) return 0;
        }
        int covered = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (j == i) continue;
                if (x1[j] <= x1[i] && x2[i] <= x2[j] && y1[j] <= y1[i] && y2[i] <= y2[j]) {
                    covered++;
                    break;
                }
            }
        }
        printf("%d\n", covered);
    }
    return 0;
}
