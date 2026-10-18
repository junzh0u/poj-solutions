// POJ 3566 - Building for UN
// Model: gpt-6-astra
// Two n-by-n floors: country i owns row i below and column i above.
// Its two strips meet at (i,i); every pair meets vertically at (i,j).
// Exactly 2*n*n offices suffice, at most 5000, with no empty offices.
#include <cstdio>
int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;
    const char *letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::printf("2 %d %d\n", n, n);
    for (int floor = 0; floor < 2; ++floor) {
        if (floor) std::putchar('\n');
        for (int row = 0; row < n; ++row) {
            for (int col = 0; col < n; ++col)
                std::putchar(letters[floor ? col : row]);
            std::putchar('\n');
        }
    }
    return 0;
}
