// POJ 1843 - Shire
// Model: gpt-6.1-sol
// Enumerate a bounding rectangle; its Manhattan perimeter is 2*(X+Y).
// Angular-order convex-chain DP counts boundary towers, including collinear
// towers on the initial and final rays from Mordor. All arithmetic is exact.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
struct P { int x,y; };
int cross(P a,P b) { return a.x*b.y-a.y*b.x; }
bool angle(P a,P b) { int c=cross(a,b); return c ? c>0 : a.x+a.y>b.x+b.y; }
int turn(P a,P b,P c) { P u={b.x-a.x,b.y-a.y},v={c.x-b.x,c.y-b.y}; return cross(u,v); }
int main() {
 int n,L; if(scanf("%d",&n)!=1) return 0;
 vector<P> a(n); for(int i=0;i<n;i++) scanf("%d%d",&a[i].x,&a[i].y); scanf("%d",&L);
 int ans=1;
 for(int r=0;r<n;r++) {
  int X=a[r].x,Y=L/2-X; if(Y<0) continue;
  vector<P> p; P o={0,0}; p.push_back(o);
  for(int i=0;i<n;i++) if(a[i].x<=X && a[i].y<=Y) p.push_back(a[i]);
  sort(p.begin()+1,p.end(),angle); int m=p.size(),rad[52],dp[52][52];
  memset(dp,0,sizeof(dp));
  for(int i=1;i<m;i++) {
   rad[i]=0; for(int j=i+1;j<m;j++) if(cross(p[i],p[j])==0) rad[i]++;
   dp[0][i]=2+rad[i]; ans=max(ans,dp[0][i]);
  }
  for(int j=1;j<m;j++) for(int i=1;i<j;i++) {
   if(cross(p[i],p[j])==0) continue;
   for(int k=0;k<i;k++) if(dp[k][i] && turn(p[k],p[i],p[j])>=0)
    dp[i][j]=max(dp[i][j],dp[k][i]+1);
   if(dp[i][j]) ans=max(ans,dp[i][j]+rad[j]);
  }
 }
 printf("%d\n",ans); return 0;
}
