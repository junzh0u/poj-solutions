// POJ 2068 - Nim
// Model: claude-opus-5
//
// Misere Nim on one heap, played by 2n players seated in a fixed cycle:
// player p (0-based) may remove 1..M[p] stones, players 0,2,4,... are my team
// and 1,3,5,... the opponents, and whoever removes the LAST stone loses.
// Since the two teams strictly alternate, "the team of the player to move" is
// a well-defined side, so this is an ordinary two-player game DP:
//
//   win[p][s] = exists k in [1, min(M[p], s)] with s-k >= 1 and !win[p+1][s-k]
//
// (k with s-k == 0 is never taken: that move loses immediately, so it is just
// excluded from the successors rather than treated as a losing successor.)
// Answer is win[0][S].
//
// The naive form costs O(S * 2n * M). It collapses to O(S * 2n): the successors
// of (p, s) are exactly s' in [max(1, s-M[p]), s-1] at seat p+1, so a losing
// successor exists iff the LARGEST losing s' < s at seat p+1 is >= s-M[p].
// Keeping only lastLose[p] = greatest s' < s with win[p][s'] false makes the
// whole board history unnecessary, so memory is O(n) as well. All of the
// current row is computed before lastLose is updated, so lastLose never leaks
// a value from the row being computed.
//
// Statement reading: turn order is the fixed cycle 1,2,...,2n (confirmed on the
// discuss board), a player must remove at least one stone, and taking the last
// stone loses. S < 2^13 and M <= 16, so everything fits in int.

#include <cstdio>

int main() {
    int n;
    int M[20], lastLose[20], cur[20];
    while (scanf("%d", &n) == 1 && n != 0) {
        int S, P = 2 * n;
        scanf("%d", &S);
        int i;
        for (i = 0; i < P; ++i) { scanf("%d", &M[i]); lastLose[i] = 0; }
        int ans = 0;
        for (int s = 1; s <= S; ++s) {
            for (int p = 0; p < P; ++p) {
                int lo = s - M[p];
                if (lo < 1) lo = 1;
                cur[p] = (lastLose[(p + 1) % P] >= lo) ? 1 : 0;
            }
            for (int p = 0; p < P; ++p) if (!cur[p]) lastLose[p] = s;
            ans = cur[0];
        }
        printf("%d\n", ans);
    }
    return 0;
}
