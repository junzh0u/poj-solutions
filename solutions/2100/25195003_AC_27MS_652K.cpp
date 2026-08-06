// POJ 2100 - Graveyard Design
// Model: claude-sonnet-5
// Approach: for a fixed section count l, sum_{i=a}^{a+l-1} i^2 = n expands to
// a quadratic in a: l*a^2 + l(l-1)*a + [(l-1)l(2l-1)/6 - n] = 0. Solve via
// the quadratic formula for each l from 1 upward while the minimal possible
// sum (a=1) does not exceed n; l tops out near cbrt(3n) (~6.7e4 for
// n=1e14), so this is O(n^{1/3}) per test case. The discriminant can reach
// ~2e19, past unsigned 64-bit range, so it and its integer square root are
// computed in __int128 (a GNU extension, kept despite -pedantic because
// plain 64-bit overflows near the largest l). Collected (a,b) pairs are
// sorted by descending section count l before output.
// Statement ambiguity: the problem text reads like a single-instance input
// ("Input file contains n ..."), but the discuss board confirms the actual
// input holds multiple test cases (one n per line), read until EOF; this is
// not stated in the problem text itself.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef __int128 lll;

static lll isqrt128(lll x) {
    if (x < 0) return -1;
    long double xld = (long double)x;
    ll r = (ll)sqrtl(xld);
    if (r < 0) r = 0;
    while ((lll)r * r > x) r--;
    while ((lll)(r + 1) * (r + 1) <= x) r++;
    return r;
}

struct Design {
    ll a, b;
};

bool cmpDesign(const Design &x, const Design &y) {
    return (x.b - x.a) > (y.b - y.a);
}

int main() {
    ll n;
    while (scanf("%lld", &n) == 1) {
        vector<Design> results;
        for (ll l = 1; ; l++) {
            lll minsum = (lll)l * (l + 1) * (2 * l + 1) / 6;
            if (minsum > (lll)n) break;
            ll C = (l - 1) * l * (2 * l - 1) / 6 - n;
            lll term1 = (lll)l * l * (lll)(l - 1) * (l - 1);
            lll D = term1 - (lll)4 * l * C;
            if (D < 0) continue;
            lll sq = isqrt128(D);
            if (sq * sq != D) continue;
            lll numer = sq - (lll)l * (l - 1);
            lll denom = (lll)2 * l;
            if (numer <= 0) continue;
            if (numer % denom != 0) continue;
            lll a = numer / denom;
            if (a < 1) continue;
            ll av = (ll)a;
            ll bv = av + l - 1;
            Design d;
            d.a = av;
            d.b = bv;
            results.push_back(d);
        }
        sort(results.begin(), results.end(), cmpDesign);
        printf("%d\n", (int)results.size());
        for (size_t i = 0; i < results.size(); i++) {
            ll a = results[i].a, b = results[i].b;
            printf("%lld", (ll)(b - a + 1));
            for (ll v = a; v <= b; v++) printf(" %lld", v);
            printf("\n");
        }
    }
    return 0;
}
