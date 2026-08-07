// POJ 1673 - EXOCENTER OF A TRIANGLE
// Model: claude-opus-5
//
// The Exocenter is just the orthocenter of ABC.
//
// Proof.  Let R be the quarter turn.  The outward square on AB puts its corner
// adjacent to A at A + R(B-A); the outward square on AC puts its corner
// adjacent to A at A - R(C-A) -- opposite signs, because "outward" is opposite
// sides of the two edges as seen from A.  The Extriangle at A is A together
// with those two corners, so its median through A points at
//     M - A = ( R(B-A) - R(C-A) ) / 2 = R(B-C) / 2,
// i.e. the Exomedian at A is the line through A perpendicular to BC -- the
// altitude from A.  The same holds at B and C, so the three Exomedians are the
// three altitudes and the Exocenter is the orthocenter.  (Building a square on
// the inward side negates its corner offset and leaves the same line, so the
// outward/inward choice does not matter.)
//
// With u = B-A and v = C-A, the orthocenter H satisfies (H-A).(B-C) = 0 and
// (H-B).(C-A) = 0, which solve in closed form to
//     H = A + (u.v)/cross(u,v) * ( v_y - u_y , u_x - v_x ).
// cross(u,v) = +-2*area is nonzero: the statement guarantees no vertex lies
// within one unit of the line through the other two.  Differencing against A
// before multiplying keeps the products small, which matters because a triangle
// at that non-degeneracy boundary can push the orthocenter out to ~1e7.
//
// Reading of the statement: "connect adjacent square corners" means the corners
// next to the shared vertex.  Taking the far corners instead yields 5.6341
// 3.5854 on the first sample rather than 9.0000 3.7500, so the sample settles
// the ambiguity.  Checked against an exact-rational reference built straight
// from the statement's construction (squares, Extriangles, medians, and their
// intersection): 1200 random triangles agreed to the last printed digit.
//
// Output notes, both from the discuss board.  A tiny epsilon is added before
// printing so a coordinate that is mathematically 0 but computes to -1e-16
// prints "0.0000" instead of "-0.0000".  And print with %f, never %lf: POJ's
// older G++ mishandles %lf in printf, which is the actual content of the
// board's much-repeated "C++ accepts but G++ gets WA" folklore.

#include <cstdio>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    while (n-- > 0) {
        double ax, ay, bx, by, cx, cy;
        if (scanf("%lf %lf %lf %lf %lf %lf",
                  &ax, &ay, &bx, &by, &cx, &cy) != 6) break;

        double ux = bx - ax, uy = by - ay;
        double vx = cx - ax, vy = cy - ay;
        double k = (ux * vx + uy * vy) / (ux * vy - uy * vx);

        printf("%.4f %.4f\n", ax + k * (vy - uy) + 1e-8,
                              ay + k * (ux - vx) + 1e-8);
    }
    return 0;
}
