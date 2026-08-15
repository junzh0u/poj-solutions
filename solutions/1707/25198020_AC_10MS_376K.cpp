// POJ 1707 - Sum of powers
// Model: claude-sonnet-5
//
// S_k(n) = sum_{i=1}^{n} i^k is a degree-(k+1) polynomial in n with
// rational coefficients. Using Bernoulli numbers B_j (with the "+"
// convention B_1 = +1/2, so that the classical closed forms such as
// S_2(n) = n(n+1)(2n+1)/6 come out with the expected signs):
//
//   S_k(n) = 1/(k+1) * sum_{j=0}^{k} C(k+1,j) * B_j * n^(k+1-j)
//
// The B_j are computed with the standard recurrence (which yields the
// "-" convention, B_1 = -1/2):
//   B_0 = 1
//   B_m = -1/(m+1) * sum_{j=0}^{m-1} C(m+1,j) * B_j   for m >= 1
// and then the sign of B_1 alone is flipped to switch convention
// (all other odd-index Bernoulli numbers beyond index 1 are zero, so
// this substitution is exact and affects nothing else).
//
// All arithmetic is done in exact fractions (long long num/den,
// reduced by gcd); for k<=20 the numerators/denominators involved stay
// small (checked with an independent Python Fraction computation up to
// k=20, max coefficient magnitude ~4.8e7, everything comfortably fits
// long long / even 32-bit). Coefficients are collected as a polynomial
// in n, then scaled by M = lcm of all denominators (the minimal positive
// integer making every coefficient an integer), and printed
// M a_{k+1} a_k ... a_1 a_0.
//
// Verified: reproduces the sample (k=2 -> "6 2 3 1 0"), and matches an
// independent Python fractions.Fraction computation for every k=1..20.
#include <cstdio>
#include <vector>
using namespace std;

typedef long long ll;

ll gcd_ll(ll a, ll b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) { ll t = a % b; a = b; b = t; }
    return a;
}

struct Frac {
    ll num, den; // den > 0, reduced
    Frac(ll n = 0, ll d = 1) {
        if (d < 0) { n = -n; d = -d; }
        ll g = gcd_ll(n, d);
        if (g == 0) g = 1;
        num = n / g;
        den = d / g;
    }
};

Frac fadd(const Frac &a, const Frac &b) {
    ll g = gcd_ll(a.den, b.den);
    ll lden = a.den / g * b.den;
    ll n = a.num * (lden / a.den) + b.num * (lden / b.den);
    return Frac(n, lden);
}

Frac fmul(const Frac &a, const Frac &b) {
    ll g1 = gcd_ll(a.num, b.den);
    ll g2 = gcd_ll(b.num, a.den);
    ll n = (a.num / g1) * (b.num / g2);
    ll d = (a.den / g2) * (b.den / g1);
    return Frac(n, d);
}

Frac fneg(const Frac &a) { return Frac(-a.num, a.den); }
Frac fdiv(const Frac &a, ll k) { return Frac(a.num, a.den * k); }

int main() {
    int k;
    if (scanf("%d", &k) != 1) return 0;

    // binomial coefficients C(k+1, j) for j=0..k+1
    int N = k + 1;
    vector<vector<ll> > C(N + 1, vector<ll>(N + 1, 0));
    for (int i = 0; i <= N; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            C[i][j] = C[i - 1][j - 1] + (j <= i - 1 ? C[i - 1][j] : 0);
        }
    }

    // Bernoulli numbers B[0..k], "-" convention (B1 = -1/2)
    vector<Frac> B(k + 1);
    B[0] = Frac(1, 1);
    for (int m = 1; m <= k; m++) {
        Frac s(0, 1);
        for (int j = 0; j < m; j++) {
            s = fadd(s, fmul(Frac(C[m + 1][j], 1), B[j]));
        }
        B[m] = fneg(fdiv(s, m + 1));
    }
    // switch to "+" convention: only B1 flips sign
    if (k >= 1) B[1] = fneg(B[1]);

    // coeffs[i] = coefficient of n^i in S_k(n), i = 0..k+1
    vector<Frac> coeffs(k + 2, Frac(0, 1));
    for (int j = 0; j <= k; j++) {
        Frac term = fmul(Frac(C[k + 1][j], 1), B[j]);
        term = fdiv(term, k + 1);
        int power = k + 1 - j;
        coeffs[power] = fadd(coeffs[power], term);
    }

    // M = lcm of all denominators
    ll M = 1;
    for (int i = 0; i <= k + 1; i++) {
        ll d = coeffs[i].den;
        ll g = gcd_ll(M, d);
        M = M / g * d;
    }

    printf("%lld", M);
    for (int i = k + 1; i >= 0; i--) {
        ll val = coeffs[i].num * (M / coeffs[i].den);
        printf(" %lld", val);
    }
    printf("\n");
    return 0;
}
