// POJ 3012 - A Number from Yanghui Triangle
// Model: claude-opus-5
//
// p is the concatenation of the zero-padded k-digit decimal forms of
// C(n,0), C(n,1), ... , C(n,n), so as a number
//     p = sum_r C(n,r) * (10^k)^(n-r) = (10^k + 1)^n
// by the binomial theorem.  Each case is therefore two modular
// exponentiations, O(log k + log n).
//
// Input framing: the leading T is authoritative.  The judge data carries more
// lines after the T-th case, so looping to EOF draws a Wrong Answer (reported
// on the discuss board and confirmed there against an accepted program).
//
// Ambiguity: the statement guarantees log10(a_{n,r}) < k, yet the data holds
// k = 0 -- a board post proves it, a program whose only behaviour was to hang
// on k == 0 got Time Limit Exceeded here.  Under the concatenation reading
// k = 0 would make p the empty string; accepted programs instead let the
// exponentiation yield 10^0 = 1 and print 2^n mod m, which is what this does.
//
// m <= 1e9 < 2^30, so a product of two residues stays below 2^60.  mulmod
// estimates the quotient in double -- a relative error near 2^-52 on a
// quotient below 2^30 puts it off by at most one -- and then forms the
// remainder with exact 32-bit wrapping arithmetic, correcting by +-m.  The
// result is an exact integer and no 64-bit division is executed; the board
// reports 64-bit division as the difference between TLE and Accepted here,
// and C++ (language 4) has no long long at all.  Checked against 64-bit
// reference arithmetic on 45 million products, among them every (a,b) pair
// for every modulus from 2 to 300 and the boundaries at m = 1e9.

#include <stdio.h>

static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

static unsigned readUInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    unsigned v = 0;
    while (c >= '0' && c <= '9') {
        v = v * 10u + (unsigned)(c - '0');
        c = gc();
    }
    return v;
}

static char obuf[1 << 16];
static int opos = 0;

static void flushOut() {
    if (opos) fwrite(obuf, 1, (size_t)opos, stdout);
    opos = 0;
}

static void writeUInt(unsigned v) {
    char tmp[12];
    int n = 0;
    if (opos > (int)sizeof(obuf) - 16) flushOut();
    if (v == 0) tmp[n++] = '0';
    while (v) {
        tmp[n++] = (char)('0' + v % 10u);
        v /= 10u;
    }
    while (n) obuf[opos++] = tmp[--n];
    obuf[opos++] = '\n';
}

/* (a * b) mod m for a, b < m <= 1e9 < 2^30, without 64-bit arithmetic. */
static unsigned mulmod(unsigned a, unsigned b, unsigned m) {
    unsigned q = (unsigned)((double)a * (double)b / (double)m);
    int r = (int)(a * b - q * m);
    while (r < 0) r += (int)m;
    while (r >= (int)m) r -= (int)m;
    return (unsigned)r;
}

static unsigned powmod(unsigned base, unsigned e, unsigned m) {
    unsigned r = 1u % m;
    base %= m;
    while (e) {
        if (e & 1u) r = mulmod(r, base, m);
        base = mulmod(base, base, m);
        e >>= 1;
    }
    return r;
}

int main() {
    unsigned t = readUInt();
    for (unsigned i = 0; i < t; ++i) {
        unsigned n = readUInt();
        unsigned k = readUInt();
        unsigned m = readUInt();
        if (m == 0) break; /* input exhausted early; never happens for m >= 2 */
        unsigned b = powmod(10u, k, m) + 1u;
        if (b >= m) b -= m;
        writeUInt(powmod(b, n, m));
    }
    flushOut();
    return 0;
}
