// POJ 2714 - Random Walk
// Model: claude-sonnet-5
//
// We choose a sign s_i in {-1,+1} for each of the n given vectors and want
// to maximize |sum s_i * v_i|.  The set of all attainable sums forms a
// centrally symmetric convex polygon (a zonogon) Z = Minkowski sum of the
// segments [-v_i, v_i], centered at the origin.  The farthest point from
// the origin (the center) must be a vertex of Z.
//
// For a sweep direction theta, the point of Z that maximizes the dot
// product with theta_hat = (cos theta, sin theta) is obtained by picking,
// independently for each i, the sign that maximizes s_i*(v_i . theta_hat),
// i.e. s_i = sign(v_i . theta_hat).  As theta sweeps once around the
// circle, this vertex only changes when v_i . theta_hat crosses zero for
// some i, which happens at exactly two angles per vector (theta
// perpendicular to v_i), so 2n critical angles total.  Sweeping through
// them in order and flipping the corresponding sign each time enumerates
// every vertex of the zonogon in O(n log n); the answer is the maximum
// norm among them.
//
// Ambiguity: none in the statement; verified against all three samples by
// hand-tracing the algorithm before coding (sample 1 gives exactly 3.000
// via the vertex (0,3), matching the printed answer).
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

static double g_ang[210];

bool angLess(int a, int b) { return g_ang[a] < g_ang[b]; }

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static double x[105], y[105];
        for (int i = 0; i < n; ++i) scanf("%lf %lf", &x[i], &y[i]);

        const double PI = acos(-1.0);
        // critical angles, normalized to (0, 2*PI]
        static int idx[210];
        int m = 0;
        for (int i = 0; i < n; ++i) {
            double a = atan2(y[i], x[i]);
            double c1 = a + PI / 2.0;
            double c2 = a - PI / 2.0;
            double cs[2] = {c1, c2};
            for (int k = 0; k < 2; ++k) {
                double v = cs[k];
                while (v <= 1e-9) v += 2 * PI;
                while (v > 2 * PI + 1e-9) v -= 2 * PI;
                g_ang[m] = v;
                idx[m] = i;
                ++m;
            }
        }
        // sort by angle
        static int order[210];
        for (int i = 0; i < m; ++i) order[i] = i;
        sort(order, order + m, angLess);

        static int s[105];
        double Sx = 0, Sy = 0;
        for (int i = 0; i < n; ++i) {
            if (x[i] > 0) s[i] = 1;
            else if (x[i] < 0) s[i] = -1;
            else s[i] = (y[i] > 0) ? 1 : -1;
            Sx += s[i] * x[i];
            Sy += s[i] * y[i];
        }
        double best = Sx * Sx + Sy * Sy;
        for (int t = 0; t < m; ++t) {
            int i = idx[order[t]];
            Sx -= 2 * s[i] * x[i];
            Sy -= 2 * s[i] * y[i];
            s[i] = -s[i];
            double d2 = Sx * Sx + Sy * Sy;
            if (d2 > best) best = d2;
        }
        printf("Maximum distance = %.3f meters.\n", sqrt(best));
    }
    return 0;
}
