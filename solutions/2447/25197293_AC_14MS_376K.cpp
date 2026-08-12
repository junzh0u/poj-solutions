// POJ 2447 - RSA
// Model: gpt-5.6-terra
// Factor the semiprime modulus with Miller-Rabin and Pollard-Rho, derive the
// private exponent with the extended Euclidean algorithm, then decrypt by
// binary modular exponentiation.  N is below 2^62, but products are not, so
// every modular product uses __int128 and this is submitted as G++.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef unsigned long long ull;
typedef __int128 i128;

ull mulmod(ull a, ull b, ull mod) {
    return (ull)((i128)a * b % mod);
}

ull powmod(ull a, ull e, ull mod) {
    ull r = 1;
    while (e) {
        if (e & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return r;
}

ull gcd64(ull a, ull b) {
    while (b) {
        ull t = a % b;
        a = b;
        b = t;
    }
    return a;
}

bool isPrime(ull n) {
    static const int small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    int i;
    if (n < 2) return false;
    for (i = 0; i < (int)(sizeof(small) / sizeof(small[0])); ++i) {
        if (n == (ull)small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    ull d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const ull bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                450775ULL, 9780504ULL, 1795265022ULL};
    for (i = 0; i < (int)(sizeof(bases) / sizeof(bases[0])); ++i) {
        ull a = bases[i] % n;
        ull x, j;
        if (a == 0) continue;
        x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        for (j = 1; j < s; ++j) {
            x = mulmod(x, x, n);
            if (x == n - 1) break;
        }
        if (j == s) return false;
    }
    return true;
}

ull rngState = 88172645463325252ULL;
ull nextRand() {
    rngState ^= rngState << 7;
    rngState ^= rngState >> 9;
    return rngState;
}

ull rho(ull n) {
    if ((n & 1) == 0) return 2;
    for (;;) {
        ull y = nextRand() % (n - 1) + 1;
        ull c = nextRand() % (n - 1) + 1;
        ull x = 0, ys = 0, g = 1, r = 1, q = 1;
        while (g == 1) {
            ull i, k = 0;
            x = y;
            for (i = 0; i < r; ++i) y = (mulmod(y, y, n) + c) % n;
            while (k < r && g == 1) {
                ull lim = min(128ULL, r - k);
                ys = y;
                q = 1;
                for (i = 0; i < lim; ++i) {
                    ull diff;
                    y = (mulmod(y, y, n) + c) % n;
                    diff = x > y ? x - y : y - x;
                    q = mulmod(q, diff, n);
                }
                g = gcd64(q, n);
                k += lim;
            }
            r <<= 1;
        }
        if (g == n) {
            do {
                ys = (mulmod(ys, ys, n) + c) % n;
                g = gcd64(x > ys ? x - ys : ys - x, n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

void factor(ull n, vector<ull>& f) {
    if (isPrime(n)) { f.push_back(n); return; }
    ull d = rho(n);
    factor(d, f);
    factor(n / d, f);
}

ull inverse(ull a, ull mod) {
    i128 oldr = a, r = mod, oldx = 1, x = 0;
    while (r != 0) {
        i128 q = oldr / r;
        i128 nr = oldr - q * r;
        i128 nx = oldx - q * x;
        oldr = r; r = nr;
        oldx = x; x = nx;
    }
    oldx %= (i128)mod;
    if (oldx < 0) oldx += mod;
    return (ull)oldx;
}

int main() {
    ull c, e, n;
    while (cin >> c >> e >> n) {
        vector<ull> f;
        factor(n, f);
        sort(f.begin(), f.end());
        ull phi = (f[0] - 1) * (f[1] - 1);
        ull d = inverse(e, phi);
        cout << powmod(c, d, n) << '\n';
    }
    return 0;
}
