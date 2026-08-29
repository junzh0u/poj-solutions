// POJ 3029 - Tour Guide
// Model: claude-sonnet-5
//
// The guide starts at the origin with speed V. Each of n<=8 tour guests walks
// along a fixed ray from time 0 until intercepted; once intercepted it is
// redirected and walks straight back to the origin at its own speed. The
// guide's only decision is the order in which to intercept the guests: for a
// fixed order, chasing each guest along a straight line and intercepting at
// the earliest possible instant is always optimal (it minimizes both that
// guest's own finish time and the time/position available for the rest, and
// it never forecloses any option since after redirecting a guest there is no
// "later interception point" left to use). So the whole problem reduces to
// trying all n! guest orders (n<=8, so <=40320) and, for each, computing the
// earliest interception times via a closed-form quadratic (the squared
// pursuit equation |P_i(t)-S|^2 = v^2*(t-t0)^2 is a downward parabola in t
// since v exceeds every guest speed, so it has exactly one root >= t0, taken
// as the larger root). The answer is the minimum, over all orders, of the
// maximum finish time (interception time + return-walk time) across guests.
//
// Statement ambiguity: whether the guide must escort a guest home before
// fetching the next one. The discuss board (message 56661/56662) confirms no
// escorting is needed -- a redirected guest walks home independently while
// the guide moves on immediately from the interception point. The board also
// flags a common bug of updating the guide's position incorrectly after an
// interception (message 75480); this solution updates it to the exact
// interception point (not the guest's eventual home position), which the
// pursuit equation already computes.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

int n;
double V;
double X[8], Y[8], VXx[8], VYy[8], SPD[8];

// Earliest time t >= t0 at which a guide starting at (sx,sy) at time t0,
// moving at speed V, can coincide with guest i (who follows its fixed ray
// from time 0). Returns t0 if already coincident.
double interceptTime(int i, double sx, double sy, double t0) {
    double Ax = X[i] - sx, Ay = Y[i] - sy;
    double ux = VXx[i], uy = VYy[i];
    double a = ux * ux + uy * uy - V * V;           // < 0 since V > speed_i
    double b = 2.0 * (Ax * ux + Ay * uy) + 2.0 * V * V * t0;
    double c = Ax * Ax + Ay * Ay - V * V * t0 * t0;
    double disc = b * b - 4.0 * a * c;
    if (disc < 0) disc = 0;
    double sq = sqrt(disc);
    double t1 = (-b + sq) / (2.0 * a);
    double t2 = (-b - sq) / (2.0 * a);
    double best = max(t1, t2);
    if (best < t0) best = t0;
    return best;
}

double simulate(int perm[]) {
    double sx = 0, sy = 0, t0 = 0, ans = 0;
    for (int k = 0; k < n; ++k) {
        int i = perm[k];
        double ti = interceptTime(i, sx, sy, t0);
        double px = X[i] + VXx[i] * ti;
        double py = Y[i] + VYy[i] * ti;
        double finish = ti + sqrt(px * px + py * py) / SPD[i];
        if (finish > ans) ans = finish;
        sx = px; sy = py; t0 = ti;
    }
    return ans;
}

int main() {
    while (scanf("%d", &n) == 1 && n) {
        scanf("%lf", &V);
        for (int i = 0; i < n; ++i) {
            double a;
            scanf("%lf %lf %lf %lf", &X[i], &Y[i], &SPD[i], &a);
            VXx[i] = SPD[i] * cos(a);
            VYy[i] = SPD[i] * sin(a);
        }
        int perm[8];
        for (int i = 0; i < n; ++i) perm[i] = i;
        double best = 1e18;
        do {
            double v = simulate(perm);
            if (v < best) best = v;
        } while (next_permutation(perm, perm + n));
        printf("%.0f\n", floor(best + 0.5));
    }
    return 0;
}
