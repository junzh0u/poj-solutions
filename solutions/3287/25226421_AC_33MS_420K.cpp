// POJ 3287 - The Trip, 2007
// Model: gpt-6-astra
// Sort and distribute cyclically over the maximum equal-size multiplicity k.
// Equal sizes occupy at most k consecutive slots, hence never share a piece.
// Piece sizes differ by at most one, attaining the lower bound ceil(n/k)
// for the maximum number of nested bags. Output order within a piece is free.
#include <cstdio>
#include <algorithm>
#include <vector>
int main() {
    int n;
    bool first = true;
    while (std::scanf("%d", &n) == 1 && n) {
        std::vector<int> a(n);
        for (int i = 0; i < n; ++i) std::scanf("%d", &a[i]);
        std::sort(a.begin(), a.end());
        int k = 1, run = 1;
        for (int i = 1; i < n; ++i) {
            run = a[i] == a[i-1] ? run + 1 : 1;
            k = std::max(k, run);
        }
        if (!first) std::puts("");
        first = false;
        std::printf("%d\n", k);
        for (int i = 0; i < k; ++i) {
            for (int j = i; j < n; j += k) {
                if (j != i) std::printf(" ");
                std::printf("%d", a[j]);
            }
            std::puts("");
        }
    }
    return 0;
}
