// POJ 3777 - Snail Alice
// Model: gpt-6.1-sol
// Maintain a geometric block (r^k, sum_{i=0}^{k-1} r^i), r=-q.
// Double the length y2 times, then append z2 terms by binary powering.
// Negate the sum for even n. No division or coprimality assumption is needed.
#include <cstdio>
typedef long long Int;
Int mod;
struct Block { Int power, sum; Block(Int p=1, Int s=0):power(p),sum(s){} };
Block join(Block a, Block b) {
    return Block(a.power*b.power%mod,(a.sum+a.power*b.sum)%mod);
}
Int power(Int a, int e) {
    Int v=1%mod;
    for (;e;e/=2,a=a*a%mod) if(e%2) v=v*a%mod;
    return v;
}
int main() {
    int x,y,z,u,v;
    while(scanf("%d%d%d",&x,&y,&z)==3 && x!=-1) {
        int p;
        scanf("%d%d%d",&u,&v,&p); mod=p;
        Int q=(power(x,y)+z)%mod;
        Block one((mod-q)%mod,1%mod), a=one, tail;
        for(int i=0;i<u;++i) a=join(a,a);
        int t=v;
        for(;t;t/=2,one=join(one,one)) if(t%2) tail=join(tail,one);
        Int ans=join(a,tail).sum;
        bool odd=(u==0 ? (v%2==0) : (v%2!=0));
        if(!odd) ans=(mod-ans)%mod;
        printf("%d\n",(int)ans);
    }
    return 0;
}
