// POJ 1299 - Polar Explorer
// Model: claude-opus-5
//
// Each data set is "START" / "X Y Z" / "END", terminated by "ENDOFINPUT".
// The rover travels along the planet's surface (a circle of radius X) to the
// crash site and back, at 5 miles per gallon, so its range is 5*Y miles.
//
// Two readings the statement leaves open, both settled by the discuss board:
//   * Which arc?  Z may exceed 180 degrees, and the figure suggests following
//     the drawn direction, but the rover takes the SHORTER arc: z = min(Z, 360-Z).
//     The official sample cannot separate the readings - its only Z > 180 case
//     (100 50 270) is "NO 250" either way, because the answer is NO under both.
//   * The trip is a round trip ("get to the crash site and back"), so the
//     distance is 2 * (pi * X * z / 180), not half of it.
// Fuel left is truncated toward zero, not rounded, and exactly-zero fuel on
// return still counts as YES (the comparison is dist <= 5*Y, not <).
//
// Floating point is safe here even though the output is a truncation.  With
// pi fixed at 3.14159 = 314159/100000, the round-trip distance is the rational
// 314159*X*z/9000000 and the fuel left is (45000000*Y - 314159*X*z)/45000000.
// Since gcd(314159, 45000000) = 1 and X*z <= 100*180 < 314159, that fraction is
// an integer only when X*z = 0, so apart from the exact case dist = 0 every
// value sits at least 1/45000000 (~2.2e-8) away from an integer - far outside
// double's error.  The same argument covers the dist <= 5*Y comparison.
// Verified by enumerating the entire input domain (X,Y,Z) = 100*101*361 cases
// against an exact integer-arithmetic reference: identical output everywhere.

#include <cstdio>
#include <cstring>

int main() {
    const double PI = 3.14159;
    char tok[64];

    while (scanf("%63s", tok) == 1) {
        if (strcmp(tok, "ENDOFINPUT") == 0) break;
        if (strcmp(tok, "START") != 0) continue;

        int x, y, z;
        if (scanf("%d %d %d", &x, &y, &z) != 3) break;
        if (scanf("%63s", tok) != 1) break;  /* "END" */

        if (z > 180) z = 360 - z;
        double dist = 2.0 * PI * x * (double)z / 180.0;  /* there and back */
        double range = 5.0 * y;

        if (dist <= range)
            printf("YES %d\n", (int)(y - dist / 5.0));
        else
            printf("NO %d\n", 5 * y);
    }
    return 0;
}
