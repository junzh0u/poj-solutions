// POJ 3537 - Crosses and Crosses
// Model: claude-opus-5
//
// 1 x n strip, players alternately put a cross on a free cell; whoever first
// completes three crosses in a row wins.  Decide who wins with optimal play.
//
// Reduction to a normal-play impartial game:
//   Under optimal play nobody ever plays a cell within distance 2 of an
//   existing cross.  If cell i holds a cross, playing i-1 lets the opponent
//   answer at i+1 (or i-2), and playing i-2 lets the opponent answer at i-1;
//   either way the opponent completes the triple at once and wins.  Playing at
//   distance >= 3 from every cross can never give anyone an immediate triple.
//   So all "safe" crosses are pairwise >= 3 apart, no two are ever adjacent,
//   and nobody can ever win outright while safe moves remain.  A player with no
//   safe move must poison a cell and lose on the reply.  Hence the game is the
//   normal-play game "place a cross at distance >= 3 from all others; the
//   player who cannot move loses".
//
// A maximal run of n cells that is free and untouched by any cross is one
// independent component.  Playing at position i (1..n) of such a run removes
// cells i-2..i+2 from play and splits it into runs of length i-3 and n-i-2, so
//   g[n] = mex { g[i-3] ^ g[n-i-2] : i = 1..n },  g[k] = 0 for k <= 0.
// The first player wins iff g[n] != 0.
//
// n <= 2000, so the O(n^2) table is trivial; everything fits in int.
// Verified against an exhaustive bitmask game-tree search (written from the
// statement's own wording) for n = 3..24, and against the losing-position list
// {6,12,22,30,32,44,54,64,76,86,98,110,118,130,132,162,170,184,194,202,282,
// 290,302,356,1046} reported on the discuss board.
//
// The statement gives a single n; the loop reads to EOF so extra data sets, if
// any, are handled too.

#include <cstdio>

const int MAXN = 2001;
const int MAXG = 4096; /* > any xor of two Grundy values, each <= n <= 2000 */

static int g[MAXN];
static int stamp[MAXG];

int main() {
    for (int n = 0; n < MAXN; n++) {
        int tag = n + 1;
        for (int i = 1; i <= n; i++) {
            int left = i - 3;
            int right = n - i - 2;
            int a = left > 0 ? g[left] : 0;
            int b = right > 0 ? g[right] : 0;
            stamp[a ^ b] = tag;
        }
        int m = 0;
        while (m < MAXG && stamp[m] == tag) m++;
        g[n] = m;
    }

    int n;
    while (scanf("%d", &n) == 1) {
        if (n < 0) n = 0;
        if (n >= MAXN) n = MAXN - 1;
        printf("%d\n", g[n] != 0 ? 1 : 2);
    }
    return 0;
}
