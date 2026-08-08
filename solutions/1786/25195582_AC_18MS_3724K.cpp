// POJ 1786 - Bridge Hands
// Model: claude-opus-5
//
// Deal 52 cards round-robin, sort each hand, print it as a 5-line card picture.
//
// Seats clockwise are N, E, S, W, so "starting with the player on her left and
// proceeding clockwise" means card i goes to seat (dealer + 1 + i) mod 4 and the
// 52nd lands back on the dealer.  Display order is fixed at South, West, North,
// East by "South player always goes first" plus the sample.  Suits rank
// C < D < S < H and ranks 2..9 < T < J < Q < K < A.
//
// Ambiguity: the statement says deals are "separated by blank lines", which
// leaves the last deal open.  The discuss board says to emit a blank line after
// every deal including the last, so that is what this does; POJ ignores trailing
// whitespace at end of file, making the choice safe under either reading.
//
// The sample only ever uses dealer N, so it cannot distinguish the real rotation
// from one that always starts at East; a hand-derived 4-dealer case pins it.
// Input framing is taken loosely: the deck is read as whitespace-delimited
// tokens until 104 characters have accumulated, so any line splitting works, and
// the loop ends on '#' or on EOF.
//
// Output is assembled in a 1 MB buffer and written with fwrite; the board
// reports TLE for solutions that print line by line.

#include <cstdio>
#include <cstdlib>
#include <cstring>

static const char *SUITS = "CDSH";              /* club < diamond < spade < heart */
static const char *RANKS = "23456789TJQKA";     /* 2 low, Ace high               */
static const char *POS   = "NESW";              /* seats in clockwise order      */

static int idxOf(const char *tbl, char c) {
    const char *p = strchr(tbl, c);
    return p ? (int)(p - tbl) : -1;
}

/* ---------------- input: whole stdin, whitespace-delimited tokens ------------ */
static char *inbuf;
static size_t inlen, ipos;

static void slurp() {
    size_t cap = 1 << 20;
    inbuf = (char *)malloc(cap);
    inlen = 0;
    for (;;) {
        if (inlen + (1 << 16) > cap) { cap <<= 1; inbuf = (char *)realloc(inbuf, cap); }
        size_t got = fread(inbuf + inlen, 1, 1 << 16, stdin);
        inlen += got;
        if (got == 0) break;
    }
    ipos = 0;
}

static int nextToken(const char **beg, size_t *len) {
    while (ipos < inlen && (unsigned char)inbuf[ipos] <= ' ') ++ipos;
    if (ipos >= inlen) return 0;
    size_t s = ipos;
    while (ipos < inlen && (unsigned char)inbuf[ipos] > ' ') ++ipos;
    *beg = inbuf + s;
    *len = ipos - s;
    return 1;
}

/* ---------------- output: one big buffer, flushed in chunks ------------------ */
static char *obuf;
static size_t olen, ocap;

static void oflush() { fwrite(obuf, 1, olen, stdout); olen = 0; }
static void oreserve(size_t n) { if (olen + n > ocap) oflush(); }
static void oputs(const char *s, size_t n) { oreserve(n); memcpy(obuf + olen, s, n); olen += n; }

int main() {
    ocap = 1 << 20;
    obuf = (char *)malloc(ocap);
    olen = 0;
    slurp();

    char border[55];
    border[0] = '+';
    for (int i = 0; i < 13; ++i) { border[1 + 4 * i] = '-'; border[2 + 4 * i] = '-';
                                   border[3 + 4 * i] = '-'; border[4 + 4 * i] = '+'; }
    border[53] = '\n';

    /* display order is fixed: South first, then West, North, East */
    static const int showSeat[4] = { 2, 3, 0, 1 };
    static const char *showName[4] = { "South player:\n", "West player:\n",
                                       "North player:\n", "East player:\n" };
    static const size_t showLen[4] = { 14, 13, 14, 13 };

    const char *tok;
    size_t tlen;

    while (nextToken(&tok, &tlen)) {
        if (tok[0] == '#') break;
        int dealer = idxOf(POS, (char)(tok[0] & ~0x20));   /* accept lower case too */
        if (dealer < 0) break;

        /* the deck may be split across lines in any way; just take 104 characters */
        char deck[104];
        size_t have = 0;
        for (size_t i = 1; i < tlen && have < 104; ++i) deck[have++] = tok[i];
        while (have < 104) {
            if (!nextToken(&tok, &tlen)) break;
            for (size_t i = 0; i < tlen && have < 104; ++i) deck[have++] = tok[i];
        }
        if (have < 104) break;

        /* deal: first card to the player on the dealer's left, then clockwise */
        bool hand[4][52];
        memset(hand, 0, sizeof(hand));
        for (int i = 0; i < 52; ++i) {
            int s = idxOf(SUITS, deck[2 * i]);
            int r = idxOf(RANKS, deck[2 * i + 1]);
            if (s < 0 || r < 0) return 0;
            hand[(dealer + 1 + i) & 3][s * 13 + r] = true;
        }

        for (int k = 0; k < 4; ++k) {
            const bool *h = hand[showSeat[k]];
            char rankRow[55], suitRow[55];
            rankRow[0] = suitRow[0] = '|';
            int col = 0;
            for (int c = 0; c < 52; ++c) {
                if (!h[c]) continue;
                char *p = rankRow + 1 + 4 * col, *q = suitRow + 1 + 4 * col;
                p[0] = RANKS[c % 13]; p[1] = ' '; p[2] = RANKS[c % 13]; p[3] = '|';
                q[0] = ' ';           q[1] = SUITS[c / 13]; q[2] = ' '; q[3] = '|';
                ++col;
            }
            rankRow[53] = suitRow[53] = '\n';

            oputs(showName[k], showLen[k]);
            oputs(border, 54);
            oputs(rankRow, 54);
            oputs(suitRow, 54);
            oputs(rankRow, 54);
            oputs(border, 54);
        }
        oputs("\n", 1);   /* ...and a trailing blank line after the last one too */
    }

    oflush();
    return 0;
}
