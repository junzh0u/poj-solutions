// POJ 3690 - Constellations
// Model: claude-opus-5
//
// For each test case, count how many of the T given P x Q constellations occur
// as a sub-matrix of the N x M sky.
//
// Approach: a 2D rolling polynomial hash over the sky, streamed one row-band at
// a time.  colh[j] holds the vertical hash (base B1) of sky[i..i+P-1][j]; a
// horizontal rolling hash (base B2) over colh then yields the hash of every
// P x Q window of that band in O(M), and advancing the band from i to i+1 is
// another O(M) sweep.  So the whole sky costs O(N*M) independent of T, with
// only one M-sized array live -- every pass is a sequential sweep, which is the
// point of hashing columns first rather than rows.  The T constellation hashes
// go into a tiny open-addressed table; each window hash is looked up there, and
// any hit is confirmed by an exact P x Q comparison of the window against a
// representative constellation, so a hash collision can never produce a wrong
// answer (equal content always hashes equal, so no match can be missed).
// Constellations are grouped into content classes rather than by hash alone, so
// even two distinct patterns sharing a key resolve exactly.
//
// Two things the statement leaves open, both settled by the discuss board's
// case (message 106501 / 154510): a 3x3 all-star sky with T=2 copies of the 2x2
// all-star pattern has answer 2.
//   * a constellation occurring many times in the sky is still counted once
//     (else the answer would be 8);
//   * identical constellations listed twice among the T count twice (else 1).
// Input framing: cases run until a line of five zeros, and blank lines separate
// the constellations -- reading whitespace-delimited tokens absorbs both, since
// no grid row can be empty (M, Q >= 1).  P > N or Q > M is legal and yields 0,
// but the T constellations are still consumed so the stream stays in sync.
//
// Language 0 (G++): the hash needs 64-bit unsigned arithmetic.

#include <cstdio>
#include <cstring>

typedef unsigned long long u64;

static const int MAXD = 1005;
static const int MAXT = 105;
static const int MAXP = 55;

static const u64 B1 = 1000000007ULL;
static const u64 B2 = 1000000009ULL;

static unsigned char sky[MAXD][MAXD];
static unsigned char pat[MAXT][MAXP * MAXP];
static u64 colh[MAXD];
static u64 pkey[MAXT];

static int classOf[MAXT];
static int clsRep[MAXT];
static int clsNext[MAXT];
static bool clsFound[MAXT];

static const int TBITS = 10;
static const int TSIZE = 1 << TBITS;
static u64 tabKey[TSIZE];
static int tabHead[TSIZE];
static bool tabUsed[TSIZE];

/* ---- buffered input ---- */

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

static bool readInt(int &out) {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
    if (c == -1) return false;
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int v = 0;
    while (c >= '0' && c <= '9') { v = v * 10 + (c - '0'); c = gc(); }
    out = v * sgn;
    return true;
}

/* Reads one whitespace-delimited grid row into dst, mapping '*' -> 2 and
   anything else -> 1.  Always fills exactly cap cells (short rows are padded
   with '0', long ones truncated) so a malformed line cannot desynchronise the
   grid geometry. */
static void readRow(unsigned char *dst, int cap) {
    int c = gc();
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r') c = gc();
    int n = 0;
    while (c != -1 && c != ' ' && c != '\t' && c != '\n' && c != '\r') {
        if (n < cap) dst[n] = (c == '*') ? 2 : 1;
        ++n;
        c = gc();
    }
    for (; n < cap; ++n) dst[n] = 1;
}

/* ---- tiny open-addressed key table ---- */

static inline int slotOf(u64 h) {
    u64 x = h * 0x9E3779B97F4A7C15ULL;
    return (int)(x >> (64 - TBITS));
}

static inline int tabFind(u64 h) {
    int s = slotOf(h);
    while (tabUsed[s]) {
        if (tabKey[s] == h) return s;
        s = (s + 1) & (TSIZE - 1);
    }
    return -1;
}

