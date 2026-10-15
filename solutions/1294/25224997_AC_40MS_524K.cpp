// POJ 1294 - Not Too Convex Hull
// Model: gpt-6.1-sol
// Sort rays around the origin. Disjoint convex wrappings partition that order
// into contiguous groups spanning less than pi, with at least two other nails.
// Precompute their convex hull areas with an incremental angular hull, then
// try each circular cut and partition into exactly B groups by dynamic programming.
// Doubled areas use double: integer products and all area sums are exact here.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { double x,y,a; } p[205],h[105];
bool cmp(const Point& a,const Point& b){return a.a<b.a;}
double cross(Point a,Point b){return a.x*b.y-a.y*b.x;}
double turn(Point a,Point b,Point c){Point u,v;u.x=b.x-a.x;u.y=b.y-a.y;v.x=c.x-b.x;v.y=c.y-b.y;return cross(u,v);}
double cost[205][205],dp[105][105];
const double INF=1e30;
int main(){int B,N;while(scanf("%d%d",&B,&N)==2 && B){
 double ox,oy;scanf("%lf%lf",&ox,&oy);int n=N-1;
 for(int i=0;i<n;++i){scanf("%lf%lf",&p[i].x,&p[i].y);p[i].x-=ox;p[i].y-=oy;p[i].a=atan2(p[i].y,p[i].x);}
 sort(p,p+n,cmp);for(int i=0;i<n;++i)p[i+n]=p[i];
 for(int i=0;i<2*n;++i)for(int j=0;j<2*n;++j)cost[i][j]=INF;
 for(int s=0;s<n;++s){int z=0;double area=0;h[z++]=p[s];
  for(int j=s+1;j<s+n && cross(p[s],p[j])>0;++j){
   while(z>=2 && turn(h[z-2],h[z-1],p[j])<=0){area-=cross(h[z-2],h[z-1]);--z;}
   area+=cross(h[z-1],p[j]);h[z++]=p[j];cost[s][j]=area;
  }
 }
 for(int s=n;s<2*n;++s)for(int j=s+1;j<2*n;++j)cost[s][j]=cost[s-n][j-n];
 double ans=INF;
 for(int s=0;s<n;++s){
  for(int b=0;b<=B;++b)for(int j=0;j<=n;++j)dp[b][j]=INF;dp[0][0]=0;
  for(int b=1;b<=B;++b)for(int j=2*b;j<=n-2*(B-b);++j){
   for(int k=2*(b-1);k<=j-2;++k){double v=dp[b-1][k]+cost[s+k][s+j-1];if(v<dp[b][j])dp[b][j]=v;}
  }
  if(dp[B][n]<ans)ans=dp[B][n];
 }
 printf("%.2f\n",ans/2);
}return 0;}
