// POJ 3634 - Circle of Debt
// Model: gpt-6.1-sol
// Assign every non-unit coin directly to its final owner. A two-dimensional
// minimum-cost DP stores Alice's and Bob's assigned values in units of five;
// Cynthia's value follows from the processed total. Unit coins fill the exact
// remaining balances, moving only the excess coins of each original owner.
// Only net debts matter; a coin changing owners costs one regardless of value.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
static int dp[201][201], nextdp[201][201];
int main() {
    int tests; scanf("%d", &tests);
    const int val[6]={100,50,20,10,5,1};
    while(tests--) {
        int ab,bc,ca,cnt[3][6],target[3]={0,0,0};
        scanf("%d%d%d",&ab,&bc,&ca);
        for(int p=0;p<3;++p) for(int k=0;k<6;++k) {
            scanf("%d",&cnt[p][k]); target[p]+=cnt[p][k]*val[k];
        }
        target[0]+=ca-ab; target[1]+=ab-bc; target[2]+=bc-ca;
        if(target[0]<0 || target[1]<0 || target[2]<0) { puts("impossible"); continue; }
        int a=target[0]/5,b=target[1]/5,c=target[2]/5;
        memset(dp,63,sizeof(dp)); dp[0][0]=0;
        int sum=0;
        for(int k=0;k<5;++k) for(int p=0;p<3;++p) for(int q=0;q<cnt[p][k];++q) {
            int v=val[k]/5;
            memset(nextdp,63,sizeof(nextdp));
            for(int x=0;x<=a && x<=sum;++x) for(int y=0;y<=b && x+y<=sum;++y) {
                int z=sum-x-y,d=dp[x][y];
                if(z>c || d>100000) continue;
                if(x+v<=a) nextdp[x+v][y]=min(nextdp[x+v][y],d+(p!=0));
                if(y+v<=b) nextdp[x][y+v]=min(nextdp[x][y+v],d+(p!=1));
                if(z+v<=c) nextdp[x][y]=min(nextdp[x][y],d+(p!=2));
            }
            sum+=v; memcpy(dp,nextdp,sizeof(dp));
        }
        int answer=100000;
        for(int x=0;x<=a;++x) for(int y=0;y<=b;++y) {
            int z=sum-x-y;
            if(z<0 || z>c || dp[x][y]>100000) continue;
            int rem[3]={target[0]-5*x,target[1]-5*y,target[2]-5*z};
            int moves=dp[x][y];
            for(int p=0;p<3;++p) moves+=max(0,cnt[p][5]-rem[p]);
            answer=min(answer,moves);
        }
        if(answer==100000) puts("impossible"); else printf("%d\n",answer);
    }
    return 0;
}
