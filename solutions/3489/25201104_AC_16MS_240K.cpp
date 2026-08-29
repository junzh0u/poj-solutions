// POJ 3489 - Knapsack I
// Model: claude-sonnet-5
//
// Each item's size can be recursively divided into k equal parts, so from
// a stock of items of size s we can produce any nonnegative integer number
// of parts of size s / k^m for any m >= 0.  Multiplying every chosen part
// up to a common subdivision level e, filling the knapsack exactly means
//   x = (a_1*s_1 + a_2*s_2 + ... + a_n*s_n) / k^e
// for some e >= 0 and nonnegative integers a_i, i.e.
//   x * k^e  is representable as a nonnegative integer combination of the s_i.
// Let g = gcd(s_1,...,s_n).  By the Chicken McNugget / numerical-semigroup
// theorem, once the target multiple of g is large enough, *every* multiple
// of g is representable (the generators s_i/g are coprime).  Since e can be
// taken arbitrarily large, the only real obstruction is divisibility: for
// every prime p dividing g, we need vp(x) + e*vp(k) >= vp(g) for some common
// e.  Primes of g that also divide k can always be fixed by taking e large
// enough; primes of g that do NOT divide k must already fully divide x.
// So: repeatedly strip from g every prime factor it shares with k
// (g /= gcd(g,k) while gcd(g,k) > 1); the answer is Yes iff the remaining
// value divides x exactly.
//
// Equivalent formulation found on the discuss board (message 108736):
// reduce b = g / gcd(g,x), then answer is Yes iff repeatedly dividing b by
// gcd(b,k) drives b to 1.  Both formulations were verified equal by
// comparing prime valuations, and both match the sample.
//
// The discuss board (115483) also shows the *stated* n <= 1000 does not
// bound the actual data (submitters saw RE with arrays up to ~4000-4500);
// avoided entirely here since only a running gcd of the n sizes is needed,
// never an array.

#include <cstdio>

typedef long long ll;

ll gcd(ll a, ll b) {
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

int main() {
    ll n, x, k;
    while (scanf("%lld %lld %lld", &n, &x, &k) == 3) {
        ll g = 0;
        for (ll i = 0; i < n; ++i) {
            ll s;
            scanf("%lld", &s);
            g = gcd(g, s);
        }
        ll b = g / gcd(g, x);
        ll d = gcd(b, k);
        while (d > 1) {
            b /= d;
            d = gcd(b, k);
        }
        puts(b == 1 ? "Yes" : "No");
    }
    return 0;
}
