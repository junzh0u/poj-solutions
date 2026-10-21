// POJ 3871 - Java Certification
// Model: gpt-6-astra
// Enumerate a lower bound for category sizes and binary-search their spread.
// A bitset DP tracks total questions and incorrect answers, with reconstruction.
// Percentage ties are rounded to even using exact integer remainders.
#include <cstdio>
#include <bitset>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;
int n,m,w;
vector<pair<int,int> > opt[10];
bitset<101> dp[11][101];
int ansn[10],answ[10];
bool feasible(int d, bool save) {
    for(int lo=max(1,(n+m-1)/m-d);lo<=n/m;++lo) {
        int hi=min(n,lo+d);
        for(int i=0;i<=m;++i) for(int s=0;s<=n;++s) dp[i][s].reset();
        dp[0][0].set(0);
        for(int i=0;i<m;++i) {
            for(size_t j=0;j<opt[i].size();++j) {
                int a=opt[i][j].first,b=opt[i][j].second;
                if(a<lo || a>hi || b>w) continue;
                for(int s=i*lo;s<=min(n-a,i*hi);++s) {
                    int t=s+a;
                    if(t+(m-i-1)*lo>n || t+(m-i-1)*hi<n) continue;
                    dp[i+1][t] |= dp[i][s]<<b;
                }
            }
        }
        if(!dp[m][n][w]) continue;
        if(save) {
            int s=n,t=w;
            for(int i=m-1;i>=0;--i) {
                for(size_t j=0;j<opt[i].size();++j) {
                    int a=opt[i][j].first,b=opt[i][j].second;
                    if(a>=lo && a<=hi && a<=s && b<=t && dp[i][s-a][t-b]) {
                        ansn[i]=a;answ[i]=b;s-=a;t-=b;break;
                    }
                }
            }
        }
        return true;
    }
    return false;
}
int main() {
    int k;
    if(scanf("%d%d%d",&k,&n,&m)!=3) return 0;
    w=n-k;
    for(int i=0;i<m;++i) {
        int p;scanf("%d",&p);
        for(int a=1;a<=n;++a) for(int b=0;b<=a;++b) {
            int q=100*(a-b)/a,r=100*(a-b)%a;
            if(2*r>a || (2*r==a && q%2)) ++q;
            if(q==p) opt[i].push_back(make_pair(a,b));
        }
    }
    int low=0,high=n-1;
    while(low<high) {
        int mid=(low+high)/2;
        if(feasible(mid,false)) high=mid;else low=mid+1;
    }
    feasible(low,true);
    for(int i=0;i<m;++i) printf("%d %d\n",answ[i],ansn[i]);
    return 0;
}
