// POJ 1811 - Prime Test
//
// Print the smallest prime factor of N, or "Prime" if N has none.
// T <= 20, 2 <= N < 2^54, 6000MS total / 4000MS per case.
//
// Approach:
//   1. Sieve the primes below 65536 once and trial-divide by them in
//      increasing order.  Scanning upward means the first divisor found IS
//      the smallest prime factor, so most inputs are answered right here; and
//      if some p*p exceeds N before any divides it, N is prime.
//   2. Whatever survives has every prime factor above 65536, hence at most
//      three of them (65536^4 > 2^54).  Deterministic Miller-Rabin on the
//      bases 2..37 -- exact for all n < 3.3e24 -- decides "Prime".
//   3. Otherwise split N with Pollard's rho in Brent's formulation and
//      recurse, taking the minimum of the prime factors found.  Brent batches
//      the gcd over 128 steps by multiplying the |x-y| differences together
//      mod N, replaying a batch one step at a time if its gcd comes back as N.
//
// On mulmod: N can reach 2^54, so a*b overflows 64 bits and C++98 on POJ has
// no __int128.  This uses Russian-peasant doubling built on an overflow-proof
// addmod (see below).  The usual faster alternative -- estimating the quotient
// a*b/m in long double -- was rejected because it cannot be validated on the
// machine this was written on, where long double is plain 64-bit; measurement
// says the doubling loop is ample anyway (the T=20 worst case, twenty
// semiprimes with both factors near 2^27, runs in 0.05s).
//
// Statement notes: the bound renders as "254" on the problem page, i.e. 2^54
// with the superscript lost.  N prime must print "Prime", not N itself.
#include <cstdio>

typedef unsigned long long ULL;

/* ---------- modular arithmetic ---------- */

/* Modular addition that cannot overflow: the naive "r += a; if (r >= m)
   r -= m" is only safe while 2m fits in 64 bits.  Comparing against m-b
   instead keeps every intermediate below m, so this is exact for every
   modulus up to 2^64-1, and it is what both mulmod and the rho iteration
   x -> x*x + c are built on.  Silent overflow there does not corrupt the
   answer -- the gcds still expose genuine factors of n -- it just turns the
   iteration into a map with a pathological cycle structure, which is how a
   single hard input once took 152 million rho steps instead of 60 thousand. */
static ULL addmod(ULL a, ULL b, ULL m) {
    return (a >= m - b) ? a - (m - b) : a + b;
}

static ULL mulmod(ULL a, ULL b, ULL m) {
    ULL r = 0;
    a %= m; b %= m;
    while (b) {
        if (b & 1ULL) r = addmod(r, a, m);
        a = addmod(a, a, m);
        b >>= 1;
    }
    return r;
}

static ULL powmod(ULL a, ULL e, ULL m) {
    ULL r = 1;
    a %= m;
    while (e) {
        if (e & 1ULL) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}

static ULL gcdULL(ULL a, ULL b) {
    while (b) { ULL t = a % b; a = b; b = t; }
    return a;
}

/* ---------- primality ---------- */

static bool witness(ULL n, ULL a, ULL d, int s) {
    ULL x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 1; i < s; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

/* Deterministic Miller-Rabin: these 12 bases decide primality for every
   n < 3.3 * 10^24, far past the 2^54 bound of this problem. */
static bool isPrime(ULL n) {
    static const ULL base[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == base[i]) return true;
        if (n % base[i] == 0) return false;
    }
    ULL d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++)
        if (!witness(n, base[i], d, s)) return false;
    return true;
}

/* ---------- Pollard-Brent factorisation ---------- */

static ULL rngState = 88172645463325252ULL;
static ULL nextRand() {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 7;
    rngState ^= rngState << 17;
    return rngState;
}

/* Brent's cycle finding with the gcd batched over 128 steps: the |x-y|
   differences are multiplied together mod n and a single gcd is taken per
   batch, trading ~128 gcds for ~128 mulmods plus one.  If a batch's gcd comes
   back as n the batch is replayed one step at a time. */
