// POJ 3508 - Hide That Number
// Model: claude-opus-5
//
// Alex writes W = last n digits of 11*X, where n is the digit count of the
// original number X.  Given W (n digits) recover X, i.e. solve
//     11*X == W  (mod 10^n),  10^(n-1) <= X < 10^n.
// gcd(11,10^n)=1, so X is unique modulo 10^n; the only way to fail is for that
// unique residue to have a leading zero, i.e. fewer than n digits.  That is
// exactly the sample's 9988 -> 0908 -> IMPOSSIBLE, which also settles the
// ambiguity of whether a phone number may start with 0: it may not.
//
// n reaches 10^6, so the inverse is taken digit-wise instead of with bignum
// arithmetic.  Writing x_i for the digits of X from the least significant and
// c_i for the carry, column i of X*10 + X reads
//     w_i = (x_i + x_{i-1} + c_i) mod 10,
// so scanning from the least significant digit upward,
//     t = w_i - x_{i-1} - c_i;  x_i = t<0 ? t+10 : t;  c_{i+1} = t<0.
// One pass, in place, O(n) with constant 1; the final carry falls off the left
// and is discarded by the truncation.  Input can be many megabytes, so both
// directions use hand-rolled buffered I/O (the discuss board is full of TLEs
// from iostream and from calling strlen inside the loop).

#include <cstdio>
#include <cstring>

static const int MAXN = 1000006;
static char buf[MAXN];

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

static char obuf[1 << 16];
static int opos = 0;

static inline void flushout() {
    if (opos) { fwrite(obuf, 1, (size_t)opos, stdout); opos = 0; }
}

static inline void pc(char c) {
    if (opos == (int)sizeof(obuf)) flushout();
    obuf[opos++] = c;
}

static void pstr(const char *s, int len) {
    while (len > 0) {
        if (opos == (int)sizeof(obuf)) flushout();
        int room = (int)sizeof(obuf) - opos;
        int take = len < room ? len : room;
        memcpy(obuf + opos, s, (size_t)take);
        opos += take;
        s += take;
        len -= take;
    }
}

static void pint(int v) {
    char tmp[12];
    int k = 0;
    if (v == 0) tmp[k++] = '0';
    while (v > 0) { tmp[k++] = (char)('0' + v % 10); v /= 10; }
    while (k > 0) pc(tmp[--k]);
}

int main() {
    int caseno = 0;
    for (;;) {
        int c = gc();
        while (c != -1 && (c < '0' || c > '9')) c = gc();
        if (c == -1) break;
        int n = 0;
        while (c >= '0' && c <= '9') {
            if (n < MAXN - 1) buf[n] = (char)c;
            ++n;
            c = gc();
        }
        if (n == 1 && buf[0] == '0') break;   /* the terminating single zero */

        ++caseno;
        int prev = buf[n - 1] - '0';          /* x_0 = w_0, no carry in */
        int carry = 0;
        for (int p = n - 2; p >= 0; --p) {
            int t = (buf[p] - '0') - prev - carry;
            if (t < 0) { t += 10; carry = 1; } else { carry = 0; }
            buf[p] = (char)('0' + t);
            prev = t;
        }

        pint(caseno);
        pc('.');
        pc(' ');
        if (buf[0] == '0') pstr("IMPOSSIBLE", 10);
        else pstr(buf, n);
        pc('\n');
    }
    flushout();
    return 0;
}
