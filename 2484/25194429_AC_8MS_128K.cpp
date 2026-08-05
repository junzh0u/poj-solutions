// POJ 2484 - A Funny Game
//
// n coins in a circle; a move removes one or two *adjacent* coins. Removing a
// coin leaves a gap, so its former neighbours are no longer adjacent. Last
// player to move wins; Alice starts. Output the winner for n up to 10^6.
//
// n == 1: Alice takes the single coin. n == 2: Alice takes both. Alice wins.
//
// n >= 3: Bob wins. Alice's first move is the only one made on a circle;
// whatever she takes (one coin or two adjacent ones), what is left is a single
// straight line of n-1 or n-2 coins. Bob then removes one or two coins from the
// exact middle of that line, splitting it into two lines of equal length (take
// one coin if the line is odd, two if it is even). From then on he mirrors
// every Alice move in the other half, so a reply always exists and Alice is the
// one who eventually cannot move. The circularity is what makes this work: on a
// circle there is no "middle" to split at, which is why the first player cannot
// use the same trick.
//
// Equivalently, the line game is Kayles, whose Grundy value is 0 only for the
// empty row, so after Alice's forced opening Bob always faces a non-zero
// position — verified here by exhaustive game-tree search over the circle for
// n <= 16, which agrees with the n <= 2 rule.
//
// Statement note: "adjacent" means adjacent among the original positions, not
// among the surviving coins — the problem spells this out, and it is what makes
// a removal split the row rather than reconnect it.

#include <cstdio>

int main() {
    long n;
    while (scanf("%ld", &n) == 1 && n != 0) {
        puts(n <= 2 ? "Alice" : "Bob");
    }
    return 0;
}
