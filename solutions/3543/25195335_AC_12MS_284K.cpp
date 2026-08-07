// POJ 3543 - iChess
// Model: claude-opus-5
//
// Given b black and w white tiles (0 <= b, w <= 10000), find the largest s such
// that an s x s board coloured in a chess pattern can be built from at most b
// black and at most w white tiles.
//
// An s x s chess-coloured board uses ceil(s*s/2) tiles of the corner's colour and
// floor(s*s/2) of the other; the corner may be either colour, so the board is
// feasible iff (hi <= b && lo <= w) || (lo <= b && hi <= w), where
// hi = (s*s+1)/2 and lo = s*s/2.
//
// s*s <= b + w <= 20000, so s <= 141 and a plain loop over s decides it.
// All arithmetic is integer: the discuss board reports pow(sqrt(25.0),2)
// evaluating to 24 at runtime on some compilers, so no floating point is used.
//
// Input framing: the statement says the file holds exactly one pair, and the
// discuss board confirms it (a loop that read past it produced Output Limit
// Exceeded). Reading with a checked while-loop is correct under both readings —
// on a single-pair file it runs exactly once — so it is used here.
//
// "Impossible" is printed when not even a 1x1 board fits, i.e. when b + w == 0.

#include <cstdio>

int main() {
    int b, w;
    while (scanf("%d %d", &b, &w) == 2) {
        int best = 0;
        for (int s = 1; s <= 141; ++s) {
            int cells = s * s;
            int hi = (cells + 1) / 2;
            int lo = cells / 2;
            if ((hi <= b && lo <= w) || (lo <= b && hi <= w)) best = s;
        }
        if (best == 0) printf("Impossible\n");
        else printf("%d\n", best);
    }
    return 0;
}
