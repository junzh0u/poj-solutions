// POJ 2776 - Cheat at Math
// Model: gpt-6.1-sol
// Merge coincident arcs, then add arcs with Euler's formula and a DSU.
// Arcs face left: retain circle points to the right of their endpoint chord.
// Count distinct intersection points, including tangencies and endpoints.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double EPS=1e-7;
struct Arc {double x,y,r,l; int t;} a[1000];
struct Point {double x,y,angle;} p[2001];
int par[1000],seen[1000];
int root(int x){return par[x]==x?x:par[x]=root(par[x]);}
bool lessp(const Point &a,const Point &b){return a.angle<b.angle;}
int main(){
 int N,n=0; if(scanf("%d",&N)!=1)return 0;
 for(int i=0;i<N;++i){Arc b;scanf("%lf%lf%lf%d",&b.x,&b.y,&b.r,&b.t);int j;
  for(j=0;j<n;++j)if(a[j].x==b.x&&a[j].y==b.y&&a[j].r==b.r)break;
  if(j==n)a[n++]=b;else a[j].t=max(a[j].t,b.t);
 }
 for(int i=0;i<n;++i){a[i].l=a[i].x+a[i].r*cos(a[i].t*acos(-1.0)/4);par[i]=i;}
 int ans=1;
 for(int i=0;i<n;++i){int m=0,c=0;
  for(int j=0;j<i;++j){double dx=a[j].x-a[i].x,dy=a[j].y-a[i].y,d2=dx*dx+dy*dy,d=sqrt(d2);
   if(d<EPS||d>a[i].r+a[j].r+EPS||d<fabs(a[i].r-a[j].r)-EPS)continue;
   double h=(a[i].r*a[i].r-a[j].r*a[j].r+d2)/(2*d),v=sqrt(max(0.0,a[i].r*a[i].r-h*h));
   int hits=0;
   for(int s=-1;s<=1;s+=2){if(s==1&&v<EPS)continue;
    double x=a[i].x+(dx*h-s*dy*v)/d,y=a[i].y+(dy*h+s*dx*v)/d;
    if(x+EPS<a[i].l||x+EPS<a[j].l)continue;
    p[m].x=x;p[m].y=y;p[m].angle=atan2(y-a[i].y,x-a[i].x);++m;++hits;
   }
   if(hits){int r=root(j);if(seen[r]!=i+1){seen[r]=i+1;++c;}}
  }
  sort(p,p+m,lessp);int k=0;
  for(int j=0;j<m;++j)if(j==0||fabs(p[j].x-p[j-1].x)>EPS||fabs(p[j].y-p[j-1].y)>EPS)++k;
  if(k>1&&fabs(p[0].x-p[m-1].x)<EPS&&fabs(p[0].y-p[m-1].y)<EPS)--k;
  ans+=k-c+(a[i].t==4);
  for(int j=0;j<i;++j)if(seen[root(j)]==i+1)par[root(j)]=i;
 }
 printf("%d\n",ans);return 0;
}
