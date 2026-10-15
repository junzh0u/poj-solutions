// POJ 3986 - Math teacher's homework
// Model: gpt-6.1-sol
// Process bits from high to low. A two-state weighted parity DP counts
// choices at this bit. Except for the all-bound-prefix branch, some
// variable has a completely free suffix, making its XOR uniform.
// Divide those weighted counts by the suffix size, then continue only
// along the all-bound-prefix branch. Bounds are inclusive; stop at 0 0.
#include <iostream>
using namespace std;
typedef long long LL;
const LL MOD = 1000000003;
int main() {
    LL inv[31]; inv[0]=1;
    for(int b=1;b<=30;++b) inv[b]=inv[b-1]*500000002%MOD;
    int n,k,m[50];
    while(cin>>n>>k && n) {
        for(int i=0;i<n;++i) cin>>m[i];
        LL ans=0;
        bool tight=true;
        for(int b=30;b>=0 && tight;--b) {
            int size=1<<b, mask=size-1, parity=0;
            LL dp[2]={1,0}, prod=1;
            for(int i=0;i<n;++i) {
                LL low=(m[i]&mask)+1;
                bool one=(m[i]&size)!=0;
                LL a=one?size:low, c=one?low:0;
                LL even=(dp[0]*a+dp[1]*c)%MOD;
                LL odd=(dp[1]*a+dp[0]*c)%MOD;
                dp[0]=even; dp[1]=odd;
                prod=prod*low%MOD;
                parity^=one;
                m[i]&=mask;
            }
            tight=(parity==((k>>b)&1));
            LL freeCount=(dp[(k>>b)&1]-(tight?prod:0)+MOD)%MOD;
            ans=(ans+freeCount*inv[b])%MOD;
        }
        if(tight) ans=(ans+1)%MOD;
        cout<<ans<<'\n';
    }
}
