// POJ 2570 - Fiber Network
// Model: claude-opus-5
//
// Each of the 26 companies owns its own directed graph on the same node set;
// a query (A,B) asks which companies can route A->B inside their own graph.
// Instead of 26 separate transitive closures, keep one matrix of 26-bit masks
// (bit c set in G[i][j] iff company c owns the arc i->j) and run a single
// Floyd-Warshall closure on it:  G[i][j] |= G[i][k] & G[k][j].
// Every bit position evolves independently under & and |, so this is exactly
// 26 closures computed bit-parallel in one n^3 pass (n <= 200 => 8e6 ops).
// The `if (!Gik) continue` guard skips most of the inner loop on sparse data.
//
// Notes on the format (the discuss board's recurring pitfalls):
//  - several test cases, terminated by n = 0; blank line after *each* case,
//    including the last one;
//  - a connection may be listed more than once, so OR the masks instead of
//    assigning;
//  - board folklore says G++ (language 0) times out here where C++ (4) passes,
//    so this is submitted under 4; input is read with a whole-stdin fread
//    tokenizer and output through a manual buffer to keep I/O off the clock.

#include <cstdio>
#include <cstring>
#include <cstdlib>

static const int MAXN = 205;

static unsigned int G[MAXN][MAXN];

/* ---- input: read all of stdin, then tokenize by hand ---- */
static char *inbuf;
static size_t inpos, inlen;

static void readAll() {
    size_t cap = 1 << 20;
    size_t len = 0;
    inbuf = (char *)malloc(cap);
    for (;;) {
        if (len + (1 << 16) + 1 > cap) {
            cap <<= 1;
            inbuf = (char *)realloc(inbuf, cap);
        }
        size_t got = fread(inbuf + len, 1, 1 << 16, stdin);
        len += got;
        if (got < (size_t)(1 << 16)) break;
    }
    inbuf[len] = '\0';
    inlen = len;
    inpos = 0;
}

static int readInt() {
    while (inpos < inlen && (inbuf[inpos] < '0' || inbuf[inpos] > '9') &&
           inbuf[inpos] != '-')
        ++inpos;
    if (inpos >= inlen) return -1;
    int sign = 1;
    if (inbuf[inpos] == '-') { sign = -1; ++inpos; }
    int v = 0;
    while (inpos < inlen && inbuf[inpos] >= '0' && inbuf[inpos] <= '9')
        v = v * 10 + (inbuf[inpos++] - '0');
    return v * sign;
}

/* next whitespace-delimited token, folded into a 26-bit company mask */
static unsigned int readMask() {
    while (inpos < inlen && (unsigned char)inbuf[inpos] <= ' ') ++inpos;
    unsigned int m = 0;
    while (inpos < inlen && (unsigned char)inbuf[inpos] > ' ') {
        char c = inbuf[inpos++];
        if (c >= 'a' && c <= 'z') m |= 1u << (c - 'a');
    }
    return m;
}

/* ---- output buffer ---- */
static char *outbuf;
static size_t outpos, outcap;

static void outChar(char c) {
    if (outpos + 1 >= outcap) {
        outcap <<= 1;
        outbuf = (char *)realloc(outbuf, outcap);
    }
    outbuf[outpos++] = c;
}

int main() {
    readAll();
    outcap = 1 << 20;
    outbuf = (char *)malloc(outcap);
    outpos = 0;

    for (;;) {
        int n = readInt();
        if (n <= 0) break;

        for (int i = 1; i <= n; ++i)
            memset(G[i] + 1, 0, sizeof(unsigned int) * n);

        for (;;) {
            int a = readInt(), b = readInt();
            if (a == 0 && b == 0) break;
            unsigned int m = readMask();
            if (a >= 1 && a <= n && b >= 1 && b <= n) G[a][b] |= m;
        }

        for (int k = 1; k <= n; ++k) {
            const unsigned int *Gk = G[k];
            for (int i = 1; i <= n; ++i) {
                unsigned int gik = G[i][k];
                if (!gik) continue;
                unsigned int *Gi = G[i];
                for (int j = 1; j <= n; ++j) Gi[j] |= gik & Gk[j];
            }
        }

        for (;;) {
            int a = readInt(), b = readInt();
            if (a == 0 && b == 0) break;
            unsigned int m = (a >= 1 && a <= n && b >= 1 && b <= n) ? G[a][b] : 0u;
            if (!m) {
                outChar('-');
            } else {
                for (int c = 0; c < 26; ++c)
                    if (m & (1u << c)) outChar((char)('a' + c));
            }
            outChar('\n');
        }
        outChar('\n');
    }

    fwrite(outbuf, 1, outpos, stdout);
    return 0;
}
