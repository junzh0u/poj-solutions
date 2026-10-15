// POJ 2764 - Unequalled Consumption
// Model: gpt-6.1-sol
// Let L be the lcm of the candy weights. For each residue modulo L,
// the number of compositions is a degree-(n-1) polynomial in the quotient.
// Coin-change DP supplies its Newton forward differences. These are
// nonnegative: writing each candy multiplicity modulo L/weight gives a
// nonnegative numerator over (1-z^L)^n, of degree less than n*L.
// Thus each residue is monotone and can be binary searched independently.
// Evaluate with capped integer binomial coefficients, avoiding both floating
// interpolation error and overflow. Distinct types remain distinct even
// when their weights agree; only positive total weights are eligible.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long Int;
int gcd(int a,int b) { return b ? gcd(b,a%b) : a; }
Int coef[2520][5];
int n,L;
bool enough(int r,Int k,Int p) {
    Int sum=0, choose=1;
    for(int d=0;d<n && d<=k;++d) {
        if(coef[r][d]) {
            if(choose>(p-1-sum)/coef[r][d]) return true;
            sum+=choose*coef[r][d];
        }
        if(d+1<n && d+1<=k) {
            Int factor=k-d, divisor=d+1;
            // Before saturation choose <= p, so p*divisor fits in 64 bits.
            if(k>=8 && choose>p*divisor/factor) choose=p;
            else choose=choose*factor/divisor;
        }
    }
    return false;
}
int main() {
    ios::sync_with_stdio(false);
    int setno=0;
    while(cin>>n && n) {
        int weights[5]; L=1;
        for(int i=0;i<n;++i) { cin>>weights[i]; L=L/gcd(L,weights[i])*weights[i]; }
        vector<Int> dp(n*L,0); dp[0]=1;
        for(int i=0;i<n;++i)
            for(int j=weights[i];j<n*L;++j) dp[j]+=dp[j-weights[i]];
        for(int r=0;r<L;++r) {
            Int a[5];
            for(int j=0;j<n;++j) a[j]=dp[r+j*L];
            for(int d=0;d<n;++d) {
                coef[r][d]=a[0];
                for(int j=0;j<n-d-1;++j) a[j]=a[j+1]-a[j];
            }
        }
        int q; cin>>q;
        cout<<"Set "<<++setno<<'\n';
        while(q--) {
            Int p; cin>>p;
            if(n==1) {
                if(p==1) cout<<weights[0]<<'\n';
                else cout<<"no candy for you\n";
                continue;
            }
            Int best=100*p+1;
            for(int r=0;r<L && r<best;++r) {
                Int low=(r==0 ? 1 : 0), high=(best-1-r)/L;
                if(high<low || !enough(r,high,p)) continue;
                while(low<high) {
                    Int mid=low+(high-low)/2;
                    if(enough(r,mid,p)) high=mid; else low=mid+1;
                }
                best=low*L+r;
            }
            cout<<best<<'\n';
        }
    }
    return 0;
}
