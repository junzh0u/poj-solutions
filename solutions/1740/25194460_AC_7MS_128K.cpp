// POJ 1740 - A New Stone Game
//
// A move: pick a pile, discard at least one stone from it, then freely
// redistribute any number of the pile's remaining stones onto other piles that
// still have stones. The player unable to move (all piles empty) loses.
//
// Losing positions (for the player to move) are exactly those whose non-empty
// piles can be split into pairs of equal size — equivalently, every pile size
// occurs an even number of times. So Alice wins iff some size has odd
// multiplicity.
//
// Why: from a paired position the opponent mirrors. Whatever the mover does
// inside one pile of a pair (discard some stones, push some onto other piles),
// the mirroring player performs the same discard on the partner pile and pushes
// the same amounts onto the partners of the destinations, restoring the pairing.
// Every move strictly decreases the total number of stones, so the game ends,
// and it ends on the mover — the mirroring player always has a reply. Conversely
// any unpaired position has a move to a paired one: sort the sizes descending and
// pair them up greedily; the largest unmatched pile can absorb the discard while
// its surplus is redistributed to level the remaining mismatches. The rule was
// confirmed by an exhaustive win/lose search over every multiset up to n=6.
//
// Statement notes: stones moved out may only land on piles that still have
// stones, and the pile chosen may itself end at zero. Neither detail changes the
// characterisation. A pile of size 0 in the input is simply ignored; the input
// ends with a line containing n = 0.

#include <cstdio>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        int cnt[101];
        for (int i = 0; i <= 100; ++i) cnt[i] = 0;
        for (int i = 0; i < n; ++i) {
            int x;
            if (scanf("%d", &x) != 1) return 0;
            if (x > 0) cnt[x]++;
        }
        int alice = 0;
        for (int v = 1; v <= 100; ++v)
            if (cnt[v] & 1) { alice = 1; break; }
        printf("%d\n", alice);
    }
    return 0;
}
