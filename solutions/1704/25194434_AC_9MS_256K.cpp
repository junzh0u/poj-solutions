// POJ 1704 - Georgia and Bob
//
// N chessmen sit on distinct grids of a half-infinite row numbered from 1.
// A move slides one chessman left by >= 1 grid without passing another
// chessman or the left edge; whoever cannot move loses.  Impartial game,
// so Sprague-Grundy applies -- the only work is finding which pairing of
// the stones turns the empty gaps into independent Nim heaps.
//
// Sort the positions.  Pair them from the RIGHT: (p[n-2],p[n-1]),
// (p[n-4],p[n-3]), ...  The heap of a pair is the number of free grids
// between its two stones, p[hi]-p[lo]-1.  Moving the LEFT stone of a pair
// grows that heap, but the opponent can immediately restore it by moving
// the right stone the same distance, so those moves are reversible and
// contribute nothing.  Moving the RIGHT stone of a pair shrinks the heap by
// any positive amount -- exactly a Nim move.  Hence the position is a loss
// for the mover iff the XOR of the gaps is 0.
//
// The parity of N decides the pairing.  With N even every stone is in a
// pair.  With N odd the leftmost stone is unpaired, so give it a virtual
// partner at grid 0 (the left edge): its heap is p[0]-1, the room it has
// to slide.  Equivalently, pairing right-to-left leaves the odd stone
// paired with the wall rather than being ignored.
//
// Verified against an exhaustive game-tree search over every stone subset
// of small boards, both parities.
//
// Statement note: the output spec mentions a third answer, "Not sure", but
// the game is finite and impartial, so one side always has a winning
// strategy and that branch is unreachable.
//
// O(N log N) per case for the sort; N <= 1000, T <= 20.

#include <cstdio>
#include <algorithm>

int p[1005];

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int n;
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) scanf("%d", &p[i]);
        std::sort(p, p + n);
        int start;
        int g = 0;
        if (n % 2 == 1) {
            /* virtual stone at 0: pair it with p[0] */
            g ^= (p[0] - 0 - 1);
            start = 1;
        } else {
            start = 0;
        }
        for (int i = start; i + 1 < n; i += 2)
            g ^= (p[i + 1] - p[i] - 1);
        printf("%s\n", g ? "Georgia will win" : "Bob will win");
    }
    return 0;
}
