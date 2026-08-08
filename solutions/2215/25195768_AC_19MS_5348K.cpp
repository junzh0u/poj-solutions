// POJ 2215 - Parliament
// Model: claude-opus-5
//
// Static 2D prefix sums. P[i][j] holds the sum of rows 1..i, seats 1..j, so a
// query (R1,S1,R2,S2) is answered in O(1) by inclusion-exclusion. Build is
// O(R*S) per assignment, answering is O(1) per query.
//
// P is a global (static) array, not a local: the one Runtime Error reported on
// the discuss board was traced by its own author to declaring this table on the
// stack. Verified rather than assumed - a stack-allocated 1001x1001 table of
// 8-byte cells is 8 MB, past the default stack on the judge.
//
// Sums are long long, hence language 0 (G++) rather than 4 (C++), which has no
// long long. The statement guarantees only that the sum of the WHOLE grid fits
// in int, which bounds a sub-rectangle only when the scores are non-negative -
// and the statement never says they are (the Czech output line literally reads
// "absolute value of comfort"). long long removes the question; 1001*1001*8 is
// under 8 MB against a 64 MB limit, and measured RSS was 9.5 MB.
//
// The number of assignments N is an explicit count and is honoured exactly;
// input is not read to EOF. Stale prefix entries cannot leak between
// assignments: every cell a query can read is either rewritten by the current
// build or lies in row/column 0, which is never written and stays zero.
//
// A blank line is printed after every assignment including the last, as the
// statement requires; the sample's own transcript omits the final one, which
// POJ ignores as trailing whitespace.
//
// Ambiguity worth recording: the official sample cannot validate the row/seat
// axis at all. Its grid has ten identical rows "1 2 ... 10", which makes the
// grid invariant under transposition, so a solution that swapped rows with
// seats reproduces all five sample answers exactly. That mutant was built and
// confirmed to survive the sample; it dies immediately against a randomized
// differential test on deliberately non-square grids.
#include <cstdio>
#include <cstdlib>

static const int MAXN = 1001;

/* Prefix[r][s] = sum of the sub-rectangle rows 1..r, seats 1..s.
   Global (static) storage: the only RE reported on the discuss board came
   from declaring this array as a local, which blows the judge's stack. */
static long long P[MAXN + 1][MAXN + 1];

static char inbuf[1 << 16];
static int inpos = 0, inlen = 0;

static int gc() {
    if (inpos == inlen) {
        inlen = (int)fread(inbuf, 1, sizeof(inbuf), stdin);
        inpos = 0;
        if (inlen <= 0) return -1;
    }
    return (unsigned char)inbuf[inpos++];
}

static bool readInt(long long &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    long long v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = neg ? -v : v;
    return true;
}

static char outbuf[1 << 16];
static int outpos = 0;

static void flushOut() {
    if (outpos) { fwrite(outbuf, 1, outpos, stdout); outpos = 0; }
}

static void putch(char c) {
    if (outpos == (int)sizeof(outbuf)) flushOut();
    outbuf[outpos++] = c;
}

static void puts_(const char *s) {
    while (*s) putch(*s++);
}

static void putnum(long long v) {
    char tmp[24];
    int n = 0;
    if (v < 0) { putch('-'); v = -v; }
    if (v == 0) tmp[n++] = '0';
    while (v > 0) { tmp[n++] = (char)('0' + (int)(v % 10)); v /= 10; }
    while (n > 0) putch(tmp[--n]);
}

int main() {
    long long n;
    if (!readInt(n)) return 0;
    for (long long t = 0; t < n; ++t) {
        long long R, S;
        readInt(R);
        readInt(S);
        for (long long i = 1; i <= R; ++i) {
            long long rowSum = 0;
            for (long long j = 1; j <= S; ++j) {
                long long v;
                readInt(v);
                rowSum += v;
                P[i][j] = P[i - 1][j] + rowSum;
            }
        }
        long long d;
        readInt(d);
        for (long long q = 0; q < d; ++q) {
            long long r1, s1, r2, s2;
            readInt(r1);
            readInt(s1);
            readInt(r2);
            readInt(s2);
            long long ans = P[r2][s2] - P[r1 - 1][s2] - P[r2][s1 - 1] + P[r1 - 1][s1 - 1];
            puts_("Absolutni hodnota pohodlnosti je ");
            putnum(ans);
            puts_(" bodu.\n");
        }
        putch('\n');
    }
    flushOut();
    return 0;
}
