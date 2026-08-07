// POJ 3101 - Astronomy
// Model: claude-sonnet-5
// Approach: assume a parade occurs at t=0 (WLOG for computing the *interval*
// between consecutive parades: the set of parade times is an additive subgroup
// of R -- each pairwise "same line" condition is a linear congruence mod pi in
// t -- so the spacing between consecutive elements does not depend on the
// initial phases). For planet i versus a fixed reference planet 1, angle
// equality mod pi requires (2pi/t1 - 2pi/ti) * t = k*pi for integer k, i.e. the
// minimal positive t satisfying just that pair is t1*ti / (2*|t1-ti|), a
// reduced fraction p_i/q_i. "Same angle mod pi versus planet 1" is transitive,
// so requiring every pair (i,j) collinear reduces to requiring every pair
// (1,i) collinear. The overall answer is LCM over i of these fractions, which
// equals LCM(numerators)/GCD(denominators) already in lowest terms (standard
// fact about LCM of reduced fractions). Numerators can need arbitrary
// precision (LCM of up to ~1000 values each up to 1e8), so the running LCM is
// kept as a base-1e9 bignum; the denominator GCD stays small (<=20000) and
// needs no bignum. The discuss board's own debate ("does this really need big
// integers?" / "sure") confirmed bignum is required rather than optional
// before any code was written.
#include <cstdio>
#include <vector>
using namespace std;

typedef long long ll;
typedef vector<ll> Big;
const ll BASE = 1000000000LL;

Big bigFromInt(ll x) {
    Big r;
    if (x == 0) { r.push_back(0); return r; }
    while (x > 0) { r.push_back(x % BASE); x /= BASE; }
    return r;
}

void bigTrim(Big &a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
}

Big bigMulSmall(const Big &a, ll m) {
    Big r(a.size() + 3, 0);
    ll carry = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        ll cur = a[i] * m + carry;
        r[i] = cur % BASE;
        carry = cur / BASE;
    }
    size_t i = a.size();
    while (carry > 0) {
        r[i] = carry % BASE;
        carry /= BASE;
        ++i;
    }
    bigTrim(r);
    return r;
}

ll bigModSmall(const Big &a, ll m) {
    ll rem = 0;
    for (size_t i = a.size(); i > 0; --i) {
        rem = (rem * BASE + a[i-1]) % m;
    }
    return rem;
}

Big bigDivSmall(const Big &a, ll m) {
    Big r(a.size(), 0);
    ll rem = 0;
    for (size_t i = a.size(); i > 0; --i) {
        ll cur = rem * BASE + a[i-1];
        r[i-1] = cur / m;
        rem = cur % m;
    }
    bigTrim(r);
    return r;
}

ll gcdll(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

void bigPrint(const Big &a) {
    printf("%lld", a.back());
    for (size_t i = a.size() - 1; i > 0; --i) {
        printf("%09lld", a[i-1]);
    }
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<ll> t(n);
    for (int i = 0; i < n; ++i) scanf("%lld", &t[i]);

    ll ref = t[0];
    Big L = bigFromInt(1);
    ll G = 0;

    for (int i = 1; i < n; ++i) {
        if (t[i] == ref) continue;
        ll a = 2 * (ref > t[i] ? ref - t[i] : t[i] - ref);
        ll b = ref * t[i];
        ll g = gcdll(a, b);
        ll p = b / g;
        ll q = a / g;

        ll r = bigModSmall(L, p);
        ll g2 = gcdll(p, r);
        L = bigMulSmall(bigDivSmall(L, g2), p);

        G = gcdll(G, q);
    }

    if (G == 0) G = 1;

    ll r2 = bigModSmall(L, G);
    ll g3 = gcdll(G, r2);
    Big finalNum = bigDivSmall(L, g3);
    ll finalDen = G / g3;

    bigPrint(finalNum);
    printf(" %lld\n", finalDen);
    return 0;
}
