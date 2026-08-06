// POJ 2456 - Aggressive cows
//
// N stalls on a line at positions x1..xN (N <= 100,000, xi <= 1e9); place C
// cows (2 <= C <= N) so that the smallest distance between any two of them is
// as large as possible.
//
// Binary search on the answer. The predicate "can C cows be placed with every
// pair at least d apart" is monotone in d: if d works, so does anything
// smaller. Checking it is a greedy scan over the sorted stalls -- put the first
// cow in the leftmost stall and every later cow in the first stall at least d
// past the previously used one. Pushing each cow as far left as legal leaves
// the most room for the rest, so this greedy places the maximum number of cows
// for that d, and it is feasible exactly when that count reaches C.
//
// The answer lies in [0, x[N-1]-x[0]]: d = 0 is always feasible because C <= N,
// and no gap can exceed the span. Binary search that range, keeping the largest
// feasible d. O(N log N) for the sort plus O(N log(max x)) for the search.
//
// Notes on the statement:
//  - The input is not stated to be sorted, and is not; sorting first is what
//    makes the greedy valid.
//  - Stall positions may repeat. Duplicates are harmless: with d >= 1 a
//    duplicate simply fails the gap test, and if C exceeds the number of
//    distinct positions the search correctly bottoms out at 0.
//  - "Minimum distance between any two of them" only needs to be checked
//    between consecutive chosen stalls, since non-adjacent pairs are farther
//    apart on a line.
#include <cstdio>
#include <cstdlib>
#include <algorithm>

static int x[100005];
static int n, c;

/* how many cows can be placed if consecutive cows must be >= d apart */
static int fits(int d) {
    int cnt = 1;
    int last = x[0];
    for (int i = 1; i < n; ++i) {
        if (x[i] - last >= d) {
            ++cnt;
            last = x[i];
            if (cnt >= c) return 1;
        }
    }
    return cnt >= c;
}

int main() {
    while (scanf("%d %d", &n, &c) == 2) {
        for (int i = 0; i < n; ++i) scanf("%d", &x[i]);
        std::sort(x, x + n);
        /* answer in [0, x[n-1]-x[0]] : 0 always feasible since c <= n */
        int lo = 0, hi = x[n - 1] - x[0];
        while (lo < hi) {
            int mid = lo + (hi - lo + 1) / 2;
            if (fits(mid)) lo = mid; else hi = mid - 1;
        }
        printf("%d\n", lo);
    }
    return 0;
}
