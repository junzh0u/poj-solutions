// POJ 3475 - Paper-, er, Transcript-Folding Game
// Model: claude-opus-5
//
// Envelope a x b, transcript c x d. Each fold halves one of the transcript's
// two sides, so after i folds across the c-side and j across the d-side it
// measures exactly c/2^i by d/2^j. It must sit axis-aligned in the envelope, so
// only two placements exist; within one placement the two sides are
// independent, and the answer is
//     min( f(c,a)+f(d,b), f(c,b)+f(d,a) )
// where f(x,y) is the least k with x/2^k <= y.
//
// Ambiguity 1 - halving an odd side. The sides are integers but a physical
// half of 5 is 2.5, not 2. Against an integer envelope side y this is exactly
// ceiling halving, since y integer gives x/2 <= y <=> ceil(x/2) <= y, and
// ceil(ceil(x/2)/2) = ceil(x/4) so it iterates correctly. Flooring instead
// (x>>1, as several discuss-board posts do) reproduces the official sample -
// both its cases have power-of-two sides - but answers 1 instead of 2 on
// "2 1 5 1". Verified against an exact-rational reference over all 14^4 inputs.
//
// Ambiguity 2 - "do not exceed" is <=, not <: sample case 1 folds 1x4 into a
// 1x2 envelope in one fold, landing on the boundary exactly.
//
// 64-bit throughout: sides reach 2^31-1, and a board post reports the real data
// overflowing signed int. iostream rather than scanf("%lld"), which is not
// dependable on the judge's MinGW runtime. Input is read to EOF as stated.

#include <iostream>

typedef long long ll;

// least k with x/2^k <= y, halving exactly (ceiling on the integer sides)
static int folds(ll x, ll y) {
    int k = 0;
    while (x > y) { x = (x + 1) / 2; ++k; }
    return k;
}

int main() {
    std::ios::sync_with_stdio(false);
    ll a, b, c, d;
    while (std::cin >> a >> b >> c >> d) {
        int p = folds(c, a) + folds(d, b);
        int q = folds(c, b) + folds(d, a);
        std::cout << (p < q ? p : q) << "\n";
    }
    return 0;
}
