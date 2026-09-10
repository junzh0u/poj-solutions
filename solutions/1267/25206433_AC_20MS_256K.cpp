// POJ 1267 - Fence
// Model: claude-sonnet-5
//
// Among all simple polygons with a fixed multiset of side lengths, the one
// inscribed in a circle (a cyclic polygon) has maximum area, provided the
// longest side is strictly less than the sum of the others (otherwise no
// closed polygon -- area 0). So binary search the circumradius R.
//
// For a chord of length a and circumradius R, the two endpoints subtend a
// central angle of either 2*asin(a/2R) (the "short way") or its explement
// 2*PI - 2*asin(a/2R) (the "long way"). Going around the polygon once, the
// n per-edge central angles (each taken along the arc that does not pass
// through any other vertex) always sum to exactly 2*PI. In the generic case
// every edge uses the short-way angle, giving the defining equation
//   sum_i 2*asin(a_i/2R) = 2*PI                      (center inside polygon)
// This is only solvable for R >= Lmax/2 when, at R = Lmax/2 (where the max
// term hits its ceiling of PI), the remaining terms already sum to at least
// PI. Otherwise the longest edge's own arc is the major (>PI) one, so
//   2*PI - 2*asin(Lmax/2R) + sum_{i!=max} 2*asin(a_i/2R) = 2*PI
//   <=>  sum_{i!=max} asin(a_i/2R) = asin(Lmax/2R)    (center outside polygon)
// Both defining functions are monotonic in R over the valid range, so each
// case is solved by its own binary search; the area is the sum of the
// per-edge circular-segment triangle areas, with the longest edge's triangle
// subtracted instead of added in the center-outside case.
//
// Ambiguity check: sample (10,5,5,4 -> 28.00) matches Brahmagupta's formula
// for a cyclic quadrilateral exactly, confirming the cyclic-polygon reading.
// The discuss board (message 125280) gives two further checks that pin the
// two branches: (3,5,4 -> 6.00), a right triangle sitting exactly on the
// boundary between the branches, and (100 x6 -> 25980.76), a regular hexagon
// (interior center, generic branch). Both matched locally. Board message
// 359048 warns against a truncated PI literal; acos(-1.0) is used instead.
#include <cstdio>
#include <cmath>
using namespace std;

static const double PI = acos(-1.0);

int n;
double a[105];

int main(){
    scanf("%d", &n);
    double sum = 0, mx = -1;
    int mxi = -1;
    for (int i = 0; i < n; i++) {
        scanf("%lf", &a[i]);
        sum += a[i];
        if (a[i] > mx) { mx = a[i]; mxi = i; }
    }
    double rest = sum - mx;
    if (mx >= rest) {
        printf("0.00\n");
        return 0;
    }

    double lo = mx / 2.0;
    double s0 = 0;
    for (int i = 0; i < n; i++) if (i != mxi) s0 += asin(a[i] / (2.0 * lo));
    bool caseA = (s0 >= PI / 2.0);

    double R;
    double L = lo, H = 1e9;
    if (caseA) {
        for (int iter = 0; iter < 200; iter++) {
            double mid = (L + H) / 2.0;
            double s = 0;
            for (int i = 0; i < n; i++) s += asin(a[i] / (2.0 * mid));
            if (s > PI) L = mid; else H = mid;
        }
    } else {
        for (int iter = 0; iter < 200; iter++) {
            double mid = (L + H) / 2.0;
            double so = 0;
            for (int i = 0; i < n; i++) if (i != mxi) so += asin(a[i] / (2.0 * mid));
            double sm = asin(mx / (2.0 * mid));
            if (so > sm) H = mid; else L = mid;
        }
    }
    R = (L + H) / 2.0;

    double area = 0;
    for (int i = 0; i < n; i++) {
        double tri = (a[i] / 4.0) * sqrt(4.0 * R * R - a[i] * a[i]);
        if (!caseA && i == mxi) area -= tri; else area += tri;
    }
    printf("%.2f\n", area);
    return 0;
}
