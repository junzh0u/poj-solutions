// POJ 2954 - Triangle
// Model: claude-sonnet-5
// Approach: Pick's theorem. For a lattice polygon, Area = I + B/2 - 1,
// where I is the number of interior lattice points and B is the number of
// boundary lattice points. So I = Area - B/2 + 1.
// B for a triangle is the sum over the three edges of gcd(|dx|, |dy|)
// (gcd(0, n) = n handles axis-aligned edges correctly).
// Area is computed via the shoelace formula; we keep everything in terms of
// twice the area (2*Area) to stay in integers, since coordinates are lattice
// points so 2*Area is always an integer. Then 2*I = 2*Area - B + 2.
// No ambiguity in the statement; verified against both sample cases.
#include <cstdio>
#include <cstdlib>

typedef long long ll;

static ll gcd(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        ll t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int main() {
    ll x1, y1, x2, y2, x3, y3;
    while (scanf("%lld %lld %lld %lld %lld %lld", &x1, &y1, &x2, &y2, &x3, &y3) == 6) {
        if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0 && x3 == 0 && y3 == 0) break;

        ll twiceArea = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
        if (twiceArea < 0) twiceArea = -twiceArea;

        ll b = gcd(x2 - x1, y2 - y1) + gcd(x3 - x2, y3 - y2) + gcd(x1 - x3, y1 - y3);

        ll twiceI = twiceArea - b + 2;
        ll interior = twiceI / 2;

        printf("%lld\n", interior);
    }
    return 0;
}
