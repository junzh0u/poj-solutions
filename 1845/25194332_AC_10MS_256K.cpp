// POJ 1845 - Sumdiv
//
// Sum of all divisors of A^B, mod 9901 (0 <= A,B <= 5*10^7).
//
// Factorize A into primes p_i^e_i, so A^B = product p_i^(e_i*B), and the
// divisor sum is the product over i of the geometric series
// 1 + p_i + p_i^2 + ... + p_i^(e_i*B) (mod 9901). 9901 happens to be prime,
// but the classic trap is computing that series via the closed form
// (p^(n+1)-1)/(p-1) with a modular inverse: it breaks whenever p == 1
// (mod 9901), since numerator and denominator both vanish, needing an
// awkward special case. geoSum() instead sums the series by recursive
// halving (split into two halves of a power-of-two-ish size and factor out
// p^(half+1) from the second half) so it never divides at all, handling
// p == 1 (mod 9901) and p == 0 (mod 9901, i.e. p == 9901 itself) for free.
//
// A == 0 is handled explicitly as 0 (0^B has no well-defined divisor sum
// for B > 0, and by the convention used by other accepted solutions to this
// problem it is 0 even at B == 0). B == 0 gives A^0 = 1, sum of divisors 1.
#include <cstdio>

typedef long long ll;
const ll MOD = 9901;

ll modpow(ll base, ll exp) {
    base %= MOD;
    if (base < 0) base += MOD;
    ll result = 1 % MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

// returns (1 + p + p^2 + ... + p^n) mod MOD, n >= 0, without division
ll geoSum(ll p, ll n) {
    if (n == 0) return 1 % MOD;
    if (n % 2 == 1) {
        ll half = geoSum(p, n / 2);
        return half * (1 + modpow(p, n / 2 + 1)) % MOD;
    } else {
        return (geoSum(p, n - 1) + modpow(p, n)) % MOD;
    }
}

int main() {
    ll A, B;
    while (scanf("%lld %lld", &A, &B) == 2) {
        if (A == 0) {
            printf("0\n");
            continue;
        }
        if (B == 0) {
            printf("1\n");
            continue;
        }

        ll ans = 1 % MOD;
        ll a = A;
        for (ll p = 2; p * p <= a; ++p) {
            if (a % p == 0) {
                ll e = 0;
                while (a % p == 0) { a /= p; ++e; }
                ans = ans * geoSum(p, e * B) % MOD;
            }
        }
        if (a > 1) {
            ans = ans * geoSum(a, B) % MOD;
        }

        printf("%lld\n", ans);
    }
    return 0;
}
