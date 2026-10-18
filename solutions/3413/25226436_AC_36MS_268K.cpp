// POJ 3413 - RPG
// Model: gpt-6-astra
// Subset DP maximizes the product of conditional quest probabilities.
// Experience depends only on the completed subset. Keep predecessors even
// for zero-probability states so that every answer is a full permutation.
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int n,d,a[10],b[10],s[10];
    if(scanf("%d%d",&n,&d)!=2) return 0;
    for(int i=0;i<n;++i) scanf("%d%d%d",&a[i],&b[i],&s[i]);
    double dp[1024]; int xp[1024],last[1024];
    int total=1<<n;
    for(int m=0;m<total;++m) { dp[m]=-1; xp[m]=d; last[m]=-1; }
    dp[0]=1;
    for(int m=1;m<total;++m) {
        for(int i=0;i<n;++i) if(m&(1<<i)) { xp[m]=xp[m^(1<<i)]+s[i]; break; }
    }
    for(int m=0;m<total;++m) for(int i=0;i<n;++i) if(!(m&(1<<i))) {
        double p;
        if(xp[m]<=a[i]) p=0;
        else if(xp[m]>=b[i]) p=1;
        else p=double(xp[m]-a[i])/(b[i]-a[i]);
        int next=m|(1<<i);
        double value=dp[m]*p;
        if(value>dp[next]) { dp[next]=value; last[next]=i; }
    }
    int order[10],m=total-1;
    for(int k=n-1;k>=0;--k) { order[k]=last[m]; m^=1<<order[k]; }
    printf("%.3f\n",dp[total-1]);
    for(int k=0;k<n;++k) printf("%d%c",order[k]+1,k+1==n?'\n':' ');
    return 0;
}