static inline int tabInsert(u64 h) {
    int s = slotOf(h);
    while (tabUsed[s]) {
        if (tabKey[s] == h) return s;
        s = (s + 1) & (TSIZE - 1);
    }
    tabUsed[s] = true;
    tabKey[s] = h;
    tabHead[s] = -1;
    return s;
}

int main() {
    int caseNo = 0;
    int N, M, T, P, Q;

    while (readInt(N) && readInt(M) && readInt(T) && readInt(P) && readInt(Q)) {
        if (N == 0 && M == 0 && T == 0 && P == 0 && Q == 0) break;
        ++caseNo;

        int i, j, r, c, t;
        for (i = 0; i < N; ++i) readRow(sky[i], M);
        for (t = 0; t < T; ++t)
            for (r = 0; r < P; ++r) readRow(pat[t] + r * Q, Q);

        u64 ch[MAXP];
        for (t = 0; t < T; ++t) {
            for (c = 0; c < Q; ++c) ch[c] = 0;
            for (r = 0; r < P; ++r) {
                const unsigned char *row = pat[t] + r * Q;
                for (c = 0; c < Q; ++c) ch[c] = ch[c] * B1 + row[c];
            }
            u64 h = 0;
            for (c = 0; c < Q; ++c) h = h * B2 + ch[c];
            pkey[t] = h;
        }

        memset(tabUsed, 0, sizeof(tabUsed));
        int nCls = 0;
        for (t = 0; t < T; ++t) {
            int s = tabInsert(pkey[t]);
            int hit = -1;
            for (int cc = tabHead[s]; cc != -1; cc = clsNext[cc]) {
                if (memcmp(pat[t], pat[clsRep[cc]], (size_t)P * Q) == 0) { hit = cc; break; }
            }
            if (hit < 0) {
                hit = nCls++;
                clsRep[hit] = t;
                clsFound[hit] = false;
                clsNext[hit] = tabHead[s];
                tabHead[s] = hit;
            }
            classOf[t] = hit;
        }

        int remaining = nCls;

        if (P <= N && Q <= M && remaining > 0) {
            u64 powB1 = 1;
            for (int k = 1; k < P; ++k) powB1 *= B1;      /* B1^(P-1) */
            u64 powB2 = 1;
            for (int k = 0; k < Q; ++k) powB2 *= B2;      /* B2^Q */

            for (j = 0; j < M; ++j) colh[j] = 0;
            for (r = 0; r < P; ++r) {
                const unsigned char *row = sky[r];
                for (j = 0; j < M; ++j) colh[j] = colh[j] * B1 + row[j];
            }

            for (i = 0; ; ++i) {
                u64 h = 0;
                for (j = 0; j < Q; ++j) h = h * B2 + colh[j];
                for (j = Q - 1; ; ++j) {
                    if (j >= Q) h = h * B2 - colh[j - Q] * powB2 + colh[j];
                    int s = tabFind(h);
                    if (s >= 0) {
                        int left = j - Q + 1;
                        for (int cc = tabHead[s]; cc != -1; cc = clsNext[cc]) {
                            if (clsFound[cc]) continue;
                            const unsigned char *pp = pat[clsRep[cc]];
                            bool ok = true;
                            for (r = 0; r < P; ++r) {
                                if (memcmp(sky[i + r] + left, pp + r * Q, (size_t)Q) != 0) { ok = false; break; }
                            }
                            if (ok) { clsFound[cc] = true; --remaining; }
                        }
                        if (remaining == 0) break;
                    }
                    if (j + 1 >= M) break;
                }
                if (remaining == 0) break;
                if (i + P >= N) break;
                const unsigned char *out = sky[i];
                const unsigned char *in = sky[i + P];
                for (j = 0; j < M; ++j)
                    colh[j] = (colh[j] - (u64)out[j] * powB1) * B1 + in[j];
            }
        }

        int ans = 0;
        for (t = 0; t < T; ++t)
            if (clsFound[classOf[t]]) ++ans;
        printf("Case %d: %d\n", caseNo, ans);
    }
    return 0;
}
