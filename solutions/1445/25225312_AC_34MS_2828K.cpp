// POJ 1445 - Random number
// Model: gpt-6.1-sol
// Count suffixes with a lower-bound DP, then unrank lexicographically.
// Apply the pictured G formula by inclusive midpoint splits; exhausting
// the finite binary string selects the current lower endpoint.
#include <iostream>
#include <string>
using namespace std;
struct Big {
    enum { L=16, BASE=100000000 };
    int d[L];
    Big(int x=0) { for(int i=0;i<L;++i)d[i]=0; d[0]=x; }
};
Big add(const Big&a,const Big&b) {
    Big c; int carry=0;
    for(int i=0;i<Big::L;++i) { int x=a.d[i]+b.d[i]+carry; c.d[i]=x%Big::BASE; carry=x/Big::BASE; }
    return c;
}
Big half(const Big&a) {
    Big c; int carry=0;
    for(int i=Big::L-1;i>=0;--i) { int x=carry*Big::BASE+a.d[i]; c.d[i]=x/2; carry=x%2; }
    return c;
}
int cmp(const Big&a,const Big&b) {
    for(int i=Big::L-1;i>=0;--i) if(a.d[i]!=b.d[i])return a.d[i]<b.d[i]?-1:1;
    return 0;
}
Big sub(const Big&a,const Big&b) {
    Big c; int borrow=0;
    for(int i=0;i<Big::L;++i) { int x=a.d[i]-b.d[i]-borrow; borrow=x<0; if(borrow)x+=Big::BASE; c.d[i]=x; }
    return c;
}
Big dp[202][202];
int main() {
    int m,n; string bits;
    if(!(cin>>m>>n>>bits))return 0;
    for(int v=1;v<=m+1;++v)dp[n+1][v]=Big(1);
    for(int i=n;i>=1;--i)
        for(int v=m;v>=i;--v)
            dp[i][v]=add(dp[i][v+1],dp[i+1][v>i?v:i+1]);
    Big a(1), b=dp[1][1];
    for(unsigned k=2;k<bits.size() && cmp(a,b)<0;++k) {
        Big mid=half(add(a,b));
        if(bits[k]=='0')b=mid; else a=add(mid,Big(1));
    }
    Big rank=a; int prev=1;
    for(int i=1;i<=n;++i) {
        int v=prev>i?prev:i;
        for(;v<=m;++v) {
            Big count=dp[i+1][v>i?v:i+1];
            if(cmp(rank,count)<=0)break;
            rank=sub(rank,count);
        }
        if(i>1)cout<<' ';
        cout<<v; prev=v;
    }
    cout<<'\n';
    return 0;
}
