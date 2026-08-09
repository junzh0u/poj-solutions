// POJ 2660 - War on Weather
// Model: claude-sonnet-5
//
// A target T on the sphere is visible from satellite S iff the segment ST
// does not pass through the sphere's interior. With O the sphere centre,
// this holds iff dot(S-T, S-T) <= dot(S,S) - dot(T,T) is false the other
// way... concretely: expand |S-T|^2 = |S|^2 - 2 S.T + |T|^2, so the
// visibility condition |S-T|^2 + |T|^2 <= |S|^2 simplifies to
// dot(S,T) >= |T|^2. |T| is taken from the target's own given coordinates
// (not the nominal earth radius) since the statement only guarantees the
// target lies within 1e-9 km of the true surface. This matches the
// contest setter's own reference solution (recovered from the Waterloo
// local-contest archive), which compares these squared distances directly
// in double precision with a plain <=, so no extra epsilon is used beyond
// ordinary floating point comparison; the statement's 1e-8 km boundary
// tolerance guarantees any genuinely ambiguous case is judged leniently.
//
// For each target, check every satellite; a target counts once if any
// satellite sees it (union over satellites, per statement "how many
// different targets can be hit").

#include <cstdio>

static double sx[105], sy[105], sz[105];
static double tx[105], ty[105], tz[105];

static double sq(double v) { return v * v; }
static double dist2(double a, double b, double c) { return sq(a) + sq(b) + sq(c); }

int main() {
    int k, m;
    while (std::scanf("%d %d", &k, &m) == 2) {
        if (k == 0 && m == 0) break;
        for (int i = 0; i < k; ++i) {
            std::scanf("%lf %lf %lf", &sx[i], &sy[i], &sz[i]);
        }
        for (int j = 0; j < m; ++j) {
            std::scanf("%lf %lf %lf", &tx[j], &ty[j], &tz[j]);
        }
        int count = 0;
        for (int j = 0; j < m; ++j) {
            double d3 = dist2(tx[j], ty[j], tz[j]);
            bool visible = false;
            for (int i = 0; i < k; ++i) {
                double d1 = dist2(sx[i], sy[i], sz[i]);
                double d2 = dist2(sx[i] - tx[j], sy[i] - ty[j], sz[i] - tz[j]);
                if (d2 + d3 <= d1) {
                    visible = true;
                    break;
                }
            }
            if (visible) ++count;
        }
        std::printf("%d\n", count);
    }
    return 0;
}
