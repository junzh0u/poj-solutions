// POJ 3696 - The Luckiest number
// Model: claude-opus-5
//
// The number of x eights is N(x) = 8 * (10^x - 1) / 9, so L | N(x) iff
// 9L | 8 * (10^x - 1) iff m | 10^x - 1 with m = 9L / gcd(8, 9L) = 9L / gcd(8, L)
// (9 is odd).  Hence the answer is the multiplicative order of 10 modulo m,
// which exists iff gcd(m, 10) = 1.  Since m = 9k with k = L / gcd(L, 8), that
// means k must be free of 2 and 5 -- equivalently 16 does not divide L and 5
// does not divide L, which matches N having 2-adic valuation exactly 3 and no
// factor 5.  Otherwise print 0.
//
// The order is found the usual way: factor m (only k <= 2e9 needs trial
// division, the 3^2 is known), get phi(m), then for every prime q | phi(m)
// divide the exponent by q while 10^(x/q) == 1 (mod m).  The primes of phi(m)
// come from factoring each p-1 separately, each of which is at most 2e9, so a
// sieve up to sqrt(2.1e9) is enough everywhere.
//
// Overflow: m can be 9 * 2e9 = 1.8e10 < 2^35, so a plain a*b % m would need 70
// bits.  mulmod splits a into an 17-bit high half and an 18-bit low half, which
// keeps every intermediate under 2^54.  No __int128 (not C++98, and POJ is
// 32-bit anyway).
//
// Input framing (the discuss board's recurring WA): multiple test cases, one L
// per line, terminated by a line containing 0; case numbers start at 1.
// Output uses iostreams so the 64-bit answer does not depend on whether the
// judge's printf understands %lld.

#include <iostream>
#include <vector>
using namespace std;

typedef unsigned long long ull;
typedef long long ll;

static vector<int> primes;

static void sieve(int n) {
    vector<char> comp(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        if (!comp[i]) {
            primes.push_back(i);
            for (ll j = (ll)i * i; j <= n; j += i) comp[(int)j] = 1;
        }
    }
}

/* a, b < m < 2^35 */
static ull mulmod(ull a, ull b, ull m) {
    ull hi = a >> 18, lo = a & 0x3FFFFULL;
    ull r = (hi * b) % m;
    r = (r << 18) % m;
    r = (r + lo * b) % m;
    return r;
}

static ull powmod(ull a, ull e, ull m) {
    ull r = 1 % m;
    a %= m;
    while (e) {
        if (e & 1) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}

/* factor n <= ~2.1e9 into (prime, exponent) pairs appended to out */
static void factor(ull n, vector<pair<ull, int> >& out) {
    for (size_t i = 0; i < primes.size(); ++i) {
        ull p = (ull)primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            out.push_back(make_pair(p, e));
        }
    }
    if (n > 1) out.push_back(make_pair(n, 1));
}

int main() {
    sieve(46400); /* > sqrt(2.1e9) */
    ll L;
    int cs = 0;
    while ((cin >> L) && L != 0) {
        ++cs;
        ll g;
        { ll a = 8, b = L; while (b) { ll t = a % b; a = b; b = t; } g = a; }
        ll k = L / g;              /* m = 9 * k */
        ull m = 9ULL * (ull)k;
        ll ans = 0;
        if (k % 2 != 0 && k % 5 != 0) {
            vector<pair<ull, int> > fm;
            factor((ull)k, fm);
            bool sawThree = false;
            for (size_t i = 0; i < fm.size(); ++i)
                if (fm[i].first == 3) { fm[i].second += 2; sawThree = true; }
            if (!sawThree) fm.push_back(make_pair((ull)3, 2));

            ull phi = 1;
            vector<ull> qs;
            for (size_t i = 0; i < fm.size(); ++i) {
                ull p = fm[i].first;
                int e = fm[i].second;
                for (int j = 1; j < e; ++j) phi *= p;
                phi *= (p - 1);
                if (e >= 2) qs.push_back(p);
                vector<pair<ull, int> > fp;
                factor(p - 1, fp);
                for (size_t j = 0; j < fp.size(); ++j) qs.push_back(fp[j].first);
            }

            ull x = phi;
            for (size_t i = 0; i < qs.size(); ++i) {
                ull q = qs[i];
                while (x % q == 0 && powmod(10, x / q, m) == 1) x /= q;
            }
            ans = (ll)x;
        }
        cout << "Case " << cs << ": " << ans << "\n";
    }
    return 0;
}
