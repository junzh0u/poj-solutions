// POJ 2191 - Mersenne Composite Numbers
// Model: claude-opus-5
//
// The Mersenne number M = 2^p - 1 is less than 2^k exactly when p <= k, so the
// candidate exponents are the primes up to k, capped at 61 -- the largest prime
// keeping 2^p - 1 inside 64 bits.  All 61 candidates are resolved once at start
// up and the answer for a given k is a prefix of that list.
//
// Deterministic Miller-Rabin (bases 2,3,5,7,11,13,17,19,23,29,31,37 are proven
// sufficient below 3.3e24) decides whether M is prime; the composite ones are
// factored by trial division restricted to candidates of the form q = 2*j*p + 1.
// That form is forced: a prime q | 2^p - 1 makes 2 have multiplicative order p
// mod q, so p | q-1, and q is odd, hence 2p | q-1.  Re-testing the cofactor for
// primality after each hit stops the scan as soon as what is left is prime, so
// the longest scan in the whole domain is 1525 candidates (p = 59).
//
// mulmod is done by binary doubling rather than __int128, which C++98 lacks,
// and output goes through std::cout because POJ's compilers do not handle %lld.
//
// Ambiguity: "less than 2^k" makes p = k eligible (2^k - 1 < 2^k), so the
// comparison is p <= k.  The official sample (k = 31, exponents 11/23/29) cannot
// separate that from a strict p < k, since 31 itself is a Mersenne prime.
// The statement says the input is a single number, but the discuss board's
// accepted codes read to EOF; doing the same is harmless either way.

#include <iostream>
#include <sstream>
#include <string>

typedef unsigned long long u64;

static u64 mulmod(u64 a, u64 b, u64 m) {
    u64 r = 0;
    a %= m;
    while (b) {
        if (b & 1ULL) {
            r += a;
            if (r >= m) r -= m;
        }
        a <<= 1;
        if (a >= m) a -= m;
        b >>= 1;
    }
    return r;
}

static u64 powmod(u64 a, u64 e, u64 m) {
    u64 r = 1;
    a %= m;
    while (e) {
        if (e & 1ULL) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool isPrime(u64 n) {
    if (n < 2) return false;
    static const u64 base[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int i = 0; i < 12; ++i) {
        if (n == base[i]) return true;
        if (n % base[i] == 0) return false;
    }
    u64 d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    for (int i = 0; i < 12; ++i) {
        u64 x = powmod(base[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool witness = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) { witness = false; break; }
        }
        if (witness) return false;
    }
    return true;
}

int main() {
    // Build the whole answer once (only 61 candidate exponents exist), so an
    // input holding many values costs nothing extra.
    int expo[16];
    std::string line[16];
    int total = 0;

    for (int p = 2; p <= 61; ++p) {
        if (!isPrime((u64)p)) continue;
        u64 m = ((u64)1 << p) - 1;
        if (isPrime(m)) continue;

        u64 fac[64];
        int nf = 0;
        u64 rest = m;
        u64 step = 2ULL * (u64)p;
        for (u64 q = step + 1; q * q <= rest; q += step) {
            if (rest % q != 0) continue;
            while (rest % q == 0) {
                fac[nf++] = q;
                rest /= q;
            }
            if (rest > 1 && isPrime(rest)) break;
        }
        if (rest > 1) fac[nf++] = rest;

        std::ostringstream os;
        for (int i = 0; i < nf; ++i) {
            if (i) os << " * ";
            os << fac[i];
        }
        os << " = " << m << " = ( 2 ^ " << p << " ) - 1";
        expo[total] = p;
        line[total] = os.str();
        ++total;
    }

    int k;
    while (std::cin >> k) {
        for (int i = 0; i < total && expo[i] <= k; ++i)
            std::cout << line[i] << "\n";
    }
    return 0;
}
