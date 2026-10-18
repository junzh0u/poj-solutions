// POJ 3091 - Triangular N-Queens Problem
// Model: gpt-6-astra
// Put k=(2*n+1)/3 queens in the bottom k rows, starting at row s.
// Use odd columns for the first s queens, then even columns. Rows and
// columns are distinct; row-column lies in disjoint ranges for the two
// groups. Every cell is inside the triangle, giving the stated optimum.
// Different valid witnesses are allowed; attacks preserve row, column,
// or row-column (there is no fourth, square-board diagonal direction).
#include <cstdio>
int main() {
    int cases;
    if (std::scanf("%d", &cases) != 1) return 0;
    for (int t = 1; t <= cases; ++t) {
        int n;
        std::scanf("%d", &n);
        int k = (2*n+1)/3, s = n-k+1;
        std::printf("%d %d %d\n", t, n, k);
        for (int i = 0; i < k; ++i) {
            int col = i < s ? 2*i+1 : 2*(i-s)+2;
            if (i % 8) std::printf(" ");
            std::printf("[%d,%d]", s+i, col);
            if (i % 8 == 7 || i == k-1) std::printf("\n");
        }
        std::printf("\n");
    }
    return 0;
}
