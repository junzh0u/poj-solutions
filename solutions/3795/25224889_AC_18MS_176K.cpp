// POJ 3795 - The Two Note Rag
// Model: gpt-6.1-sol
// Choose the unique 1/2 suffix divisible by 2^R, then lift its discrete
// logarithm modulo 5^R. Since 2 is primitive modulo powers of 5, each
// level has exactly one of five possible exponent lifts. CRT gives the
// desired decimal suffix. All moduli fit in 64 bits; products use doubling.
#include <iostream>
using namespace std;
typedef long long Int;
Int mul(Int a, Int b, Int m) {
    Int c=0;
    while(b) { if(b&1) c=(c+a)%m; a=(a+a)%m; b/=2; }
    return c;
}
Int power(Int e, Int m) {
    Int a=2, b=1;
    while(e) { if(e&1) b=mul(b,a,m); a=mul(a,a,m); e/=2; }
    return b;
}
int main() {
    int digit[20]; Int ans[21];
    const Int mask=1<<20;
    Int suffix=0, place=1;
    for(int i=0;i<20;++i) {
        Int mod=Int(1)<<(i+1);
        digit[i]=((suffix+place)%mod==0)?1:2;
        suffix=(suffix+digit[i]*place)%mask;
        place=place*10%mask;
    }
    Int mod=1, period=1, k=0;
    for(int r=1;r<=20;++r) {
        mod*=5;
        Int target=0;
        for(int i=r-1;i>=0;--i) target=(target*10+digit[i])%mod;
        int choices=(r==1)?4:5;
        for(int j=0;j<choices;++j) {
            if(power(k+j*period,mod)==target) { k+=j*period; break; }
        }
        period=(r==1)?4:period*5;
        ans[r]=k;
        while(ans[r]<r) ans[r]+=period;
    }
    int n,r; cin>>n;
    for(int i=1;i<=n;++i) { cin>>r; cout<<i<<' '<<r<<' '<<ans[r]<<'\n'; }
    return 0;
}
