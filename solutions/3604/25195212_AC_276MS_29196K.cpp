// POJ 3604 - Professor Ben
// Model: claude-opus-5
//
// For N with factorization prod p_i^e_i, the answer is
//     A(N) = sum_{d | N} tau(d)^3
// where tau is the divisor-count function.  tau^3 is multiplicative, hence so
// is its divisor sum A.  For a prime power,
//     A(p^e) = sum_{k=0..e} (k+1)^3 = ( (e+1)(e+2)/2 )^2
// by the classic sum-of-cubes identity, so
//     A(N) = prod_i ( (e_i+1)(e_i+2)/2 )^2.
// Max over N <= 5e6 is 289340100 (at N = 4324320 = 2^5*3^3*5*7*11*13), which
// fits in 32 bits unsigned; no 64-bit arithmetic is needed anywhere.
//
// With Q up to 5e5 and N up to 5e6, A is precomputed for the whole range by a
// linear (Euler) sieve that carries the multiplicative value directly: cnt[n]
// holds the exponent of n's smallest prime factor, so when the sieve extends
// i by its smallest prime p it can divide out the old prime-power factor and
// multiply in the new one.  Memory: 20MB for A + 5MB for cnt, inside 64MB.
// Input is read with a hand-rolled fread scanner and output batched through
// one buffer, since 5e5 scanf/printf pairs are the other plausible TLE.

#include <cstdio>

const int MAXN = 5000000;
const int MAXP = 350000; /* pi(5e6) = 348513 */

static unsigned int A[MAXN + 1];
static unsigned char cnt[MAXN + 1];
static int primes[MAXP];

/* A(p^e) = ((e+1)(e+2)/2)^2 */
static inline unsigned int powVal(int e) {
    unsigned int t = (unsigned int)(e + 1) * (unsigned int)(e + 2) / 2u;
    return t * t;
}

static void sieve() {
    int pc = 0;
    A[1] = 1;
    for (int i = 2; i <= MAXN; ++i) {
        if (A[i] == 0u) { /* untouched => prime */
            primes[pc++] = i;
            A[i] = 9u; /* powVal(1) */
            cnt[i] = 1;
        }
        for (int j = 0; j < pc; ++j) {
            int p = primes[j];
            if (p > MAXN / i) break;
            int k = p * i;
            if (i % p == 0) {
                int c = cnt[i];
                cnt[k] = (unsigned char)(c + 1);
                A[k] = A[i] / powVal(c) * powVal(c + 1);
                break;
            }
            cnt[k] = 1;
            A[k] = A[i] * 9u;
        }
    }
}

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static inline bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x;
    return true;
}

static const int OBUF = 1 << 20;
static char obuf[OBUF + 64];
static int opos = 0;

static inline void writeUInt(unsigned int v) {
    if (opos >= OBUF) { fwrite(obuf, 1, (size_t)opos, stdout); opos = 0; }
    char tmp[12];
    int n = 0;
    do { tmp[n++] = (char)('0' + (int)(v % 10u)); v /= 10u; } while (v);
    while (n) obuf[opos++] = tmp[--n];
    obuf[opos++] = '\n';
}

int main() {
    sieve();
    int q;
    if (!readInt(q)) return 0;
    for (int i = 0; i < q; ++i) {
        int n;
        if (!readInt(n)) break;
        writeUInt((n >= 1 && n <= MAXN) ? A[n] : 0u);
    }
    fwrite(obuf, 1, (size_t)opos, stdout);
    return 0;
}
