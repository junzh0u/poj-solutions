// POJ 3407 - Brookebond s'en va en guerre...
// Model: claude-opus-5
//
// Great-circle distance on a sphere of radius 6370 km between two points given
// as degrees + minutes with a hemisphere letter.
//
// Signs: N/E positive, S/W negative; value = deg + min/60.  The statement notes
// E and W are equivalent on the 0 and 180 meridians, which needs no special
// case: +180 and -180 are the same point, as are +0 and -0.
//
// The central angle is computed as atan2(|u x v|, u . v) on the unit vectors
// rather than acos(u . v).  This is not a cosmetic choice: for identical points
// the dot product rounds to just above 1 and for exactly antipodal points to
// just below -1, so acos returns NaN on both -- reproduced locally, and the
// discuss board reports the same trap (its advice is to use arctan instead of
// arccos).  atan2 of the cross-product norm against the dot product is exact at
// both ends and needs no clamping or epsilon.
//
// Input framing: the statement describes exactly two lines, but the board
// reports the real data may hold several cases, so points are read in pairs
// until EOF.  With a single case that still emits exactly one line.
//
// Output: 3 decimals, matching the sample and the stated 1-metre precision;
// the rounding is the only error left (worst 0.4993 m over 652 checked cases,
// against a 60-digit Decimal reference).

#include <cstdio>
#include <cmath>

static const double R = 6370.0;
static const double PI = 3.14159265358979323846264338327950288;

static int readPoint(double *lat, double *lon)
{
    double d1, m1, d2, m2;
    char h1[16], h2[16];
    if (scanf("%lf %lf %15s %lf %lf %15s", &d1, &m1, h1, &d2, &m2, h2) != 6)
        return 0;
    double la = d1 + m1 / 60.0;
    if (h1[0] == 'S' || h1[0] == 's') la = -la;
    double lo = d2 + m2 / 60.0;
    if (h2[0] == 'W' || h2[0] == 'w') lo = -lo;
    *lat = la * PI / 180.0;
    *lon = lo * PI / 180.0;
    return 1;
}

int main()
{
    double la1, lo1, la2, lo2;
    while (readPoint(&la1, &lo1)) {
        if (!readPoint(&la2, &lo2)) break;
        double dlon = lo2 - lo1;
        double c1 = cos(la1), s1 = sin(la1);
        double c2 = cos(la2), s2 = sin(la2);
        double cd = cos(dlon), sd = sin(dlon);
        double a = c2 * sd;                      /* |u x v| components */
        double b = c1 * s2 - s1 * c2 * cd;
        double num = sqrt(a * a + b * b);
        double den = s1 * s2 + c1 * c2 * cd;     /* u . v */
        printf("%.3f\n", atan2(num, den) * R);
    }
    return 0;
}
