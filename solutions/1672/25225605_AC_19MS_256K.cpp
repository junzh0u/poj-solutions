// POJ 1672 - Fully Diversified Sequences of Sets
// Model: gpt-6-astra
// The total incidence is even exactly when n is 0 or 3 modulo 4.
// Start with the three-set construction or the empty construction.
// Extend by four: add all new elements to old sets, then four sets
// with new-element multiplicities 1,2,3,4. This attains the lower
// bound n, since element n must occur in n different sets.
// Emit blocks in lexicographic order, without storing the matrix.
#include <cstdio>
int n;
void row(int a, int b, int c, int d, int tail) {
    std::printf("%d", a);
    if (b) std::printf(" %d", b);
    if (c) std::printf(" %d", c);
    if (d) std::printf(" %d", d);
    for (int j=tail;j<=n;++j) std::printf(" %d",j);
    std::printf("\n");
}
int main() {
    while(std::scanf("%d",&n)==1 && n) {
        if(n%4==1 || n%4==2) { std::printf("0\n\n"); continue; }
        std::printf("%d\n",n);
        int k=0;
        if(n%4==3) {
            row(1,3,0,0,4); row(2,3,0,0,4); row(2,3,0,0,4);
            k=3;
        }
        for(;k<n;k+=4) {
            row(k+1,k+2,k+3,k+4,k+5);
            row(k+2,k+4,0,0,k+5);
            row(k+3,k+4,0,0,k+5);
            row(k+3,k+4,0,0,k+5);
        }
        std::printf("\n");
    }
    return 0;
}
