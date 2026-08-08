// POJ 2397 - Spiderman
// Model: claude-opus-5
//
// A solution is a sign choice for each d[i]; the heights after each stage must
// stay >= 0 and end at 0.  The building height is (highest point reached) + 2,
// so the objective is to minimise the maximum height along the path -- the
// discuss board reports this as the WA people hit, since any legal path already
// satisfies everything else the statement says.
//
// DP over (stage, height): f[i][h] = smallest achievable "max height so far"
// among legal prefixes ending at height h after i stages.  f[0][0] = 0;
// going up to h' costs max(f, h'), going down to h' < h leaves the running max
// unchanged.  Smaller prefix maxima are never worse, so the DP is sound, and
// the answer is f[m][0] with the U/D string recovered from stored predecessors.
//
// Heights are bounded by min(prefix sum, suffix sum) -- above that the walk can
// no longer come back down to 0 -- which caps the state space at sum/2 <= 500
// and keeps the inner loop short (the board warns the case count can be high).
// Everything fits in 32-bit ints, so the language choice is free; submitted
// under 4 (C++).
//
// Ambiguity: none in the objective once "minimise the building height" is read
// as minimising the peak.  The official sample is no evidence either way -- it
// is reproduced byte-for-byte by a mutant that ignores the minimisation
// entirely, as well as by one that permits going below street level.

#include <cstdio>

static const int MAXM = 2005;
static const int MAXH = 2005;
static const int INF = 1000000000;

static int d[MAXM];
static int pre[MAXM], bnd[MAXM];
static int cur[MAXH + 1], nxt[MAXH + 1];
static char choice[MAXM][MAXH + 1];
static char out[MAXM + 2];

static char ibuf[1 << 16];
static int ipos = 0, ilen = 0;

static int readChar() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return (unsigned char)ibuf[ipos++];
}

/* reads a signed integer; returns 0 at end of input */
static int readInt(int *v) {
    int c = readChar();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = readChar();
    if (c == -1) return 0;
    int sign = 1;
    if (c == '-') { sign = -1; c = readChar(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = readChar(); }
    *v = x * sign;
    return 1;
}

int main() {
    int n;
    if (!readInt(&n)) return 0;
    while (n-- > 0) {
        int m;
        if (!readInt(&m)) return 0;
        if (m < 0) m = 0;
        if (m > MAXM - 1) m = MAXM - 1;
        int S = 0;
        for (int i = 0; i < m; i++) {
            if (!readInt(&d[i])) d[i] = 0;
            S += d[i];
        }

        pre[0] = 0;
        for (int i = 0; i < m; i++) pre[i + 1] = pre[i] + d[i];
        for (int i = 0; i <= m; i++) {
            int rest = S - pre[i];
            int b = pre[i] < rest ? pre[i] : rest;
            if (b > MAXH) b = MAXH;
            bnd[i] = b;
        }

        for (int h = 0; h <= bnd[0]; h++) cur[h] = INF;
        cur[0] = 0;

        for (int i = 0; i < m; i++) {
            int nb = bnd[i + 1];
            int cb = bnd[i];
            int step = d[i];
            for (int h = 0; h <= nb; h++) nxt[h] = INF;
            for (int h = 0; h <= cb; h++) {
                int v = cur[h];
                if (v >= INF) continue;
                int u = h + step;
                if (u <= nb) {
                    int cand = v > u ? v : u;
                    if (cand < nxt[u]) { nxt[u] = cand; choice[i + 1][u] = 'U'; }
                }
                int w = h - step;
                if (w >= 0 && w <= nb) {
                    if (v < nxt[w]) { nxt[w] = v; choice[i + 1][w] = 'D'; }
                }
            }
            for (int h = 0; h <= nb; h++) cur[h] = nxt[h];
        }

        if (m == 0) {
            putchar('\n');
        } else if (cur[0] >= INF) {
            puts("IMPOSSIBLE");
        } else {
            int h = 0;
            for (int i = m; i >= 1; i--) {
                char c = choice[i][h];
                out[i - 1] = c;
                h = (c == 'U') ? h - d[i - 1] : h + d[i - 1];
            }
            out[m] = '\0';
            puts(out);
        }
    }
    return 0;
}
