// POJ 3798 - Cave Crisis
// Model: gpt-6.1-sol
// Inflate obstacles and both tunnel walls by the robot radius. A connected
// top-to-bottom barrier first appears at half the minimax polygon gap.
// Also bound the radius by the clearance at the fixed starting point.
// Polygon distances include intersections and containment; touching is allowed
// at the limiting radius, and a zero limiting radius means impossible.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} };
P operator-(P a,P b){return P(a.x-b.x,a.y-b.y);}
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
double ps(P p,P a,P b){ P v=b-a; double t=dot(p-a,v)/dot(v,v); t=max(0.0,min(1.0,t)); return sqrt(dot(p-P(a.x+t*v.x,a.y+t*v.y),p-P(a.x+t*v.x,a.y+t*v.y))); }
bool on(P p,P a,P b){return fabs(cross(b-a,p-a))<1e-9 && dot(p-a,p-b)<=1e-9;}
int sign(double x){return (x>1e-9)-(x< -1e-9);}
bool intersect(P a,P b,P c,P d){
 int x=sign(cross(b-a,c-a)), y=sign(cross(b-a,d-a));
 int z=sign(cross(d-c,a-c)), t=sign(cross(d-c,b-c));
 return (x*y<0 && z*t<0)||on(a,c,d)||on(b,c,d)||on(c,a,b)||on(d,a,b);
}
bool inside(P p,const vector<P>& a){
 bool in=false; int n=a.size();
 for(int i=0,j=n-1;i<n;j=i++){
  if(on(p,a[j],a[i]))return true;
  if((a[i].y>p.y)!=(a[j].y>p.y) && p.x<(a[j].x-a[i].x)*(p.y-a[i].y)/(a[j].y-a[i].y)+a[i].x)in=!in;
 }return in;
}
double distancePoly(const vector<P>& a,const vector<P>& b){
 if(inside(a[0],b)||inside(b[0],a))return 0;
 double res=1e50;
 for(unsigned i=0;i<a.size();++i)for(unsigned j=0;j<b.size();++j){
  P c=a[i],d=a[(i+1)%a.size()],e=b[j],f=b[(j+1)%b.size()];
  if(intersect(c,d,e,f))return 0;
  res=min(res,min(min(ps(c,e,f),ps(d,e,f)),min(ps(e,c,d),ps(f,c,d))));
 }
 return res;
}
int main(){
 int w,n; while(scanf("%d%d",&w,&n)==2 && (w||n)){
  vector< vector<P> > a(n); double ans=w/2.0;
  double g[102][102],best[102]; bool used[102]={false};
  for(int i=0;i<n+2;++i)for(int j=0;j<n+2;++j)g[i][j]=1e50;
  g[n][n+1]=g[n+1][n]=w;
  for(int i=0;i<n;++i){int k;scanf("%d",&k);a[i].resize(k); double lo=1e50,hi=-1e50;
   for(int j=0;j<k;++j){scanf("%lf%lf",&a[i][j].x,&a[i][j].y);lo=min(lo,a[i][j].y);hi=max(hi,a[i][j].y);}
   for(int j=0;j<k;++j)ans=min(ans,ps(P(),a[i][j],a[i][(j+1)%k]));
   g[i][n]=g[n][i]=w/2.0-hi;g[i][n+1]=g[n+1][i]=lo+w/2.0;
   for(int j=0;j<i;++j)g[i][j]=g[j][i]=distancePoly(a[i],a[j]);
  }
  for(int i=0;i<n+2;++i)best[i]=1e50;best[n]=0;
  for(int z=0;z<n+2;++z){int v=-1;for(int i=0;i<n+2;++i)if(!used[i]&&(v<0||best[i]<best[v]))v=i;
   used[v]=true;for(int i=0;i<n+2;++i)best[i]=min(best[i],max(best[v],g[v][i]));
  }
  ans=min(ans,best[n+1]/2);if(ans<1e-9)puts("impossible");else printf("%.2f\n",ans);
 }
 return 0;
}
