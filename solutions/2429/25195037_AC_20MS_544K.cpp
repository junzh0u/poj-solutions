// POJ 2429 - GCD & LCM Inverse
// Model: claude-opus-5
//
// Given g = gcd(a,b) and l = lcm(a,b) (both < 2^63), recover the pair with the
// smallest a+b.  Write a = g*x, b = g*y; then gcd(x,y) = 1 and x*y = n = l/g.
// Coprimality means every prime power of n goes wholly to x or wholly to y, so
// the candidates are exactly the 2^k subsets of n's prime powers (k <= 15,
// because the product of the first 16 primes, 2*3*...*53, already exceeds
// 2^63).  a+b = g*(x + n/x) is minimised by taking the largest divisor
// x <= sqrt(n), so enumerate the subsets and keep the biggest product with
// x <= n/x.
//
// Factoring n: trial division by primes up to 1e5, then deterministic
// Miller-Rabin (smallest prime-base prefix that is proven for the magnitude,
// up to all 12 bases for 64-bit) plus Brent's variant of Pollard's rho on what
// is left.  64-bit modular multiplication is done by
// shift-and-add (n < 2^63 keeps a<<1 inside 64 bits) because POJ's compilers
// have no 128-bit integer type.  The rho driver is iterative and uses a fixed
// sequence of constants - the discuss board reports both recursion (TLE) and
// srand(time(NULL)) (RE) biting people here.
//
// Ambiguity: the statement never says what to print when g does not divide l;
// such input is treated as having no solution and skipped.  It also does not
// call out g == l, which is legal and yields a == b == g (the board flags it).

#include <iostream>

typedef unsigned long long ull;

static ull mulmod(ull a, ull b, ull m) {
    ull r = 0;
    if (a >= m) a %= m;
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

static ull powmod(ull a, ull e, ull m) {
    ull r = 1 % m;
    a %= m;
    while (e) {
        if (e & 1ULL) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}

static ull gcd_ull(ull a, ull b) {
    while (b) {
        ull t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static const int SMALL[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

static bool is_prime(ull n) {
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        ull p = (ull)SMALL[i];
        if (n % p == 0) return n == p;
    }
    /* smallest base set that is deterministic for this magnitude */
    int nb = 12;
    if (n < 3215031751ULL) nb = 4;
    else if (n < 3474749660383ULL) nb = 6;
    else if (n < 341550071728321ULL) nb = 7;
    else if (n < 3825123056546413051ULL) nb = 9;
    ull d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; s++; }
    for (int i = 0; i < nb; i++) {
        ull x = powmod((ull)SMALL[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int j = 1; j < s; j++) {
            x = mulmod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Brent's cycle-finding variant of Pollard's rho; returns a non-trivial
   divisor of the odd composite n. */
static ull pollard(ull n) {
    if ((n & 1ULL) == 0) return 2;
    /* STEP advances the sequence y <- y*y + c (mod n).  The reduction is a
       conditional subtract, not a %, because a 64-bit division costs about as
       much as the whole multiply on the judge's compiler. */
#define STEP(y) do { (y) = mulmod((y), (y), n) + c; if ((y) >= n) (y) -= n; } while (0)
    for (ull c = 1; ; c++) {
        ull x = 2, y = 2, ys = 2, q = 1, d = 1;
        int r = 1;
        const int m = 128;
        do {
            x = y;
            for (int i = 0; i < r; i++) STEP(y);
            int k = 0;
            while (k < r && d == 1) {
                ys = y;
                int lim = (m < r - k) ? m : r - k;
                for (int i = 0; i < lim; i++) {
                    STEP(y);
                    ull diff = (x > y) ? (x - y) : (y - x);
                    if (diff) q = mulmod(q, diff, n);
                }
                d = gcd_ull(q, n);
                k += lim;
            }
            r <<= 1;
        } while (d == 1);
        if (d == n) {
            d = 1;
            y = ys;
            while (d == 1) {
                STEP(y);
                ull diff = (x > y) ? (x - y) : (y - x);
                if (diff == 0) break;
                d = gcd_ull(diff, n);
            }
        }
        if (d != 1 && d != n) return d;
    }
#undef STEP
}

/* primes below SIEVE_N, for the trial-division pass */
#define SIEVE_N 100000
static bool composite[SIEVE_N];
static int primes[10000];
static int nprimes = 0;

static void build_sieve() {
    for (int i = 2; i < SIEVE_N; i++) {
        if (!composite[i]) {
            primes[nprimes++] = i;
            for (long long j = (long long)i * i; j < SIEVE_N; j += i)
                composite[(int)j] = true;
        }
    }
}

static ull fac_p[64];
static int fac_e[64];
static int nfac;

static void add_factor(ull p) {
    for (int i = 0; i < nfac; i++) {
        if (fac_p[i] == p) { fac_e[i]++; return; }
    }
    fac_p[nfac] = p;
    fac_e[nfac] = 1;
    nfac++;
}

static ull stack_[128];

static void factorize(ull n) {
    nfac = 0;
    for (int i = 0; i < nprimes; i++) {
        ull p = (ull)primes[i];
        if (p * p > n) break;
        while (n % p == 0) { add_factor(p); n /= p; }
    }
    if (n == 1) return;
    int top = 0;
    stack_[top++] = n;
    while (top) {
        ull v = stack_[--top];
        if (v == 1) continue;
        if (is_prime(v)) { add_factor(v); continue; }
        ull d = pollard(v);
        stack_[top++] = d;
        stack_[top++] = v / d;
    }
}

static ull prod[1 << 15];

int main() {
    std::ios::sync_with_stdio(false);
    build_sieve();
    ull g, l;
    while (std::cin >> g >> l) {
        if (g == 0 || l % g != 0) continue;
        ull n = l / g;
        factorize(n);

        ull pw[16];
        for (int i = 0; i < nfac; i++) {
            ull v = 1;
            for (int j = 0; j < fac_e[i]; j++) v *= fac_p[i];
            pw[i] = v;
        }

        int k = nfac;
        int total = 1 << k;
        prod[0] = 1;
        ull best = 1;
        for (int mask = 1; mask < total; mask++) {
            int low = 0;
            while (!((mask >> low) & 1)) low++;
            ull d = prod[mask ^ (1 << low)] * pw[low];
            prod[mask] = d;
            if (d <= n / d && d > best) best = d;
        }

        ull a = best * g;
        ull b = (n / best) * g;
        std::cout << a << " " << b << "\n";
    }
    return 0;
}
