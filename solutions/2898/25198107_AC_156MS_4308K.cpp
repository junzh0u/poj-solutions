// POJ 2898 - Entertainment
// Model: claude-sonnet-5
//
// Approach: "Same game" style puzzle. For each selected entry, BFS-flood-fill
// (iterative, not recursive, to avoid stack overflow on up to 1000x1000
// grids -- confirmed by the discuss board as a common source of RE) the
// 4-connected group of equal characters and blank it out. Then compact the
// table in two independent passes exactly as described: (1) each row's
// non-blank characters slide left, keeping their left-to-right order; (2)
// each column's non-blank characters (of the row-compacted table) slide
// down, keeping their top-to-bottom order. Finally the fully blank trailing
// columns and leading rows are dropped, shrinking the table. It can be shown
// (conjugate-partition argument on the per-row lengths) that after both
// passes each row's blanks form a trailing suffix, so printing a row simply
// means printing its non-blank prefix -- matching the sample's ragged-width
// output exactly.
//
// Coordinates in each query are always relative to the *current* (already
// shrunk) table, and the statement guarantees they land on a non-blank
// entry, so no bounds/blank checks are needed on the query itself.
//
// Ambiguity settled via discuss board (message 119454): if the map becomes
// completely empty, nothing at all is printed for it (not even a blank
// line) -- only the "Test case #i:" header. Board also confirms (message
// 50467/50458) that query coordinates are relative to the table state at
// the time of the query, and (message 176448) that plain token-based
// (scanf/getchar-free) parsing avoids an input-desync pitfall when k == 0.
// The grid's row count m and column count n are never given explicitly:
// consecutive whitespace-separated tokens are grid rows as long as they
// start with a lowercase letter; the first token starting with a digit is
// k.

#include <cstdio>
#include <cstdlib>
#include <cstring>

static const int MAXD = 1002;

static char grid[MAXD][MAXD];
static int R, C;

// ---------- fast input ----------
static char ibuf[1 << 20];
static int ilen = 0, ipos = 0;
static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (!ilen) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}
static inline bool isws(int c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}
// returns token length, or -1 on EOF (no token found)
static int readToken(char *out) {
    int c = gc();
    while (c != -1 && isws(c)) c = gc();
    if (c == -1) return -1;
    int len = 0;
    while (c != -1 && !isws(c)) {
        out[len++] = (char)c;
        c = gc();
    }
    out[len] = 0;
    return len;
}
static char tokbuf[MAXD + 10];
static inline int readInt() {
    int len = readToken(tokbuf);
    if (len < 0) return -1;
    return atoi(tokbuf);
}

// ---------- fast output ----------
static char obuf[1 << 20];
static int olen = 0;
static inline void oc(char c) {
    obuf[olen++] = c;
    if (olen == (int)sizeof(obuf)) {
        fwrite(obuf, 1, olen, stdout);
        olen = 0;
    }
}
static inline void ostr(const char *s) {
    while (*s) oc(*s++);
}
static inline void oflush() {
    if (olen) {
        fwrite(obuf, 1, olen, stdout);
        olen = 0;
    }
}

// ---------- BFS flood fill ----------
static int qr[MAXD * MAXD];
static int qc[MAXD * MAXD];

static void flood(int sr, int sc) {
    char ch = grid[sr][sc];
    int head = 0, tail = 0;
    grid[sr][sc] = 0;
    qr[tail] = sr; qc[tail] = sc; tail++;
    while (head < tail) {
        int x = qr[head], y = qc[head];
        head++;
        if (x > 0 && grid[x - 1][y] == ch) { grid[x - 1][y] = 0; qr[tail] = x - 1; qc[tail] = y; tail++; }
        if (x + 1 < R && grid[x + 1][y] == ch) { grid[x + 1][y] = 0; qr[tail] = x + 1; qc[tail] = y; tail++; }
        if (y > 0 && grid[x][y - 1] == ch) { grid[x][y - 1] = 0; qr[tail] = x; qc[tail] = y - 1; tail++; }
        if (y + 1 < C && grid[x][y + 1] == ch) { grid[x][y + 1] = 0; qr[tail] = x; qc[tail] = y + 1; tail++; }
    }
}

static char tmpRow[MAXD];
static char tmpCol[MAXD];

static void processQuery(int r, int c) {
    int sr = r - 1, sc = c - 1;
    flood(sr, sc);

    // Pass 1: compact each row to the left.
    int maxRowLen = 0;
    for (int i = 0; i < R; i++) {
        int len = 0;
        for (int j = 0; j < C; j++)
            if (grid[i][j] != 0) tmpRow[len++] = grid[i][j];
        for (int j = 0; j < len; j++) grid[i][j] = tmpRow[j];
        for (int j = len; j < C; j++) grid[i][j] = 0;
        if (len > maxRowLen) maxRowLen = len;
    }
    int newC = maxRowLen;

    if (newC == 0) {
        R = 0;
        C = 0;
        return;
    }

    // Pass 2: compact each (remaining) column downward.
    int maxColCnt = 0;
    for (int j = 0; j < newC; j++) {
        int cnt = 0;
        for (int i = 0; i < R; i++)
            if (grid[i][j] != 0) tmpCol[cnt++] = grid[i][j];
        int pad = R - cnt;
        for (int i = 0; i < pad; i++) grid[i][j] = 0;
        for (int i = 0; i < cnt; i++) grid[pad + i][j] = tmpCol[i];
        if (cnt > maxColCnt) maxColCnt = cnt;
    }
    int newR = maxColCnt;
    int offset = R - newR;
    if (offset > 0) {
        for (int i = 0; i < newR; i++)
            for (int j = 0; j < newC; j++)
                grid[i][j] = grid[offset + i][j];
    }
    R = newR;
    C = newC;
}

int main() {
    int tc = 0;
    char tok[MAXD + 10];
    for (;;) {
        int len = readToken(tok);
        if (len < 0) break; // EOF, no more test cases

        int m = 0, n = len;
        for (int j = 0; j < len; j++) grid[0][j] = tok[j];
        m = 1;

        int k = -1;
        for (;;) {
            len = readToken(tok);
            if (len < 0) { k = 0; break; } // malformed/EOF safety net
            if (tok[0] >= '0' && tok[0] <= '9') {
                k = atoi(tok);
                break;
            } else {
                for (int j = 0; j < len; j++) grid[m][j] = tok[j];
                m++;
            }
        }

        R = m;
        C = n;
        tc++;

        char hdr[64];
        sprintf(hdr, "Test case #%d:\n", tc);
        ostr(hdr);

        for (int q = 0; q < k; q++) {
            int r = readInt();
            int c = readInt();
            if (R > 0 && C > 0) processQuery(r, c);
        }

        for (int i = 0; i < R; i++) {
            int j = 0;
            while (j < C && grid[i][j] != 0) { oc(grid[i][j]); j++; }
            oc('\n');
        }
    }
    oflush();
    return 0;
}