static ULL pollard(ULL n) {
    if ((n & 1ULL) == 0) return 2;
    if (n % 3 == 0) return 3;
    const ULL BATCH = 128;
    for (;;) {
        ULL c = nextRand() % (n - 1) + 1;
        ULL y = nextRand() % n;
        ULL x = y, ys = y, q = 1, d = 1, r = 1;
        do {
            x = y;
            for (ULL i = 0; i < r; i++) y = addmod(mulmod(y, y, n), c, n);
            ULL k = 0;
            while (k < r && d == 1) {
                ys = y;
                ULL lim = (BATCH < r - k) ? BATCH : (r - k);
                for (ULL i = 0; i < lim; i++) {
                    y = addmod(mulmod(y, y, n), c, n);
                    ULL diff = (x > y) ? (x - y) : (y - x);
                    q = mulmod(q, diff, n);
                }
                d = gcdULL(q, n);
                k += lim;
            }
            r <<= 1;
        } while (d == 1);
        if (d == n) {
            /* the batch swallowed the factor (or hit a zero difference):
               walk the same stretch again taking a gcd every step */
            d = 1;
            do {
                ys = addmod(mulmod(ys, ys, n), c, n);
                ULL diff = (x > ys) ? (x - ys) : (ys - x);
                if (diff == 0) { d = n; break; }
                d = gcdULL(diff, n);
            } while (d == 1);
        }
        if (d != 1 && d != n) return d;
        /* degenerate polynomial: retry with a fresh c and start point */
    }
}

static ULL smallestFactor(ULL n) {   /* n composite, all factors > 65536 */
    if (isPrime(n)) return n;
    ULL d = pollard(n);
    ULL a = smallestFactor(d);
    ULL b = smallestFactor(n / d);
    return a < b ? a : b;
}

/* ---------- small primes ---------- */

static const int LIM = 65536;              /* 2^16; 65536^2 > 2^32 */
static bool composite[LIM + 1];
static int primes[7000], numPrimes = 0;

static void sieve() {
    for (int i = 2; i <= LIM; i++) {
        if (!composite[i]) {
            primes[numPrimes++] = i;
            for (long long j = (long long)i * i; j <= LIM; j += i)
                composite[(int)j] = true;
        }
    }
}

/* ---------- I/O (no %llu: POJ's runtime is unreliable with it) ---------- */

static ULL readULL() {
    char buf[32];
    if (scanf("%31s", buf) != 1) return 0;
    ULL v = 0;
    for (int i = 0; buf[i]; i++) v = v * 10ULL + (ULL)(buf[i] - '0');
    return v;
}

static void printULL(ULL v) {
    char buf[32];
    int len = 0;
    if (v == 0) buf[len++] = '0';
    while (v > 0) { buf[len++] = (char)('0' + (int)(v % 10ULL)); v /= 10ULL; }
    while (len > 0) putchar(buf[--len]);
    putchar('\n');
}

int main() {
    sieve();
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        ULL n = readULL();
        if (n < 2) { printf("Prime\n"); continue; }

        /* Scanning primes upward, the first divisor found is by construction
           the smallest prime factor. */
        ULL found = 0;
        bool certainlyPrime = false;
        for (int i = 0; i < numPrimes; i++) {
            ULL p = (ULL)primes[i];
            if (p * p > n) { certainlyPrime = true; break; }
            if (n % p == 0) { found = p; break; }
        }
        if (certainlyPrime) { printf("Prime\n"); continue; }
        if (found) { printULL(found); continue; }

        /* Every prime factor of n now exceeds 65536, so n is either prime or
           a product of at most three such primes (65536^4 > 2^54). */
        if (isPrime(n)) { printf("Prime\n"); continue; }
        printULL(smallestFactor(n));
    }
    return 0;
}
