// POJ 1822 - Fence2
// Model: gpt-6.1-sol
// Choose the first shift as an unordered subset, then arrange the rest.
// Shift order matters; worker order within a shift does not.
// Base-10000 integers retain the exact count without 64-bit arithmetic.
#include <cstdio>
#include <cstring>
struct Big {
    int d[32];
    Big(int v=0) { std::memset(d,0,sizeof(d)); d[0]=v; }
};
Big add(const Big &a,const Big &b) {
    Big c; int carry=0;
    for(int i=0;i<32;++i) {
        int v=a.d[i]+b.d[i]+carry; c.d[i]=v%10000; carry=v/10000;
    }
    return c;
}
Big mul(const Big &a,const Big &b) {
    Big c;
    for(int i=0;i<32;++i) {
        int carry=0;
        for(int j=0;i+j<32;++j) {
            int v=c.d[i+j]+a.d[i]*b.d[j]+carry;
            c.d[i+j]=v%10000; carry=v/10000;
        }
    }
    return c;
}
Big choose[51][51], ways[51];
int main() {
    int n,k;
    if(std::scanf("%d%d",&n,&k)!=2) return 0;
    choose[0][0]=Big(1);
    for(int i=1;i<=n;++i) {
        choose[i][0]=Big(1);
        for(int j=1;j<=i;++j) choose[i][j]=add(choose[i-1][j-1],choose[i-1][j]);
    }
    ways[0]=Big(1);
    for(int i=1;i<=n;++i)
        for(int j=1;j<=k && j<=i;++j)
            ways[i]=add(ways[i],mul(choose[i][j],ways[i-j]));
    int p=31;
    while(p>0 && ways[n].d[p]==0) --p;
    std::printf("%d",ways[n].d[p]);
    while(--p>=0) std::printf("%04d",ways[n].d[p]);
    std::puts("");
    return 0;
}
