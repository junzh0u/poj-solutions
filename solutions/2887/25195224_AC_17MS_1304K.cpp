// POJ 2887 - Big String
// Model: claude-opus-5
//
// Approach: never build the string.  With at most 2000 commands there are at
// most 2000 insertions, so a query is answered by rewinding it through them.
// Insertion i put character ch_i at position e_i of the string that existed
// just before it, where e_i = min(p_i, len_{i-1} + 1) -- the clamp is the
// statement's "if p is larger than the length of the string, the character is
// appended to the end".  e_i is fixed once and never moves.  Mapping a
// position q of the string after insertion i back to the string before it:
//   q == e_i  ->  that is exactly ch_i, answer found;
//   q >  e_i  ->  it sat at q-1 before the shift;
//   q <  e_i  ->  unchanged.
// So walk the insertions that preceded the query newest-first, adjusting q;
// if nothing claims it, q indexes the original string directly.  O(N) per
// query, O(N^2) = 4e6 steps overall, plus one linear read of the input.
//
// Notes on the input, from the problem's discuss board: it is a *single* test
// case -- looping to EOF times out, apparently on junk after the data -- and
// the ~1 MB first line needs raw reading (cin >> is reported to TLE), hence
// the fread-based scanner.  The one "this is WA" post about this rewind idea
// turned out to be an undersized 1011-element array for N <= 2000; the
// author says so himself in the follow-up.
//
// Ambiguity: p for an I command has no stated upper bound, so the position
// parse saturates instead of overflowing (see POS_CAP).

#include <cstdio>
#include <cstring>

static const int MAXS = 1000006;
static const int MAXN = 2005;
/* Any position beyond the largest reachable length (1,000,000 + 2,000) is
   equivalent to "past the end", so saturating the parse here keeps every
   quantity inside plain int without needing long long (which POJ's older
   C++ front end is not guaranteed to accept under -pedantic C++98). */
static const int POS_CAP = 2000000;

static char s[MAXS];
static int slen;

static char insCh[MAXN];
static int insPos[MAXN];
static int insCnt;

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

static char obuf[8192];
static int olen = 0;

static void putch(char c) {
    if (olen == (int)sizeof(obuf)) { fwrite(obuf, 1, olen, stdout); olen = 0; }
    obuf[olen++] = c;
}

static int readInt() {
    int c, v = 0;
    while ((c = gc()) != -1 && (c < '0' || c > '9')) {}
    for (; c >= '0' && c <= '9'; c = gc()) {
        if (v <= POS_CAP) v = v * 10 + (c - '0');
    }
    if (v > POS_CAP) v = POS_CAP;
    return v;
}

static int readToken() {
    int c;
    while ((c = gc()) != -1 && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) {}
    return c;
}

int main() {
    int c;
    /* First line: the initial string, up to 1,000,000 characters. */
    slen = 0;
    while ((c = gc()) != -1 && c != '\n') {
        if (c == '\r') continue;
        if (slen < MAXS - 1) s[slen++] = (char)c;
    }
    s[slen] = '\0';

    int n = readInt();

    int curLen = slen;
    insCnt = 0;

    for (int q = 0; q < n; ++q) {
        int op = readToken();
        if (op == -1) break;

        if (op == 'I') {
            int ch = readToken();
            int p = readInt();
            if (p > curLen + 1) p = curLen + 1;   /* appended to the end */
            if (p < 1) p = 1;
            insCh[insCnt] = (char)ch;
            insPos[insCnt] = p;
            ++insCnt;
            ++curLen;
        } else { /* 'Q' */
            int p = readInt();
            char ans = 0;
            for (int j = insCnt - 1; j >= 0; --j) {
                if (p == insPos[j]) { ans = insCh[j]; break; }
                if (p > insPos[j]) --p;
            }
            if (ans == 0) ans = s[p - 1];
            putch(ans);
            putch('\n');
        }
    }

    if (olen) fwrite(obuf, 1, olen, stdout);
    return 0;
}
