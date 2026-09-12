// POJ 3712 - Edges and More Edges
// Model: claude-sonnet-5
//
// Maximum edges in an undirected simple graph on n vertices with no
// k-matching (matching number < k), via the Erdos-Gallai matching-number
// theorem. Let s = k-1 (allowed matching number). If floor(n/2) <= s, the
// complete graph already has matching number <= s, so answer = C(n,2).
// Otherwise (n >= 2k) the extremal graph is one of two constructions:
//   - K_{2s+1} plus n-2s-1 isolated vertices: C(2s+1,2) edges.
//   - K_s joined completely to the remaining n-s vertices (which form an
//     independent set): C(s,2) + s*(n-s) edges.
// Answer is the max of the two. No statement ambiguity beyond the
// definition of k-matching (the board's only traffic), resolved by
// matching the given sample (1000 1 -> 0, 500 2 -> 499).
#include <cstdio>

int main() {
    int n, k;
    while (scanf("%d %d", &n, &k) == 2) {
        if (n == 0 && k == 0) break;
        int s = k - 1;
        int ans;
        if (n / 2 <= s) {
            ans = n * (n - 1) / 2;
        } else {
            int a = (2 * s + 1) * (2 * s) / 2;
            int b = s * (s - 1) / 2 + s * (n - s);
            ans = (a > b) ? a : b;
        }
        printf("%d\n", ans);
    }
    return 0;
}
