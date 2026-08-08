// POJ 2189 - Romeo Meets Juliet
// Model: claude-opus-5
//
// The fence has P posts and therefore P-1 unit gaps; gap i is the stretch
// between post i and post i+1.  A cow reported as X grazes in gap X, and
// several cows may share a gap, so the input is a multiset over gaps.  The
// answer is the maximum number of consecutive gaps whose cow counts sum to
// at most C -- a longest-window-with-bounded-sum problem, solved with two
// pointers over cnt[1..P-1] in O(N+P).
//
// Reading notes: the region is measured in gaps, not posts (the sample's
// answer 3 rules out the +1 off-by-one), and the bound is "no more than C",
// i.e. sum <= C.  An empty region is legal, so when every single gap already
// holds more than C cows the answer is 0.  The discuss board's "the data is
// broken" post supplies what it calls judge case 7 (N=100, P=10, C=7, answer
// 1); this reading reproduces it -- gap 4 holds 6 cows and every other gap
// holds more than 7 -- which is what makes that post evidence rather than a
// warning.

#include <cstdio>

int cnt[1005];

int main() {
    int n, p, c;
    if (scanf("%d %d %d", &n, &p, &c) != 3) return 0;
    for (int i = 0; i < n; i++) {
        int x;
        if (scanf("%d", &x) != 1) break;
        if (x >= 1 && x <= 1000) cnt[x]++;
    }

    int gaps = p - 1;              /* gaps are indexed 1..p-1 */
    int best = 0, sum = 0, lo = 1;
    for (int hi = 1; hi <= gaps; hi++) {
        sum += cnt[hi];
        while (sum > c) {
            sum -= cnt[lo];
            lo++;
        }
        if (hi - lo + 1 > best) best = hi - lo + 1;
    }

    printf("%d\n", best);
    return 0;
}
