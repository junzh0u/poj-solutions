// POJ 3896 - Multiprocessor Scheduling
// Model: gpt-6.1-sol
// Shortest monotone path in the two-job progress diagram. Conflicting
// procedure pairs are forbidden rectangles. An optimal path turns only
// at grid corners: run both jobs to the next conflict, then follow its
// entry boundary to a corner. At every crossed boundary we also allow
// one job to wait while the other finishes, permitting anticipatory waits.
// Rectangle boundaries are legal: finishing and starting may coincide.
#include <cstdio>
#include <algorithm>
using namespace std;
int p[2][301],s[2][301],dp[301][301];
int main(){
 int T; scanf("%d",&T);
 while(T--){
  int n; scanf("%d",&n);
  for(int k=0;k<2;++k){s[k][0]=0;for(int i=1;i<=n;++i){int d;scanf("%d%d",&p[k][i],&d);s[k][i]=s[k][i-1]+d;}}
  for(int i=0;i<=n;++i)for(int j=0;j<=n;++j)dp[i][j]=1000000000;
  dp[0][0]=0;
  for(int i=0;i<=n;++i)for(int j=0;j<=n;++j){
   int base=dp[i][j];
   if(i<n)dp[i+1][j]=min(dp[i+1][j],base+s[0][i+1]-s[0][i]);
   if(j<n)dp[i][j+1]=min(dp[i][j+1],base+s[1][j+1]-s[1][j]);
   int a=i+1,b=j+1,x=s[0][i],y=s[1][j],elapsed=0;
   while(a<=n && b<=n){
    {
     if(y==s[1][b-1])dp[a][b-1]=min(dp[a][b-1],base+elapsed+s[0][a]-x);
     if(x==s[0][a-1])dp[a-1][b]=min(dp[a-1][b],base+elapsed+s[1][b]-y);
    }
    if(p[0][a]==p[1][b])break;
    int d=min(s[0][a]-x,s[1][b]-y);x+=d;y+=d;elapsed+=d;
    if(x==s[0][a])++a;
    if(y==s[1][b])++b;
   }
   if(a>n || b>n)dp[n][n]=min(dp[n][n],base+elapsed+max(s[0][n]-x,s[1][n]-y));
  }
  printf("%d\n",dp[n][n]);
 }
 return 0;
}
