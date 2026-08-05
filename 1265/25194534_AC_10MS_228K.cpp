// POJ 1265 - Area
//
// A robot walks the boundary of a lattice polygon and radios its moves; from
// those moves report I (interior lattice points), E (boundary lattice points)
// and A (the area).
//
// The moves are *relative*: each "dx dy" line is a step from the current
// position, not an absolute vertex, so the vertices have to be accumulated.
// Three standard identities then give everything:
//
//   2A  = |sum over edges of (x_i * y_{i+1} - x_{i+1} * y_i)|   (shoelace)
//   E   = sum over edges of gcd(|dx|, |dy|)
//   I   = A - E/2 + 1  = (2A - E + 2) / 2                       (Pick)
//
// Everything is kept as exact integers: twice the area is an integer, which is
// what makes the rearranged Pick's theorem give an exact interior count, and
// 2A - E is always even so the final division is exact. The only floating point
// is the last printf, where 2A/2.0 is an exact half-integer.
//
// The statement promises an anti-clockwise walk, but taking the absolute value
// of the shoelace sum costs nothing and makes a clockwise input work too.
//
// Ambiguity: the statement says "separate the three numbers by two single
// blanks", which reads like two spaces between numbers; the sample's raw <pre>
// shows a single space between each pair (i.e. two separators, one blank each),
// and that is what is printed here. Each scenario is terminated by a blank line.
//
// Bounds are tiny: m < 100 moves, the polygon fits in a 100x100 square, so 2A
// stays under ~20000 and long is far more than enough.

#include <cstdio>
#include <cstdlib>

static long gcdl(long a, long b) {
    while (b) { long t = a % b; a = b; b = t; }
    return a;
}

int main() {
    int scenarios;
    if (scanf("%d", &scenarios) != 1) return 0;
    for (int s = 1; s <= scenarios; ++s) {
        int m;
        scanf("%d", &m);
        long x = 0, y = 0;
        long twiceArea = 0;   /* shoelace sum, signed */
        long boundary = 0;    /* lattice points on the edges */
        for (int i = 0; i < m; ++i) {
            long dx, dy;
            scanf("%ld %ld", &dx, &dy);
            long nx = x + dx, ny = y + dy;
            twiceArea += x * ny - nx * y;
            boundary += gcdl(labs(dx), labs(dy));
            x = nx; y = ny;
        }
        if (twiceArea < 0) twiceArea = -twiceArea;
        long interior = (twiceArea - boundary + 2) / 2;
        printf("Scenario #%d:\n", s);
        printf("%ld %ld %.1f\n\n", interior, boundary, twiceArea / 2.0);
    }
    return 0;
}
