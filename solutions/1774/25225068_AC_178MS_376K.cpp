// POJ 1774 - Fold Paper Strips
// Model: gpt-6.1-sol
// Keep each original strip section as a reflected convex quadrilateral.
// A crease fails only when the preceding fold has the same direction and
// folded paper covers the crease segment in its interior; mere contact is legal.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double EPS=1e-8;
struct P { double x,y; P(){} P(double X,double Y):x(X),y(Y){} };
struct Q { P p[4]; };
double cross(P a,P b,P c){return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);}
P reflect(P p,P a,P b){double x=b.x-a.x,y=b.y-a.y; double t=((p.x-a.x)*x+(p.y-a.y)*y)/(x*x+y*y);return P(2*(a.x+t*x)-p.x,2*(a.y+t*y)-p.y);}
bool covers(const Q& q,P a,P b){
 double area=0;for(int i=0;i<4;i++)area+=q.p[i].x*q.p[(i+1)%4].y-q.p[i].y*q.p[(i+1)%4].x;
 if(fabs(area)<EPS)return false;
 double lo=0,hi=1,sgn=area>0?1:-1;
 for(int i=0;i<4;i++){
  double u=sgn*cross(q.p[i],q.p[(i+1)%4],a),v=sgn*cross(q.p[i],q.p[(i+1)%4],b);
  double delta=v-u;
  if(fabs(delta)<EPS){if(u < EPS)return false;}
  else if(delta>0)lo=max(lo,-u/delta);else hi=min(hi,-u/delta);
 }
 return hi-lo>EPS;
}
int main(){int n;while(scanf("%d",&n)==1&&n){
 vector<Q> pieces;double pa=0,pb=0;int prev=-1,fail=0;
 for(int i=1;i<=n;i++){
 double a,b,d;scanf("%lf%lf%lf",&a,&b,&d);if(fail)continue;
 P top(a,1),bottom(b,0);
 if(prev==(int)d){for(unsigned j=0;j<pieces.size();j++)if(covers(pieces[j],top,bottom)){fail=i;break;}}
 if(fail)continue;
 Q q;q.p[0]=P(pa,1);q.p[1]=top;q.p[2]=bottom;q.p[3]=P(pb,0);pieces.push_back(q);
 for(unsigned j=0;j<pieces.size();j++)for(int k=0;k<4;k++)pieces[j].p[k]=reflect(pieces[j].p[k],top,bottom);
 pa=a;pb=b;prev=(int)d;
 }
 if(fail)printf("NO %d\n",fail);else puts("YES");
 }return 0;}
