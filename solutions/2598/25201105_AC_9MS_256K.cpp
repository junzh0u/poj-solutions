// POJ 2598 - Match Throwing Game
// Model: claude-sonnet-5
// Buffon's needle: line spacing d = 1, needle length L.
// Single-throw intersection probability:
//   L <= d:  p = 2L / (pi*d)
//   L >  d:  p = (2/(pi*d)) * ( L - sqrt(L^2-d^2) + d*acos(d/L) )
// (continuous at L=d, since sqrt term -> 0 and acos(1) -> 0, matching 2L/(pi*d)).
// Over N=1000 independent throws, the count of intersections is Binomial(N,p);
// its mode (most probable value) is floor((N+1)*p) for non-integer (N+1)*p,
// which matches both samples exactly (L=0.5 -> 318, L=1.21 -> 718).
#include <cstdio>
#include <cmath>
using namespace std;

int main() {
    double L;
    const double d = 1.0;
    const double PI = acos(-1.0);
    while (scanf("%lf", &L) == 1) {
        double p;
        if (L <= d) {
            p = 2.0 * L / (PI * d);
        } else {
            p = (2.0 / (PI * d)) * (L - sqrt(L*L - d*d) + d * acos(d / L));
        }
        double val = 1001.0 * p;
        long ans = (long)floor(val + 1e-9);
        printf("%ld\n", ans);
    }
    return 0;
}
