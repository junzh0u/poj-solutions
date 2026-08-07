// POJ 3904 - Sky Code
// Model: claude-opus-5
//
// Count 4-element subsets of the N given stars whose four ID numbers have
// gcd 1.  Stars are distinct objects even when their IDs repeat: the discuss
// board settles this ("must the numbers in the chosen subset be distinct?" ->
// "no"), and the counting below treats every star as its own item, so a value
// occurring k times contributes k separately to every divisor class.
//
// Mobius inversion.  Let cnt[d] be the number of stars whose ID is a multiple
// of d.  The number of 4-subsets with gcd exactly divisible by d is C(cnt[d],4),
// so the answer is  sum_{d>=1} mu(d) * C(cnt[d], 4),  and only squarefree d
// contribute.  C(10000,4) ~ 4.2e14 needs 64 bits; printing goes through
// std::cout so no %lld is required from POJ's compilers.
//
// Two ways to get cnt[] are used, whichever is cheaper for the case at hand,
// because the input holds many test cases (read to EOF) and the time limit is
// tight:
//   * harmonic sweep over every squarefree d <= maxv  (~6e4 ops, independent
//     of n) -- best for a large case;
//   * enumerate the squarefree divisors of each distinct value (<= 32 of them,
//     since 2*3*5*7*11*13 > 10000) -- cost proportional to the case size, so a
//     stream of tiny cases costs almost nothing.

#include <cstdio>
#include <cstring>
#include <iostream>

const int MAXV = 10000;

static int mu[MAXV + 1];
static int spf[MAXV + 1];
static int primes[1300];
static int pc;

static int occ[MAXV + 1];
static int cnt[MAXV + 1];
static int distinctVals[MAXV + 1];
static int touched[MAXV + 1];

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return false;
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x;
    return true;
}

static void sieve() {
    mu[1] = 1;
    spf[1] = 1;
    for (int i = 2; i <= MAXV; ++i) {
        if (spf[i] == 0) { spf[i] = i; primes[pc++] = i; mu[i] = -1; }
        for (int j = 0; j < pc; ++j) {
            int p = primes[j];
            if (p > spf[i] || (long)p * i > MAXV) break;
            int t = p * i;
            spf[t] = p;
            mu[t] = (p == spf[i]) ? 0 : -mu[i];
        }
    }
}

static long long choose4(long long c) {
    if (c < 4) return 0;
    return c * (c - 1) * (c - 2) * (c - 3) / 24;
}

int main() {
    sieve();
    int n;
    while (readInt(n)) {
        int maxv = 1;
        int nd = 0;
        for (int i = 0; i < n; ++i) {
            int v;
            if (!readInt(v)) v = 1;
            if (occ[v]++ == 0) distinctVals[nd++] = v;
            if (v > maxv) maxv = v;
        }

        long long ans = 0;
        if (n >= 4) {
            /* Costs: harmonic sweep ~ 0.61*maxv*ln(maxv); divisor enumeration
               ~ 32 per distinct value.  Pick the smaller. */
            if (32 * nd < 61 * maxv / 10) {
                int nt = 0;
                for (int i = 0; i < nd; ++i) {
                    int v = distinctVals[i];
                    int w = occ[v];
                    int div[32];
                    int ndiv = 1;
                    div[0] = 1;
                    int x = v;
                    while (x > 1) {
                        int p = spf[x];
                        while (x % p == 0) x /= p;
                        for (int k = 0, e = ndiv; k < e; ++k)
                            div[ndiv++] = div[k] * p;
                    }
                    for (int k = 0; k < ndiv; ++k) {
                        int d = div[k];
                        if (cnt[d] == 0) touched[nt++] = d;
                        cnt[d] += w;
                    }
                }
                for (int i = 0; i < nt; ++i) {
                    int d = touched[i];
                    if (mu[d]) ans += (long long)mu[d] * choose4(cnt[d]);
                    cnt[d] = 0;
                }
            } else {
                for (int d = 1; d <= maxv; ++d) {
                    if (mu[d] == 0) continue;
                    long long c = 0;
                    for (int m = d; m <= maxv; m += d) c += occ[m];
                    if (c >= 4) ans += (long long)mu[d] * choose4(c);
                }
            }
        }
        std::cout << ans << "\n";

        for (int i = 0; i < nd; ++i) occ[distinctVals[i]] = 0;
    }
    return 0;
}
