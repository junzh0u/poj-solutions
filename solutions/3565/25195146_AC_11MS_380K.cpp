// POJ 3565 - Ants
// Model: claude-opus-5
//
// Match n ant colonies to n apple trees so the n straight routes are pairwise
// non-intersecting (special judge: any non-crossing perfect matching passes).
//
// Key fact: a minimum-total-Euclidean-length perfect matching never crosses.
// If routes P1-Q1 and P2-Q2 crossed at O, then
//     |P1Q2| + |P2Q1| <= (|P1O|+|OQ2|) + (|P2O|+|OQ1|) = |P1Q1| + |P2Q2|,
// and the inequality is strict because equality needs O on segment P1Q2, i.e.
// Q2 on line P1Q1 -- three collinear points, which the statement excludes.
// So swapping a crossing pair strictly shortens the total; a minimum cannot
// cross.  The bound needs the true distance: it comes from the triangle
// inequality, which squared distances do not satisfy (the discuss board's
// counterexample "3 / -8 -1 / -8 -7 / 0 8 / -5 -8 / -4 -8 / -8 8" is exactly
// a case where the squared-cost optimum crosses).  Hence sqrt(), not d^2.
//
// Stage 1: Hungarian algorithm (Jonker-Volgenant potentials, O(n^3), the slack
// array is what keeps it O(n^3) rather than O(n^4)) over the sqrt distances.
//
// Stage 2: the same swap argument, applied directly, as a certificate.  Stage 1
// runs in double, and the discuss board's recurring complaint on this problem is
// precision, so the answer is not trusted: while any two routes properly cross,
// swap their endpoints.  Crossing is decided by integer orientation with no eps
// -- |coords| <= 10000 makes every cross product at most 8e8, inside 32-bit --
// and each swap strictly shortens the total by the argument above, so the loop
// cannot cycle and must halt at a provably crossing-free matching.  Starting
// from the stage-1 optimum it normally performs zero swaps; it only earns its
// keep if a float comparison in stage 1 slipped.

#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

static const double INF = 1e18;

int n;
int cx[105], cy[105];  // ant colonies, 1..n
int tx[105], ty[105];  // apple trees,  1..n
double a[105][105];

// Orientation of (p, q, r); exact, since every value fits in 32 bits.
static long cross3(long px, long py, long qx, long qy, long rx, long ry) {
    return (qx - px) * (ry - py) - (rx - px) * (qy - py);
}

static int sgn(long v) { return v > 0 ? 1 : (v < 0 ? -1 : 0); }

// Do routes i->mi and j->mj properly cross?  No three of the four points are
// collinear, so no orientation is zero and only proper crossings can occur.
static bool crosses(int i, int mi, int j, int mj) {
    long d1 = cross3(cx[i], cy[i], tx[mi], ty[mi], cx[j], cy[j]);
    long d2 = cross3(cx[i], cy[i], tx[mi], ty[mi], tx[mj], ty[mj]);
    long d3 = cross3(cx[j], cy[j], tx[mj], ty[mj], cx[i], cy[i]);
    long d4 = cross3(cx[j], cy[j], tx[mj], ty[mj], tx[mi], ty[mi]);
    return sgn(d1) * sgn(d2) < 0 && sgn(d3) * sgn(d4) < 0;
}

int main() {
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 1; i <= n; ++i) scanf("%d %d", &cx[i], &cy[i]);
    for (int j = 1; j <= n; ++j) scanf("%d %d", &tx[j], &ty[j]);
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) {
            double dx = (double)cx[i] - tx[j], dy = (double)cy[i] - ty[j];
            a[i][j] = sqrt(dx * dx + dy * dy);
        }

    // ---- stage 1: minimum-cost perfect matching ----
    vector<double> u(n + 1, 0.0), v(n + 1, 0.0);
    vector<int> p(n + 1, 0), way(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<double> minv(n + 1, INF);
        vector<char> used(n + 1, 0);
        do {
            used[j0] = 1;
            int i0 = p[j0], j1 = 0;
            double delta = INF;
            for (int j = 1; j <= n; ++j)
                if (!used[j]) {
                    double cur = a[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                    if (minv[j] < delta) { delta = minv[j]; j1 = j; }
                }
            for (int j = 0; j <= n; ++j) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else minv[j] -= delta;
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    vector<int> m(n + 1, 0);  // m[i] = tree matched to colony i
    for (int j = 1; j <= n; ++j) m[p[j]] = j;

    // ---- stage 2: exact uncrossing certificate ----
    bool clean = false;
    while (!clean) {
        clean = true;
        for (int i = 1; i <= n && clean; ++i)
            for (int j = i + 1; j <= n && clean; ++j)
                if (crosses(i, m[i], j, m[j])) {
                    int t = m[i]; m[i] = m[j]; m[j] = t;
                    clean = false;
                }
    }

    for (int i = 1; i <= n; ++i) printf("%d\n", m[i]);
    return 0;
}
