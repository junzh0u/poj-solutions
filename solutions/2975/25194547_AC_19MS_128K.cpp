// POJ 2975 - Nim
//
// Count the winning moves from a given Nim position.
//
// A Nim position is losing (for the player to move) exactly when the xor of all
// pile sizes is 0, so a winning move is one that leaves xor 0.  Let x be the xor
// of every pile.  If x == 0 the position is already losing and there is no
// winning move, so the answer is 0.  Otherwise, a move touches exactly one pile
// i, and to zero the xor that pile must end up holding t = x ^ k[i] stones —
// the target is forced, so each pile contributes at most one winning move.
// A move must strictly *remove* stones, so pile i counts only when
// (x ^ k[i]) < k[i].  That test is also what makes the count exact: t is never
// negative, and t == k[i] is impossible here because x != 0.  O(n) per case.
//
// Input is a stream of test cases terminated by a line holding a single 0, which
// is not processed.  Piles are positive (1 <= ki <= 1e9), so a single pile always
// has exactly one winning move: take the whole pile.  ki fits in 32 bits;
// unsigned int is used so the xor and the comparison are both well defined.

#include <cstdio>

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        static unsigned int k[1005];
        unsigned int x = 0u;
        for (int i = 0; i < n; ++i) {
            scanf("%u", &k[i]);
            x ^= k[i];
        }
        int cnt = 0;
        if (x != 0u) {
            for (int i = 0; i < n; ++i) {
                if ((x ^ k[i]) < k[i]) ++cnt;
            }
        }
        printf("%d\n", cnt);
    }
    return 0;
}
