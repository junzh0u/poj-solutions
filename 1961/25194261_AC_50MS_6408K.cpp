// POJ 1961 - Period
//
// For every prefix of S of length i (2 <= i <= N) report the largest K > 1
// such that the prefix equals A^K, i.e. A repeated K times.
//
// Approach: KMP failure function.
//   fail[i] = length of the longest proper border of s[0..i-1].
//   p = i - fail[i] is the smallest period of that prefix.
//   The prefix is a K-th power iff p divides i and p < i, and then K = i / p.
//
// Why the smallest period is enough: suppose some d with d | i, d < i is a
// period, and p is the smallest period. Then d <= i/2, so
// p + d - gcd(p,d) <= 2d <= i, and by Fine and Wilf gcd(p,d) is also a
// period. Minimality of p forces gcd(p,d) = p, hence p | d | i. So if p does
// not divide i, no divisor of i is a period at all, and only p needs checking.
//
// N is up to 1e6 per case with an unbounded number of cases, and the answer
// can be ~N lines per case (e.g. "aaa...a" prints 999999 lines), so both the
// reader and the writer are hand-rolled buffered ones; formatting the output
// dominates the runtime otherwise.
//
// Statement notes: characters are ASCII 97..126, so a whitespace-delimited
// token is safe. The output format is fussy: a "Test case #k" header for every
// case and a blank line after every case, including the last. Input ends with
// a line holding 0. The declared N is trusted, but the code falls back to the
// actual token length if it is shorter.
#include <cstdio>
#include <cstring>

static const int MAXN = 1000006;

static char s[MAXN];
static int fail_[MAXN];

/* ---- fast input ---- */
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
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    if (c == -1) return false;
    int neg = 0;
    if (c == '-') { neg = 1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = neg ? -v : v;
    return true;
}

/* read a token of exactly n characters (skipping leading whitespace) */
static inline int readStr(char *dst, int n) {
    int c = gc();
    while (c != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) c = gc();
    int k = 0;
    while (c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        if (k < n) dst[k] = (char)c;
        ++k;
        c = gc();
    }
    if (k > n) k = n;
    dst[k] = '\0';
    return k;
}

/* ---- fast output ---- */
static char obuf[1 << 16];
static int opos = 0;

static inline void oflush() {
    if (opos) { fwrite(obuf, 1, (size_t)opos, stdout); opos = 0; }
}
static inline void oc(char c) {
    if (opos == (int)sizeof(obuf)) oflush();
    obuf[opos++] = c;
}
static inline void ostr(const char *p) {
    while (*p) oc(*p++);
}
static inline void oint(int v) {
    char tmp[12];
    int t = 0;
    if (v == 0) { oc('0'); return; }
    while (v > 0) { tmp[t++] = (char)('0' + v % 10); v /= 10; }
    while (t > 0) oc(tmp[--t]);
}

int main() {
    int n;
    int tc = 0;
    while (readInt(n)) {
        if (n == 0) break;
        int len = readStr(s, n);
        if (len < n) n = len; /* defensive: trust the actual string length */

        /* KMP failure function: fail_[i] = length of longest proper
           border of s[0..i-1] */
        fail_[0] = 0;
        if (n > 0) fail_[1] = 0;
        int k = 0;
        for (int i = 1; i < n; ++i) {
            while (k > 0 && s[i] != s[k]) k = fail_[k];
            if (s[i] == s[k]) ++k;
            fail_[i + 1] = k;
        }

        ++tc;
        ostr("Test case #");
        oint(tc);
        oc('\n');
        for (int i = 2; i <= n; ++i) {
            int p = i - fail_[i];      /* smallest period candidate */
            if (p < i && i % p == 0) {
                oint(i);
                oc(' ');
                oint(i / p);
                oc('\n');
            }
        }
        oc('\n');
    }
    oflush();
    return 0;
}
