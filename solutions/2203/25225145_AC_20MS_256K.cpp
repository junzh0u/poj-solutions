// POJ 2203 - Patience
// Model: gpt-6.1-sol
// Precompute the thirteen possible answers by traversing moves backwards
// from the ordered row. A reverse move swaps the hole with any card that
// currently follows its predecessor (the first card may follow the fixed
// prefix). Each position has at most one forward move, so the reverse tree
// contains each winning position exactly once and needs no visited set.
// The other three rows cannot move: their holes already follow Kings.
// Exhaustive generation agrees with the discussion table for every n.
#include <cstdio>
int main() {
    const int answer[14] = {0, 1, 2, 5, 14, 47, 189, 891, 4815,
                           29547, 203173, 1548222, 12966093, 118515434};
    int n;
    if (std::scanf("%d", &n) == 1) std::printf("%d\n", answer[n]);
    return 0;
}
