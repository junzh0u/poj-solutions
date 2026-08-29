// POJ 2720 - Last Digits
// Model: claude-sonnet-5
// f(x) = b^f(x-1), f(0)=1; output last n digits of f(i).
// This is a power tower b^b^...^b (i copies) modulo 10^n. Since 10^n is small
// (n<=7), the standard technique is the generalized Euler theorem: for any
// a, m and exponent e with e >= log2(m),
//   a^e mod m = a^(e mod phi(m) + phi(m)) mod m
// (true even when gcd(a,m) != 1). We recurse down the tower reducing the
// modulus by phi each level; recursion bottoms out quickly since repeated
// phi of anything <= 10^7 reaches 1 within about 25 steps.
// To decide, at each level, whether the exponent f(x-1) is "large enough"
// to apply the lemma, we track a saturating (capped) exact value of f(x-1):
// rawcap[x] = min(f(x), CAP) computed directly by saturating exponentiation.
// Since CAP (2*10^7) comfortably exceeds any modulus we ever use (<=10^7)
// and log2(10^7) ~= 23.3, a threshold of 40 safely distinguishes "small,
// exact" exponents (used directly) from "large" ones (where the lemma
// applies). No statement ambiguity found; matches the known generalized
// Euler tower technique discussed on the problem's board.
#include <cstdio>
typedef long long ll;

static const ll CAP = 20000000LL;
static const ll THRESH = 40;

ll phi(ll x) {
    ll result = x;
    for (ll p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            result -= result / p;
        }
    }
    if (x > 1) result -= result / x;
    return result;
}

ll satmul(ll a, ll b, ll cap) {
    ll prod = a * b;
    if (prod > cap) return cap + 1;
    return prod;
}

ll powcap(ll base, ll exp, ll cap) {
    ll result = 1;
    ll cur = base;
    if (cur > cap) cur = cap + 1;
    while (exp > 0) {
        if (exp & 1) result = satmul(result, cur, cap);
        cur = satmul(cur, cur, cap);
        exp >>= 1;
    }
    if (result > cap) result = cap + 1;
    return result;
}

ll powmod(ll base, ll exp, ll mod) {
    if (mod == 1) return 0;
    ll result = 1 % mod;
    ll cur = base % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * cur) % mod;
        cur = (cur * cur) % mod;
        exp >>= 1;
    }
    return result;
}

ll b_global;
ll rawcap[105];

ll compute(int x, ll m) {
    if (m == 1) return 0;
    if (x == 0) return 1 % m;
    ll phim = phi(m);
    ll ecap = rawcap[x - 1];
    ll exponent;
    if (ecap >= THRESH) {
        ll emod = compute(x - 1, phim);
        exponent = emod + phim;
    } else {
        exponent = ecap;
    }
    return powmod(b_global, exponent, m);
}

int main() {
    ll b, i, n;
    while (scanf("%lld", &b) == 1 && b != 0) {
        scanf("%lld %lld", &i, &n);
        b_global = b;
        rawcap[0] = 1;
        for (int x = 1; x <= (int)i; x++) {
            rawcap[x] = powcap(b, rawcap[x - 1], CAP);
        }
        ll m = 1;
        for (int k = 0; k < n; k++) m *= 10;
        ll ans = compute((int)i, m);
        printf("%0*lld\n", (int)n, ans);
    }
    return 0;
}
