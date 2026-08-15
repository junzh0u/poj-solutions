// POJ 1624 - This Takes the Cake
// Model: claude-sonnet-5
//
// The cake is a convex quadrilateral. Legal cuts join two of the 8 boundary
// points: the 4 vertices and the 4 edge midpoints. Listing these 8 points in
// boundary (counterclockwise) order gives a convex 8-gon P[0..7] where
// P[0],P[2],P[4],P[6] are the original vertices and the odd-indexed points
// are the midpoints of the edges between them. Any chord joining two of
// these 8 points splits the convex quadrilateral into exactly the two
// polygons formed by walking the boundary from one endpoint to the other
// (inclusive) and closing with the straight chord - this is valid precisely
// because all 8 points already lie in convex position on the boundary, so a
// chain of consecutive boundary points plus the chord is itself a simple
// convex polygon. So we just enumerate all C(8,2)=28 pairs, compute one
// side's area via the shoelace formula, derive the other side by
// subtracting from the total area, and keep the pair minimizing the
// difference. Pairs that are boundary-adjacent (e.g. a vertex and the
// midpoint of one of its own edges) degrade to a zero-area piece and can
// never be optimal, so no special-casing is needed to exclude them.
//
// Ambiguity check: the sample's cut (1,2)-(5.5,1) is confirmed to be the
// midpoint of the V3-V0 edge and the midpoint of the V1-V2 edge under this
// point numbering, matching the expected areas 4.375/5.125 exactly.

#include <cstdio>
#include <cmath>

struct Pt { double x, y; };

int main() {
    double x[4], y[4];
    int caseNo = 0;
    while (scanf("%lf %lf %lf %lf %lf %lf %lf %lf",
                  &x[0], &y[0], &x[1], &y[1], &x[2], &y[2], &x[3], &y[3]) == 8) {
        bool allZero = true;
        for (int i = 0; i < 4; ++i) {
            if (x[i] != 0.0 || y[i] != 0.0) { allZero = false; break; }
        }
        if (allZero) break;

        Pt P[8];
        for (int i = 0; i < 4; ++i) {
            P[2 * i].x = x[i];
            P[2 * i].y = y[i];
        }
        for (int i = 0; i < 4; ++i) {
            int j = (i + 1) % 4;
            P[2 * i + 1].x = (x[i] + x[j]) / 2.0;
            P[2 * i + 1].y = (y[i] + y[j]) / 2.0;
        }

        double total = 0.0;
        for (int i = 0; i < 8; ++i) {
            int j = (i + 1) % 8;
            total += P[i].x * P[j].y - P[j].x * P[i].y;
        }
        total = fabs(total) / 2.0;

        double bestDiff = -1.0, bestA = 0.0, bestB = 0.0;
        for (int i = 0; i < 8; ++i) {
            for (int j = i + 1; j < 8; ++j) {
                double s = 0.0;
                for (int k = i; k < j; ++k) {
                    s += P[k].x * P[k + 1].y - P[k + 1].x * P[k].y;
                }
                s += P[j].x * P[i].y - P[i].x * P[j].y;
                double area1 = fabs(s) / 2.0;
                double area2 = total - area1;
                double diff = fabs(area1 - area2);
                if (bestDiff < 0.0 || diff < bestDiff) {
                    bestDiff = diff;
                    if (area1 <= area2) { bestA = area1; bestB = area2; }
                    else { bestA = area2; bestB = area1; }
                }
            }
        }

        ++caseNo;
        printf("Cake %d: %.3f %.3f\n", caseNo, bestA, bestB);
    }
    return 0;
}
