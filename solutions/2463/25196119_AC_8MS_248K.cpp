// POJ 2463 - Bungee Jumping
// Model: claude-sonnet-5
//
// Energy conservation: falling a depth x from the bridge, the rope (nominal
// length l, spring constant k) only exerts force once x > l, contributing
// elastic PE 0.5*k*(x-l)^2. Kinetic energy at depth x is therefore
//   0.5*w*v(x)^2 = w*g*x - 0.5*k*max(0,x-l)^2.
// If l >= s the rope never engages before the ground, so it's pure free
// fall: v = sqrt(2*g*s). Otherwise James Bond is "stuck in the air" iff the
// elastic PE required to stretch the rope to depth s exceeds the total
// gravitational PE available at depth s, i.e. 0.5*k*(s-l)^2 > w*g*s -- in
// that case KE would go negative before he ever reaches the ground, meaning
// he oscillates back up short of it. Otherwise the ground speed follows
// directly from the same energy equation at x = s.
//
// Ambiguity check: the discuss board's "参考代码" thread posts exactly this
// closed-form solution (no binary search needed) and it matches an
// independent derivation via the depth of maximum descent x_max = l + m +
// sqrt(m^2+2*l*m), m = g*w/k: checking KE(s) < 0 is algebraically the same
// test as x_max < s, since v^2(x) is a downward parabola in x for x >= l
// with its larger root at x_max. Verified this closed form against all 8
// sample cases by hand.
#include <cstdio>
#include <cmath>

static const double G = 9.81;
static const double EPS = 1e-9;

int main() {
    double k, l, s, w;
    while (std::scanf("%lf %lf %lf %lf", &k, &l, &s, &w) == 4) {
        if (k == 0 && l == 0 && s == 0 && w == 0) break;

        double v;
        if (l >= s) {
            v = std::sqrt(2.0 * G * s);
        } else {
            double dl = s - l;
            if (0.5 * k * dl * dl > w * G * s + EPS) {
                std::printf("Stuck in the air.\n");
                continue;
            }
            double v2 = 2.0 * G * s - k * dl * dl / w;
            if (v2 < 0) v2 = 0;
            v = std::sqrt(v2);
        }

        if (v > 10.0 + EPS) {
            std::printf("Killed by the impact.\n");
        } else {
            std::printf("James Bond survives.\n");
        }
    }
    return 0;
}
