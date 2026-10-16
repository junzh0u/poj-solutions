// POJ 1710 - Magic of David Copperfield
// Model: gpt-6-astra
// Remove diagonals from bottom right to top left. Every turn has an
// odd move count, so the finger's checkerboard color alternates and
// is opposite the diagonal removed. Remaining cells stay connected.
// Distinct move counts range from 2*N+1 to 6*N-5, within the limits.
#include <cstdio>
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int k = 2*n+1;
    for (int d = 2*n-2; d >= 1; --d, k += 2) {
        printf("%d", k);
        for (int r = 0; r < n; ++r) {
            int c = d-r;
            if (c >= 0 && c < n) printf(" %d", r*n+c+1);
        }
        printf("\n");
    }
    return 0;
}
