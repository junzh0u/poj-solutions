// POJ 3263 - Tallest Cow
// Model: claude-sonnet-5
// Approach: Cow I has the max height H. Each constraint "A sees B" means the
// cow at min(A,B) is the strict maximum over the open range between the two
// indices (every cow strictly between them is shorter than it, and the far
// endpoint is at least as tall). So every cow strictly between min(A,B) and
// max(A,B) must be strictly shorter than H by at least the number of such
// intervals that cover it. Model this as a difference array: for the
// normalized pair (a,b) with a<b, bump diff[a+1]++ and diff[b]-- (only when
// b-a>=2, i.e. there's actually a cow strictly between them), then each
// cow's height is H minus the prefix-summed coverage count.
// Ambiguity/pitfall: the discuss board and local testing both confirm the
// input can contain duplicate (A,B) pairs (including reordered duplicates
// like (17,34) and (34,17)); counting a duplicate again would over-reduce a
// cow's height. Constraints are deduplicated via a std::set of normalized
// pairs before building the difference array.
#include <cstdio>
#include <set>
#include <utility>

int main() {
    int n, I, H, r;
    if (std::scanf("%d %d %d %d", &n, &I, &H, &r) != 4) return 0;

    std::set<std::pair<int, int> > seen;
    static int diff[10005];
    for (int i = 0; i <= n + 2; ++i) diff[i] = 0;

    for (int k = 0; k < r; ++k) {
        int a, b;
        std::scanf("%d %d", &a, &b);
        if (a > b) { int t = a; a = b; b = t; }
        std::pair<int, int> p(a, b);
        if (seen.find(p) != seen.end()) continue;
        seen.insert(p);
        if (b - a >= 2) {
            diff[a + 1]++;
            diff[b]--;
        }
    }

    int cnt = 0;
    for (int i = 1; i <= n; ++i) {
        cnt += diff[i];
        std::printf("%d\n", H - cnt);
    }

    (void)I;
    return 0;
}
