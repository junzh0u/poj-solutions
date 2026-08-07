// POJ 1329 - Circle Through Three Points
// Model: claude-opus-5
//
// Circumcircle of three non-collinear points, printed in two algebraic forms.
//
// The centre is equidistant from A, B, C, so subtracting |P-A|^2 = |P-B|^2 and
// |P-A|^2 = |P-C|^2 leaves a 2x2 linear system, solved by Cramer's rule (its
// determinant is twice the signed area of ABC, nonzero because the statement
// guarantees the points are not collinear).  A is translated to the origin
// first: the system is much better conditioned in the offsets (u,v), and the
// constant term of equation (2) then comes out without a cancellation.
//
// Expanding (x-h)^2 + (y-k)^2 = r^2 into x^2 + y^2 + cx + dy - e = 0 gives
//     c = -2h,  d = -2k,  -e = h^2 + k^2 - r^2
// so the value printed after "x" in equation (1) is -h (likewise -k after "y"),
// and the trailing constant of equation (2) is h^2 + k^2 - r^2.  With the centre
// at A + (u,v) and r^2 = u^2 + v^2 that constant is Ax^2 + Ay^2 + 2Ax*u + 2Ay*v,
// which is what the code evaluates -- no large-cancellation subtraction.
//
// Every term prints as "<sign> <|value|>" with three decimals, so no double sign
// can ever appear.  The input holds many data sets, one per line, read to EOF.
// A blank line follows every pair, the last one included.
//
// Arithmetic is done in long double and narrowed to double only for printing (no
// %Lf, which old runtimes mishandle).  Where long double is wider than double --
// as on the judge's x87 -- that is free extra headroom for values landing exactly
// on a .xxx5 rounding boundary, e.g. "12 41 -21 13 -10 -31", whose d is exactly
// 0.8625; where it is not, the code degrades to plain double evaluation.
//
// Ambiguity: the statement does not say which sign a term that rounds to 0.000
// carries.  The discuss board reports the judge data has no such case, and both
// the board's worked example (-1 1 1 1 1 -1 -> "(x + 0.000)^2 + (y + 0.000)^2")
// and an Accepted submission quoted there use '+', so anything whose magnitude
// rounds to 0.000 prints as "+ 0.000" and never "- 0.000".

#include <cstdio>
#include <cmath>

/* Print " <sign> <|v|>" with three decimals, choosing '+' for anything that
   rounds to zero so that "-0.000" can never be emitted. */
static void term(double v, const char *suffix) {
    char sign = (v < -0.0005) ? '-' : '+';
    double a = fabs(v);
    if (a < 0.0005) a = 0.0; /* also kills the IEEE -0.0 case */
    printf(" %c %.3f%s", sign, a, suffix);
}

int main() {
    double ax, ay, bx, by, cx, cy;
    while (scanf("%lf %lf %lf %lf %lf %lf", &ax, &ay, &bx, &by, &cx, &cy) == 6) {
        long double bux = (long double)bx - ax, buy = (long double)by - ay;
        long double cux = (long double)cx - ax, cuy = (long double)cy - ay;
        long double det = 2.0L * (bux * cuy - cux * buy);
        long double rb = bux * bux + buy * buy;
        long double rc = cux * cux + cuy * cuy;
        long double u = (rb * cuy - rc * buy) / det; /* centre relative to A */
        long double v = (bux * rc - cux * rb) / det;
        long double h = (long double)ax + u;
        long double k = (long double)ay + v;
        double r = sqrt((double)(u * u + v * v)); /* plain sqrt: no overload doubt */
        long double konst = (long double)ax * ax + (long double)ay * ay
                          + 2.0L * ((long double)ax * u + (long double)ay * v);

        printf("(x");
        term((double)(-h), ")^2");
        printf(" + (y");
        term((double)(-k), ")^2");
        printf(" = %.3f^2\n", r);

        printf("x^2 + y^2");
        term((double)(-2.0L * h), "x");
        term((double)(-2.0L * k), "y");
        term((double)konst, "");
        printf(" = 0\n\n");
    }
    return 0;
}
