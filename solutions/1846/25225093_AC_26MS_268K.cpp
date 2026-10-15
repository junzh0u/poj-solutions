// POJ 1846 - System
// Model: gpt-6.1-sol
// Every component is an undirected cycle of length at least three.
// Insert city n into an edge of a smaller system, or put it in a triangle:
// f[n] = (n-1)*f[n-1] + C(n-1,2)*f[n-3], with f[0]=1.
// Disconnected systems count too; loops and two-city cycles are forbidden.
// Base-10000 limbs keep all products within signed 32-bit arithmetic.
#include <cstdio>
#include <cstring>
struct Big {
    int d[50];
    Big() { std::memset(d, 0, sizeof(d)); }
};
Big f[101];
int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;
    f[0].d[0] = 1;
    for (int i=3; i<=n; ++i) {
        int carry=0;
        for (int j=0; j<50; ++j) {
            int v=f[i-1].d[j]*(i-1)
                +f[i-3].d[j]*((i-1)*(i-2)/2)+carry;
            f[i].d[j]=v%10000;
            carry=v/10000;
        }
    }
    int j=49;
    while (j>0 && f[n].d[j]==0) --j;
    std::printf("%d", f[n].d[j]);
    while (--j>=0) std::printf("%04d", f[n].d[j]);
    std::printf("\n");
    return 0;
}
