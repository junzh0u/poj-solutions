// POJ 2635 - The Embarrassed Cryptographer
//
// K is a product of two primes with up to 101 decimal digits (the statement's
// "10100" is 10^100; likewise "106" for L is 10^6 -- the page loses exponent
// superscripts). K therefore never fits any built-in integer type and stays a
// decimal string. Since K = p*q with both factors prime, its only divisors are
// p and q, so the FIRST prime below L that divides K is exactly the smaller
// factor: sieve every prime under 10^6 once, then for each test walk them in
// increasing order and print "BAD p" at the first hit, "GOOD" if none divides.
//
// K mod p comes straight off the digits by Horner. Digits are packed three at a
// time into base-1000 limbs, which is the largest power of ten that keeps the
// whole chain in 32 bits (acc < p < 10^6, so acc*1000 + limb <= 999998999 <
// 2^31); base 10^4 would need 64-bit arithmetic, and POJ's judges are 32-bit,
// where a 64-bit divide becomes a slow __umoddi3 library call. That is also why
// the remainder is taken by Barrett reduction rather than `%`: magic[i] =
// floor(2^32 / p) is precomputed alongside the sieve, and q = (x*m) >> 32 is
// within one of floor(x/p), so a single conditional subtraction fixes it. The
// inner loop is then a 32x32->64 multiply plus a multiply instead of a divide.
//
// Worst case is ~34 limbs x 78498 primes x 20 cases, and the whole spec-maximum
// input runs in 0.15s locally.
//
// Note L is exclusive: a factor equal to L is not "strictly less than L", so the
// prime loop breaks on p >= L. L = 2 always yields GOOD.

#include <cstdio>
#include <cstring>

const int MAXP = 1000000;

static bool comp[MAXP];
static unsigned int primes[80000];
static unsigned int magic[80000];
static int np = 0;

int main() {
    for (int i = 2; i < MAXP; ++i) {
        if (!comp[i]) {
            primes[np] = (unsigned int)i;
            magic[np] = (unsigned int)(0x100000000ULL / (unsigned long long)i);
            ++np;
            if (i <= (MAXP - 1) / i)
                for (int j = i * i; j < MAXP; j += i) comp[j] = true;
        }
    }

    char buf[160];
    int L;
    unsigned int limbs[64];
    while (scanf("%s %d", buf, &L) == 2) {
        if (L == 0 && buf[0] == '0' && buf[1] == '\0') break;

        int len = (int)strlen(buf);
        int nl = 0, pos = 0, r = len % 3;
        if (r) {
            unsigned int v = 0;
            for (int i = 0; i < r; ++i) v = v * 10u + (unsigned int)(buf[pos++] - '0');
            limbs[nl++] = v;
        }
        while (pos < len) {
            limbs[nl++] = (unsigned int)((buf[pos] - '0') * 100 + (buf[pos + 1] - '0') * 10 + (buf[pos + 2] - '0'));
            pos += 3;
        }

        unsigned int UL = (unsigned int)L;
        int found = 0;
        for (int i = 0; i < np; ++i) {
            unsigned int p = primes[i];
            if (p >= UL) break;
            unsigned int m = magic[i];
            unsigned int acc = 0;
            for (int j = 0; j < nl; ++j) {
                unsigned int x = acc * 1000u + limbs[j];
                unsigned int q = (unsigned int)(((unsigned long long)x * m) >> 32);
                acc = x - q * p;
                if (acc >= p) acc -= p;
            }
            if (acc == 0) {
                printf("BAD %u\n", p);
                found = 1;
                break;
            }
        }
        if (!found) printf("GOOD\n");
    }
    return 0;
}
