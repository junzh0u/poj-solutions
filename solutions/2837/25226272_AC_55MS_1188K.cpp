// POJ 2837 - Silver Matrix
// Model: gpt-6-astra
// Put 1 on the diagonal. For i != j, d = i XOR j identifies a
// perfect matching: every vertex has exactly one partner for each d.
// Give its two directed entries 2*d and 2*d+1. Thus row i together
// with column i contains 1 and every pair 2*d,2*d+1 exactly once.
// The shared diagonal is counted once; any valid matrix is allowed.
// O(n*n) time and O(1) auxiliary space, with n = 2^K <= 512.
#include <cstdio>

int main() {
    int k;
    if (std::scanf("%d", &k) != 1) return 0;
    const int n = 1 << k;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int value = i == j ? 1 : 2 * (i ^ j) + (i > j);
            std::printf("%d%c", value, j + 1 == n ? '\n' : ' ');
        }
    }
    return 0;
}
