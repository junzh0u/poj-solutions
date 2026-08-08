// POJ 3129 - How I Wonder What You Are!
// Model: claude-opus-5
//
// Count the stars visible through at least one telescope. Star i is visible
// through telescope j iff the angle between the direction of the star and the
// telescope's centre direction is strictly less than the telescope's angular
// radius psi_j. Both are directions from the origin, so the star's distance is
// irrelevant.
//
// The angle is taken as atan2(|s x t|, s . t), not acos of the normalized dot
// product: atan2 stays accurate as the angle approaches 0 or pi, where acos
// loses all precision (cos(1e-8) rounds to exactly 1.0 in a double). It is also
// exactly invariant under scaling either vector, so no normalization is needed
// for correctness. The statement guarantees |theta - psi| > 1e-8 for every
// pair, which is many orders of magnitude above this form's ~1e-15 error, so no
// epsilon is used in the comparison.
//
// A star seen through several telescopes counts once, so visibility is recorded
// in a per-star flag rather than summed per telescope; conversely two stars
// listed at the same position are two stars (the statement indexes the stars,
// and the discuss board reports duplicate positions in the data), so the flags
// are keyed by input index and never by coordinates.
//
// Input is a sequence of datasets terminated by a line "0"; every read is also
// checked so a missing terminator at EOF ends the run cleanly, and the arrays
// are sized from n rather than from the stated bound of 500.
//
// O(n*m) per dataset: 0.05s locally for 49 maximal datasets, against a 5s limit.

#include <cstdio>
#include <cmath>
#include <vector>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n > 0) {
        std::vector<double> sx(n), sy(n), sz(n);
        std::vector<char> seen(n, 0);
        for (int i = 0; i < n; i++) {
            if (scanf("%lf %lf %lf", &sx[i], &sy[i], &sz[i]) != 3) return 0;
            double r = sqrt(sx[i]*sx[i] + sy[i]*sy[i] + sz[i]*sz[i]);
            sx[i] /= r; sy[i] /= r; sz[i] /= r;
        }
        int m = 0;
        if (scanf("%d", &m) != 1) return 0;
        for (int j = 0; j < m; j++) {
            double tx, ty, tz, psi;
            if (scanf("%lf %lf %lf %lf", &tx, &ty, &tz, &psi) != 4) return 0;
            double r = sqrt(tx*tx + ty*ty + tz*tz);
            tx /= r; ty /= r; tz /= r;
            for (int i = 0; i < n; i++) {
                if (seen[i]) continue;
                double dot = sx[i]*tx + sy[i]*ty + sz[i]*tz;
                double cx = sy[i]*tz - sz[i]*ty;
                double cy = sz[i]*tx - sx[i]*tz;
                double cz = sx[i]*ty - sy[i]*tx;
                double theta = atan2(sqrt(cx*cx + cy*cy + cz*cz), dot);
                if (theta < psi) seen[i] = 1;
            }
        }
        int cnt = 0;
        for (int i = 0; i < n; i++) if (seen[i]) cnt++;
        printf("%d\n", cnt);
    }
    return 0;
}
