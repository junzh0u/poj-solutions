// POJ 3716 - Panda's Birthday Present
// Model: claude-opus-5
//
// Four dice; every one of the 6 sides of every die is independently painted
// red or blue with probability 1/2.  A round rolls all four dice and scores
// the number of red faces up.  Given the scores p and q of rounds 1 and 2,
// report E[score of round 3].
//
// Key point (the one the discuss board says people miss): the 7^4 vectors of
// per-die red-counts are NOT equally likely.  A die showing r red sides has
// prior weight C(6,r)/2^6, so the posterior is
//     P(r) proportional to  prod_i C(6,r_i) * L(p|r) * L(q|r),
// with L(k|r) the probability that exactly k of the four dice come up red.
// The answer is then sum_i r_i/6 under that posterior.
//
// Enumerating all 7^4 = 2401 states in exact rationals over the whole input
// domain (all 25 pairs 0<=p,q<=4) collapses to the closed form
//     E = (p + q + 10) / 7.
// Both samples agree (2 2 -> 14/7 = 2.000, 4 4 -> 18/7 = 2.571).  With a
// uniform prior instead of the binomial one, 20 of the 25 pairs change
// (4 4 would give 3.231), so the weighting is genuinely load-bearing.
//
// Rounding: the answer is always N/7 with N = p+q+10 in [10,18], and none of
// those nine values sits within 1/14 of a half-way point at three decimals.
// So the rounding is unambiguous and the whole computation is done in exact
// integers -- round(1000*N/7) = (2000*N + 7) / 14 -- with no floating point
// and no printf("%f") at all.  That matters here: the board reports the
// float-based one-liner passing under C++ (language 4) but getting Wrong
// Answer under G++ (language 0), and pure integer output cannot differ
// between the two compilers' formatting or libm.  Submitted under 4.

#include <cstdio>

int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases-- > 0) {
        int p, q;
        if (scanf("%d %d", &p, &q) != 2) break;
        int n = p + q + 10;          /* answer is exactly n/7 */
        int scaled = (2000 * n + 7) / 14;   /* round(1000*n/7), no ties */
        printf("%d.%03d\n", scaled / 1000, scaled % 1000);
    }
    return 0;
}
