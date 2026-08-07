// POJ 3440 - Coin Toss
// Model: claude-sonnet-5
// Approach: closed-form geometric probability. Place the coin center's
// position relative to the grid lines as (dx, dy) = distances to the
// nearest internal vertical / horizontal grid line. With radius r = c/2
// (and r < t/2 guaranteed since c < t), the coin covers:
//   own tile (always) + left/right neighbor iff dx<r + top/bottom neighbor
//   iff dy<r + diagonal neighbor iff dx^2+dy^2<r^2 (distance to nearest
//   grid vertex). The diagonal condition implies both dx<r and dy<r, so
//   tile counts are: 1 (neither), 2 (exactly one of dx<r,dy<r), 3 (both
//   but not within r of the vertex), 4 (within r of the vertex).
// Summing over all Nx=n-1 internal vertical lines (length m*t each),
// Ny=m-1 internal horizontal lines (length n*t each), and Nxy=(n-1)(m-1)
// internal vertices (each contributing a full disk of area pi*r^2 and a
// square of area 4r^2, non-overlapping since r<t/2) gives areas for the
// four events by inclusion-exclusion; dividing by total area A=m*n*t^2
// gives the probabilities. Verified by hand against all 3 sample cases.
// Ambiguity: none in the statement itself; the discuss board's own
// gripe threads ("PE", "tile" singular vs "tiles" spacing, precision of
// pi, negative zero) are all encoding/formatting pitfalls, not
// algorithmic ones, and are handled below. m,n,t,c are read as ints but
// every product is computed in double (board: "changed m,n,t,c to
// double and it AC'd") since m*n*t^2 can reach 2.5e13, overflowing
// 32-bit int silently.
#include <cstdio>
#include <cmath>
using namespace std;

static const double PI = 3.14159265358979323846264338327950288;

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 1; tc <= T; ++tc) {
        int m, n, t, c;
        scanf("%d %d %d %d", &m, &n, &t, &c);
        double r = (double)c / 2.0;
        double dm = (double)m, dn = (double)n, dt = (double)t;
        double A = dm * dn * dt * dt;
        double Nx = dn - 1.0;      // internal vertical lines
        double Ny = dm - 1.0;      // internal horizontal lines
        double Nxy = (dn - 1.0) * (dm - 1.0); // internal vertices

        double areaV = Nx * (dm * dt) * (2.0 * r);
        double areaH = Ny * (dn * dt) * (2.0 * r);
        double areaVH = Nxy * 4.0 * r * r;
        double areaD = Nxy * PI * r * r;

        double p1 = (A - areaV - areaH + areaVH) / A;
        double p2 = (areaV + areaH - 2.0 * areaVH) / A;
        double p3 = (areaVH - areaD) / A;
        double p4 = areaD / A;

        double q1 = p1 * 100.0, q2 = p2 * 100.0, q3 = p3 * 100.0, q4 = p4 * 100.0;
        if (fabs(q1) < 1e-9) q1 = 0.0;
        if (fabs(q2) < 1e-9) q2 = 0.0;
        if (fabs(q3) < 1e-9) q3 = 0.0;
        if (fabs(q4) < 1e-9) q4 = 0.0;

        printf("Case %d:\n", tc);
        printf("Probability of covering 1 tile  = %.4f%%\n", q1);
        printf("Probability of covering 2 tiles = %.4f%%\n", q2);
        printf("Probability of covering 3 tiles = %.4f%%\n", q3);
        printf("Probability of covering 4 tiles = %.4f%%\n", q4);
        if (tc != T) printf("\n");
    }
    return 0;
}
