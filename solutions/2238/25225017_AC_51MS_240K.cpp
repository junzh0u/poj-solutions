// POJ 2238 - Computer Basketball Game
// Model: gpt-6.1-sol
// Enumerate the legal fixed skill assignments. For each pair of remaining
// scores, solve the two possession probabilities as simultaneous equations.
// Misses may preserve either possession without changing the scores; made
// baskets keep possession and reaching or exceeding the target wins.
#include <cstdio>
#include <algorithm>
using namespace std;
double dp[34][34][2];
double chance(int n, const int a[], const int b[]) {
    double s[2][2], stay[2], change[2], den[2];
    for (int k=0;k<2;++k) {
        const int *u=k?b:a, *v=k?a:b;
        s[k][0]=double(u[0])/(u[0]+u[1])*u[0]/(u[0]+v[3]);
        s[k][1]=double(u[1])/(u[0]+u[1])*0.8*u[1]/(u[1]+v[3]);
        double miss=1-s[k][0]-s[k][1];
        stay[k]=miss*0.8*u[2]/(u[2]+v[2]);
        change[k]=miss-stay[k];
        den[k]=1-stay[k];
    }
    for(int i=0;i<=n;++i) for(int j=0;j<=n;++j) {
        if(i==0) dp[i][j][0]=dp[i][j][1]=1;
        else if(j==0) dp[i][j][0]=dp[i][j][1]=0;
        else {
            double x=s[0][0]*dp[max(0,i-2)][j][0]+s[0][1]*dp[max(0,i-3)][j][0];
            double y=s[1][0]*dp[i][max(0,j-2)][1]+s[1][1]*dp[i][max(0,j-3)][1];
            double det=den[0]*den[1]-change[0]*change[1];
            dp[i][j][0]=(x*den[1]+change[0]*y)/det;
            dp[i][j][1]=(y*den[0]+change[1]*x)/det;
        }
    }
    return dp[n][n][0];
}
int main(){
    int n,m,b[4],a[4];
    while(scanf("%d%d%d%d%d%d",&n,&m,b,b+1,b+2,b+3)==6){
        double ans=0;
        for(a[0]=1;a[0]<=10;++a[0]) for(a[1]=1;a[1]<=10;++a[1]) for(a[2]=1;a[2]<=10;++a[2]){
            a[3]=m-a[0]-a[1]-a[2];
            if(a[3]>=1&&a[3]<=10) ans=max(ans,chance(n,a,b));
        }
        printf("%.3f\n",ans);
    }
    return 0;
}
