// POJ 2645 - Boastin' Red Socks
// Model: claude-sonnet-5
//
// Probability of drawing two red socks from n = r+b socks (r red) is
// r(r-1) / (n(n-1)) = p/q. Reduce p/q to lowest terms p1/q1. For a given
// total n, r(r-1) must equal p1 * (n(n-1)/q1), which requires q1 to divide
// n(n-1) exactly. Try n = 2..50000 in increasing order (fewest total socks
// wins) and, for each n where q1 | n(n-1), solve the quadratic r(r-1) = x
// for an integer r in [0, n]. The first n that yields such an r gives the
// answer; if none does, print "impossible".
//
// Special case p == 0: the quadratic approach would report r=1 (root of
// r(r-1)=0 nearest positive infinity), but the board (message 174800/33825)
// confirms the judge expects r=0, b=2 (i.e. "0 2"), not "1 1".
//
// All intermediate values fit comfortably in a 64-bit signed integer: since
// p <= q, after reduction p1 <= q1, and m = n(n-1)/q1 is an exact integer,
// so x = p1*m <= q1*m = n(n-1) <= 49999*50000 < 2.5e9.
#include <cstdio>
#include <cmath>

typedef long long ll;

static ll gcd(ll a, ll b) {
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

int main() {
    ll p, q;
    while (scanf("%lld %lld", &p, &q) == 2) {
        if (p == 0 && q == 0) break;
        if (p == 0) {
            printf("0 2\n");
            continue;
        }
        ll g = gcd(p, q);
        ll p1 = p / g, q1 = q / g;
        bool found = false;
        for (int n = 2; n <= 50000 && !found; ++n) {
            ll total = (ll)n * (n - 1);
            if (total % q1 != 0) continue;
            ll m = total / q1;
            ll x = p1 * m;
            ll r0 = (ll)((1.0 + sqrt(1.0 + 4.0 * (double)x)) / 2.0);
            for (ll r = r0 - 2; r <= r0 + 2; ++r) {
                if (r >= 0 && r <= n && r * (r - 1) == x) {
                    printf("%lld %lld\n", r, (ll)n - r);
                    found = true;
                    break;
                }
            }
        }
        if (!found) printf("impossible\n");
    }
    return 0;
}
