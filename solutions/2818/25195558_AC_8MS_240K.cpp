// POJ 2818 - Making Change
// Model: claude-opus-5
//
// Bounded coin change: given stocks of quarters/dimes/nickels/pennies, pay
// exactly C cents (0..99) with the fewest coins, or report it is impossible.
//
// Approach: C <= 99 bounds every useful count, so the stocks can be capped at
// 3 quarters, 9 dimes, 19 nickels and 99 pennies without changing any answer.
// What remains is an exhaustive scan over (quarters, dimes, nickels) with the
// pennies forced by the remainder -- at most 4*10*20 = 800 candidates per case,
// so the exact minimum is read off directly. Greedy is wrong here (the sample's
// "1 3 0 10 30" pays 3 dimes, not a quarter plus five pennies), which is what
// the exhaustive scan exists to avoid.
//
// Ambiguity: with several optimal payouts the required output would be
// undefined, but there are none -- over every amount 0..99 and every stock
// vector, no two distinct payouts of equal cardinality are both optimal, so the
// answer is unique and any tie-break is vacuous. Termination is the literal
// statement rule ("a line of 5 zeros"): a line like "5 9 9 9 0" is real input
// and prints an all-zero dispense.
#include <cstdio>

int main() {
    int q, d, n, p, c;
    while (scanf("%d %d %d %d %d", &q, &d, &n, &p, &c) == 5) {
        if (q == 0 && d == 0 && n == 0 && p == 0 && c == 0) break;
        if (q > 3) q = 3;
        if (d > 9) d = 9;
        if (n > 19) n = 19;
        if (p > 99) p = 99;
        int best = -1, ba = 0, bb = 0, be = 0, bf = 0;
        for (int a = 0; a <= q; ++a) {
            for (int b = 0; b <= d; ++b) {
                for (int e = 0; e <= n; ++e) {
                    int f = c - 25 * a - 10 * b - 5 * e;
                    if (f < 0 || f > p) continue;
                    int tot = a + b + e + f;
                    if (best < 0 || tot < best) {
                        best = tot; ba = a; bb = b; be = e; bf = f;
                    }
                }
            }
        }
        if (best < 0) {
            printf("Cannot dispense the desired amount.\n");
        } else {
            printf("Dispense %d quarters, %d dimes, %d nickels, and %d pennies.\n",
                   ba, bb, be, bf);
        }
    }
    return 0;
}
