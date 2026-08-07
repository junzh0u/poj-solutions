// POJ 2329 - Nearest number - 2
// Model: claude-opus-5
//
// Replace every zero of an N x N matrix (N <= 200) by the nearest non-zero
// entry under the Manhattan metric; if two or more non-zero entries tie for
// nearest, the zero stays a zero.
//
// Approach: one multi-source BFS over the whole grid, O(N^2) per test case.
// Because the grid has no obstacles, the BFS layer of a cell equals its
// Manhattan distance to the nearest non-zero, so the BFS is exact.  Alongside
// the distance each cell carries src[] = the index of its unique nearest
// non-zero, or -1 for "two or more".
//
// The propagation rule is exact, not a heuristic.  For a cell c at distance d,
// its set of nearest sources is exactly the union of the nearest-source sets of
// its neighbours at distance d-1: any source s at Manhattan distance d from c
// has a shortest grid path whose first step lands on a neighbour n with
// |n-s| = d-1, and no neighbour can be closer than d-1 or c would not be at d;
// conversely s in S(n) gives |c-s| <= (d-1)+1 = d and >= d, so |c-s| = d.  So c
// is ambiguous iff some neighbour at d-1 is ambiguous, or two such neighbours
// hold different unique sources -- which is the single test src[v] != src[u]
// applied whenever dst[v] == dst[u]+1.  FIFO order guarantees src[u] is final
// when u is dequeued, since every cell at distance d-1 is dequeued (and has
// expanded) before any cell at distance d.
//
// Statement ambiguity, and how it was settled: "if there are two or more
// nearest non-zeroes" counts non-zero ELEMENTS, not distinct values.  Two
// nearest non-zeroes that happen to carry the SAME value are still a tie and
// still leave a zero.  The official 3x3 sample cannot distinguish the two
// readings, but the discuss board can: both posted Accepted solutions tie-break
// on element identity (the Pascal one compares stored source coordinates, the
// other counts cells on the diamond ring regardless of their values).  On the
// board's own 5x5 all-ones diamond the readings disagree completely -- element
// counting leaves the matrix unchanged, value counting turns it into all 1s.
//
// Input framing the statement omits: the file holds several matrices and must
// be read to EOF (the board's test posts concatenate cases, and one posts the
// matching concatenated output).  A matrix of all zeroes stays all zeroes;
// non-zero entries are never modified, and replacements are read from the
// ORIGINAL matrix rather than from partially rewritten values.

#include <cstdio>

const int MAXN = 205;
const int MAXC = MAXN * MAXN;

static int val[MAXC];
static int dst[MAXC];
static int src[MAXC];
static int que[MAXC];
static int n;

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
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    out = x * sgn;
    return true;
}

static char obuf[1 << 16];
static int opos = 0;

static void flushOut() { fwrite(obuf, 1, opos, stdout); opos = 0; }

static void putCh(char c) {
    if (opos == (int)sizeof(obuf)) flushOut();
    obuf[opos++] = c;
}

static void putInt(int x) {
    char tmp[12];
    int k = 0;
    if (x == 0) { putCh('0'); return; }
    while (x > 0) { tmp[k++] = (char)('0' + x % 10); x /= 10; }
    while (k > 0) putCh(tmp[--k]);
}

int main() {
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    while (readInt(n)) {
        if (n <= 0) continue;
        int total = n * n;
        for (int i = 0; i < total; ++i) readInt(val[i]);

        int head = 0, tail = 0;
        for (int i = 0; i < total; ++i) {
            if (val[i] != 0) { dst[i] = 0; src[i] = i; que[tail++] = i; }
            else { dst[i] = -1; src[i] = -2; }
        }
        while (head < tail) {
            int u = que[head++];
            int r = u / n, c = u - r * n;
            int nd = dst[u] + 1;
            for (int k = 0; k < 4; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                int v = nr * n + nc;
                if (dst[v] < 0) {
                    dst[v] = nd;
                    src[v] = src[u];
                    que[tail++] = v;
                } else if (dst[v] == nd && src[v] != src[u]) {
                    src[v] = -1;
                }
            }
        }

        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                int i = r * n + c;
                int out;
                if (val[i] != 0) out = val[i];
                else if (src[i] < 0) out = 0;
                else out = val[src[i]];
                if (c) putCh(' ');
                putInt(out);
            }
            putCh('\n');
        }
    }
    flushOut();
    return 0;
}
