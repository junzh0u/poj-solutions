// POJ 3209 - From Pythagoras to ...
// Model: claude-sonnet-5
// Approach: x^2+y^2=n has an integer solution (n can be negative per the
// discuss board) iff n<0 is impossible (NO), n==0 is trivial (YES), and for
// n>0 the classical sum-of-two-squares theorem applies: n is a sum of two
// squares iff every prime factor congruent to 3 (mod 4) occurs to an even
// power. n fits in signed 64-bit and can be a product of two ~large (up to
// ~3e9) primes, so full factorization needs Miller-Rabin + Pollard's rho
// (hence __int128 mulmod -> compiled as G++, not the old MSVC "4").
#include <cstdio>
#include <cstdlib>
#include <map>
using namespace std;

typedef unsigned long long u64;
typedef __int128 u128;

u64 gcd64(u64 a, u64 b) {
    while (b) { u64 t = a % b; a = b; b = t; }
    return a;
}

u64 mulmod(u64 a, u64 b, u64 mod) {
    return (u64)((u128)a * b % mod);
}

u64 powmod(u64 a, u64 d, u64 mod) {
    u64 r = 1 % mod;
    a %= mod;
    while (d) {
        if (d & 1) r = mulmod(r, a, mod);
        a = mulmod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static const u64 WITNESSES[] = {2,3,5,7,11,13,17,19,23,29,31,37};
static const int NUM_WITNESS = 12;

bool millerRabin(u64 n) {
    if (n < 2) return false;
    for (int i = 0; i < NUM_WITNESS; i++) {
        u64 p = WITNESSES[i];
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int r = 0;
    while ((d & 1) == 0) { d >>= 1; r++; }
    for (int i = 0; i < NUM_WITNESS; i++) {
        u64 a = WITNESSES[i];
        u64 x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int j = 0; j < r - 1; j++) {
            x = mulmod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

u64 pollardRho(u64 n) {
    if (n % 2 == 0) return 2;
    u64 x = rand() % n;
    u64 y = x;
    u64 c = rand() % n + 1;
    u64 d = 1;
    while (d == 1) {
        x = (mulmod(x, x, n) + c) % n;
        y = (mulmod(y, y, n) + c) % n;
        y = (mulmod(y, y, n) + c) % n;
        u64 diff = x > y ? x - y : y - x;
        if (diff == 0) { d = n; break; }
        d = gcd64(diff, n);
    }
    if (d == n) return pollardRho(n);
    return d;
}

void factor(u64 n, map<u64,int> &f) {
    if (n == 1) return;
    if (millerRabin(n)) { f[n]++; return; }
    u64 d = n;
    while (d == n) d = pollardRho(n);
    factor(d, f);
    factor(n / d, f);
}

int main() {
    srand(12345);
    int T;
    if (scanf("%d", &T) != 1) return 0;
    for (int tc = 0; tc < T; tc++) {
        long long n;
        scanf("%lld", &n);
        if (n < 0) {
            printf("NO\n");
            continue;
        }
        if (n == 0) {
            printf("YES\n");
            continue;
        }
        u64 m = (u64)n;
        // strip small primes first for speed
        map<u64,int> f;
        for (u64 p = 2; p * p <= m && p < 100000; p++) {
            while (m % p == 0) { f[p]++; m /= p; }
        }
        if (m > 1) factor(m, f);
        bool ok = true;
        for (map<u64,int>::iterator it = f.begin(); it != f.end(); ++it) {
            if (it->first % 4 == 3 && (it->second % 2) != 0) { ok = false; break; }
        }
        printf(ok ? "YES\n" : "NO\n");
    }
    return 0;
}
