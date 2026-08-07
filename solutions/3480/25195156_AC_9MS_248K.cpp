// POJ 3480 - John
// Model: claude-opus-5
//
// Misere Nim ("anti-Nim"): players alternately remove >=1 M&Ms from a single
// color pile; the player who takes the LAST M&M loses. John moves first.
//
// Classic result: with piles a[1..n], the first player wins iff
//   (some a[i] > 1  and  xor of all a[i] != 0)   -- play it as normal Nim
//   or (all a[i] <= 1 and xor == 0)              -- i.e. an even number of 1s.
// Since 1 <= A_i, "all a[i] <= 1" means every pile is exactly 1, and then the
// xor is 0 exactly when the number of piles is even, so the first player wins
// iff n is even (each turn removes one pile; whoever faces the last single
// M&M must eat it and loses).
//
// Verified against an exhaustive game-tree search over every multiset of up to
// 5 piles with sizes up to 6, plus randomized trials with up to 7 piles.
//
// Ambiguity note: "eat several M&Ms of the same color" is read as "remove any
// positive number of M&Ms, all from one color" (a standard Nim move, the whole
// pile allowed). The sample is consistent with this: {3,5,1} has xor 7 != 0
// with a pile > 1 -> John; {1} is a single 1 (odd count) -> Brother.
//
// O(N) per test case. Input is T test cases, each an N followed by N values.

#include <cstdio>

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    while (T-- > 0) {
        int n;
        if (scanf("%d", &n) != 1) return 0;
        int x = 0, mx = 0;
        for (int i = 0; i < n; ++i) {
            int a;
            scanf("%d", &a);
            x ^= a;
            if (a > mx) mx = a;
        }
        bool johnWins = (mx > 1) ? (x != 0) : (x == 0);
        puts(johnWins ? "John" : "Brother");
    }
    return 0;
}
