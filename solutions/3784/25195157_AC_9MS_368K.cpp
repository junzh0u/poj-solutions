// POJ 3784 - Running Median
// Model: claude-opus-5
//
// After each odd-indexed value of a data set, print the median of the values
// received so far.  Two heaps: a max-heap holding the lower half and a min-heap
// holding the upper half, kept so that |lo| == |hi| or |lo| == |hi| + 1.  The
// median after an odd count is then lo[0].  O(M log M) per data set.
//
// Output framing: one header line "<set number> <count of medians>" where the
// count is (M+1)/2, then the medians ten per line, no blank lines, no trailing
// spaces.  Values are 32-bit signed, so plain int throughout.
//
// P <= 1000 and M <= 9999, i.e. up to ~10^7 values, so both input parsing and
// output formatting are hand-rolled over fread/fwrite buffers.

#include <cstdio>
#include <cstring>

static const int MAXM = 10005;

static int lo[MAXM]; /* max-heap: lower half */
static int hi[MAXM]; /* min-heap: upper half */
static int loN, hiN;

static void pushLo(int v) {
    int i = loN++;
    lo[i] = v;
    while (i > 0) {
        int p = (i - 1) >> 1;
        if (lo[p] >= lo[i]) break;
        int t = lo[p]; lo[p] = lo[i]; lo[i] = t;
        i = p;
    }
}

static void pushHi(int v) {
    int i = hiN++;
    hi[i] = v;
    while (i > 0) {
        int p = (i - 1) >> 1;
        if (hi[p] <= hi[i]) break;
        int t = hi[p]; hi[p] = hi[i]; hi[i] = t;
        i = p;
    }
}

static int popLo() {
    int top = lo[0];
    lo[0] = lo[--loN];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, b = i;
        if (l < loN && lo[l] > lo[b]) b = l;
        if (r < loN && lo[r] > lo[b]) b = r;
        if (b == i) break;
        int t = lo[b]; lo[b] = lo[i]; lo[i] = t;
        i = b;
    }
    return top;
}

static int popHi() {
    int top = hi[0];
    hi[0] = hi[--hiN];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, b = i;
        if (l < hiN && hi[l] < hi[b]) b = l;
        if (r < hiN && hi[r] < hi[b]) b = r;
        if (b == i) break;
        int t = hi[b]; hi[b] = hi[i]; hi[i] = t;
        i = b;
    }
    return top;
}

/* ---- input ---- */
static char ibuf[1 << 16];
static int ipos, ilen;

static int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

/* returns 0 on EOF before any digit */
static int readInt(int *out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return 0;
    int neg = 0;
    if (c == '-') { neg = 1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    *out = neg ? -v : v;
    return 1;
}

/* ---- output ---- */
static char obuf[1 << 16];
static int opos;

static void flushOut() {
    if (opos) { fwrite(obuf, 1, opos, stdout); opos = 0; }
}

static void putCh(char c) {
    if (opos == (int)sizeof(obuf)) flushOut();
    obuf[opos++] = c;
}

static void putInt(int v) {
    char tmp[12];
    int n = 0;
    if (v < 0) {
        putCh('-');
        /* build digits from the negative side so INT_MIN is safe */
        do { tmp[n++] = (char)('0' - v % 10); v /= 10; } while (v);
    } else {
        do { tmp[n++] = (char)('0' + v % 10); v /= 10; } while (v);
    }
    while (n) putCh(tmp[--n]);
}

int main() {
    int p;
    if (!readInt(&p)) return 0;
    for (int cs = 0; cs < p; cs++) {
        int id, m;
        if (!readInt(&id)) break;
        if (!readInt(&m)) break;
        putInt(id);
        putCh(' ');
        putInt((m + 1) / 2);
        putCh('\n');

        loN = hiN = 0;
        int printed = 0;
        for (int i = 1; i <= m; i++) {
            int x;
            if (!readInt(&x)) x = 0;
            if (loN == 0 || x <= lo[0]) pushLo(x); else pushHi(x);
            if (loN > hiN + 1) pushHi(popLo());
            else if (hiN > loN) pushLo(popHi());
            if (i & 1) {
                if (printed) putCh(printed % 10 ? ' ' : '\n');
                putInt(lo[0]);
                printed++;
            }
        }
        if (printed) putCh('\n');
    }
    flushOut();
    return 0;
}
