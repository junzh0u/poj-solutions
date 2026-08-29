// POJ 3334 - Connected Gheeves
// Model: claude-sonnet-5
//
// Each gheef is a V-shaped funnel: a polyline p_1..p_n whose y-coordinates
// strictly decrease to a unique cusp p_c and then strictly increase, with the
// left chain (indices < c) all left of the cusp's x and the right chain
// (indices > c) all right of it, forming a convex, non-self-intersecting
// container. At any horizontal level y, the cross-sectional area of water is
// the sum, over each side, of the trapezoid area between that wall and the
// vertical line through the cusp (integral of horizontal offset over the
// submerged height); this reduces to a per-segment trapezoid sum that stops
// (with a linear interpolation of the boundary point) at the query level.
//
// The two gheeves are joined at their cusps by a zero-volume pipe, so both
// share a single global water surface height L (classic communicating
// vessels): volume(L) = 0 below a gheef's own cusp, otherwise the trapezoid
// sum above, and area(L) is monotonic non-decreasing in L. Binary-searching
// L against the poured amount a gives the answer -- but the crucial
// (non-obvious) point, confirmed by the discuss board (message 151557,
// "注意二分上界" / "watch the binary search upper bound", and an independently
// posted solution using hi = min(min(P top ys), min(Q top ys))) is that the
// system-wide ceiling is min(capP, capQ), NOT max: since both gheeves share
// one surface, the moment that surface would need to rise past the LOWER of
// the two rims (min(y_1,y_n) on whichever side), that side overflows
// continuously and holds the entire shared surface there forever, however
// much more is poured -- the higher-rimmed gheef never gets to use its extra
// headroom. The two official samples both settle well below either rim, so
// this only shows up once a is large enough to reach a cap; the board is the
// only source that pins it down (author reported WA with max, AC with min).
//
// Verified against both official samples (3.536, and -15.000, the latter
// exercising the case where the whole answer stays inside one funnel because
// global L never reaches the other funnel's cusp).

#include <cstdio>
#include <cmath>
#include <vector>

using namespace std;

struct Point {
    double x, y;
};

static double cal(const vector<Point>& p, double y, int c, int n) {
    if (y < p[c].y) return 0.0;
    double cx = p[c].x;
    double ret = 0.0;
    int i;
    for (i = 0; i < c; ++i) {
        double a, b, h;
        if (y >= p[i].y) {
            h = p[i].y - p[i+1].y;
            a = p[i].x;
        } else if (y >= p[i+1].y) {
            h = y - p[i+1].y;
            a = p[i].x + (p[i+1].x - p[i].x) * (y - p[i].y) / (p[i+1].y - p[i].y);
        } else {
            h = 0.0;
            a = 0.0;
        }
        b = p[i+1].x;
        ret += (cx - a + cx - b) / 2.0 * h;
    }
    for (i = c; i < n - 1; ++i) {
        double a, b, h;
        if (y >= p[i+1].y) {
            h = p[i+1].y - p[i].y;
            a = p[i+1].x;
        } else if (y >= p[i].y) {
            h = y - p[i].y;
            a = p[i].x + (p[i+1].x - p[i].x) * (y - p[i].y) / (p[i+1].y - p[i].y);
        } else {
            h = 0.0;
            a = 0.0;
        }
        b = p[i].x;
        ret += (a - cx + b - cx) / 2.0 * h;
    }
    return ret;
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        double a;
        scanf("%lf", &a);

        int n1;
        scanf("%d", &n1);
        vector<Point> P(n1);
        int c1 = 0;
        for (int i = 0; i < n1; ++i) {
            scanf("%lf %lf", &P[i].x, &P[i].y);
            if (P[i].y < P[c1].y) c1 = i;
        }

        int n2;
        scanf("%d", &n2);
        vector<Point> Q(n2);
        int c2 = 0;
        for (int i = 0; i < n2; ++i) {
            scanf("%lf %lf", &Q[i].x, &Q[i].y);
            if (Q[i].y < Q[c2].y) c2 = i;
        }

        double capP = P[0].y < P[n1-1].y ? P[0].y : P[n1-1].y;
        double capQ = Q[0].y < Q[n2-1].y ? Q[0].y : Q[n2-1].y;
        double cuspP = P[c1].y;
        double cuspQ = Q[c2].y;

        double lo = cuspP < cuspQ ? cuspP : cuspQ;
        double hi = capP < capQ ? capP : capQ;

        for (int iter = 0; iter < 200; ++iter) {
            double mid = lo + (hi - lo) / 2.0;
            double vol = cal(P, mid, c1, n1) + cal(Q, mid, c2, n2);
            if (vol >= a) hi = mid; else lo = mid;
        }

        printf("%.3f\n", lo);
    }
    return 0;
}
