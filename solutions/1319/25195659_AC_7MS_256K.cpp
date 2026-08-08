// POJ 1319 - Pipe Fitters
// Model: claude-opus-5
//
// Unit-diameter pipes in a W x H rectangle, in one of exactly two patterns.
//
// Grid: square lattice, floor(W) * floor(H).  Orientation-independent.
//
// Skew: hexagonal lattice.  Rows are tangent to the row below, so consecutive
// row centres differ by sqrt(3)/2 vertically and 0.5 horizontally; row r
// (0-based) has centre height 0.5 + r*sqrt(3)/2, i.e. r+1 rows need height
// 1 + r*sqrt(3)/2.  A row starting at offset o in {0, 0.5} holds floor(W - o)
// pipes, so the offset rows hold floor(W - 0.5) -- which equals floor(W), not
// floor(W)-1, when the fractional part of W is at least 0.5 (sample 2.9 10.5).
// Starting on a full row is optimal, but both parities are tried anyway.
//
// The skew pattern is NOT orientation-independent, so both W x H and H x W
// must be tried: 2.9 x 10 packs 22 pipes upright and 29 pipes on its side.
//
// Ties go to "grid" per the statement; the sample never exercises a tie, so
// 1 1 (one pipe either way) was constructed to check that rule.
//
// Input is a sequence of container dimensions read to EOF.
//
// All comparisons use a 1e-9 epsilon.  Row heights 1 + r*sqrt(3)/2 are
// irrational for r >= 1 and so never sit on an integer boundary; the widths
// do (W - 0.5 is exact in binary for any one-decimal W), and there the epsilon
// absorbs the decimal-to-double rounding of the input.

#include <cstdio>
#include <cmath>

static const double EPS = 1e-9;

static int ifloor(double x) {
    if (x < 0.0) return 0;
    return (int)floor(x + EPS);
}

static int skewCount(double W, double H) {
    const double S = sqrt(3.0) / 2.0;
    int best = 0;
    for (int startOffset = 0; startOffset < 2; ++startOffset) {
        int total = 0;
        for (int r = 0;; ++r) {
            if (1.0 + r * S > H + EPS) break;
            double o = ((r + startOffset) % 2) ? 0.5 : 0.0;
            total += ifloor(W - o);
        }
        if (total > best) best = total;
    }
    return best;
}

int main() {
    double a, b;
    while (scanf("%lf %lf", &a, &b) == 2) {
        int grid = ifloor(a) * ifloor(b);
        int skew = skewCount(a, b);
        int skew2 = skewCount(b, a);
        if (skew2 > skew) skew = skew2;
        if (skew > grid) printf("%d skew\n", skew);
        else printf("%d grid\n", grid);
    }
    return 0;
}
