// POJ 3431 - Texture Tile
// Model: gpt-6-astra
// Try side lengths in descending order. For each separation d, compute
// equal runs down columns d apart, then across rows d apart. Both runs
// must cover d+1 pixels. This is exact O(N^3) time and O(N^2) space.
// Coordinates are one-based row, column, as in the second sample.
#include <cstdio>
static int a[371][371], v[371][371];
int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;
    for (int i=0;i<n;++i)
        for (int j=0;j<n;++j) std::scanf("%d", &a[i][j]);
    for (int d=n-1;d>0;--d) {
        for (int i=n-1;i>=0;--i)
            for (int j=0;j+d<n;++j)
                v[i][j] = a[i][j]==a[i][j+d] ? v[i+1][j]+1 : 0;
        for (int i=0;i+d<n;++i) {
            int run=0;
            for (int j=n-1;j>=0;--j) {
                run = a[i][j]==a[i+d][j] ? run+1 : 0;
                if (j+d<n && run>=d+1 && v[i][j]>=d+1) {
                    std::printf("%d %d %d\n",i+1,j+1,d+1);
                    return 0;
                }
            }
        }
    }
    std::puts("1 1 1");
    return 0;
}
