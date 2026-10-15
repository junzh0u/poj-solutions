// POJ 1839 - Cattle
// Model: gpt-6.1-sol
// Precompute survivors in each possible car by propagating deaths, then
// partition the entire queue with dynamic programming. Attackers cannot die.
// As clarified on the board, protection belongs to each (attacker, victim)
// pair; all animals must be loaded. Mutually protecting cycles stay alive.
#include <cstdio>
#include <algorithm>
using namespace std;
static int protector[1001][1001], score[1001][21];
static int dp[2][1001];
int main() {
    int n,k,m,d;
    if(scanf("%d%d%d",&n,&k,&m)!=3) return 0;
    scanf("%d",&d);
    for(int i=0;i<d;++i) {
        int a,b,c; scanf("%d%d%d",&a,&b,&c);
        protector[a][b]=c;
    }
    for(int l=1;l<=n;++l) for(int len=1;len<=m && l+len-1<=n;++len) {
        int r=l+len-1;
        unsigned int need[20]={0}, dead=0;
        for(int a=l;a<=r;++a) for(int b=l;b<=r;++b) {
            int c=protector[a][b];
            if(!c) continue;
            if(c<l || c>r) dead |= 1U<<(b-l);
            else need[b-l] |= 1U<<(c-l);
        }
        bool changed=true;
        while(changed) {
            changed=false;
            for(int b=0;b<len;++b) if(!(dead&(1U<<b)) && (need[b]&dead)) {
                dead |= 1U<<b; changed=true;
            }
        }
        int count=0;
        for(int b=0;b<len;++b) if(!(dead&(1U<<b))) ++count;
        score[l][len]=count;
    }
    for(int i=0;i<=n;++i) dp[0][i]=-1000000;
    dp[0][0]=0;
    k=min(k,n);
    for(int cars=1;cars<=k;++cars) {
        int cur=cars&1, prev=cur^1;
        for(int i=0;i<=n;++i) dp[cur][i]=dp[prev][i];
        for(int i=1;i<=n;++i) for(int len=1;len<=m && len<=i;++len)
            dp[cur][i]=max(dp[cur][i],dp[prev][i-len]+score[i-len+1][len]);
    }
    printf("%d\n",dp[k&1][n]);
    return 0;
}
