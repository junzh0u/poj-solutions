// POJ 3568 - Diver
// Model: gpt-6-astra
// Scale time by shark speed: danger regions become periodic radius-r disks.
// A disk passed below imposes its lower arc and both speed-limit tangents.
// Propagate intersecting disks through a sufficient number of periods.
// A dependency chain surviving beyond all simple periodic paths is impossible.
// Clip tangent rays at time zero and the surface. Past disk centers cannot
// cause a future bottom collision. Equality is safe: intersections are strict.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct Disk { double x,y; int shark; Disk(double X,double Y,int I):x(X),y(Y),shark(I){} };
bool rayhit(double x,double y,double a,int sign,const Disk &b,double r,double d){
 double dx=b.x-x,dy=b.y-y, norm=1+a*a;
 double proj=a*dx+sign*dy, disc=proj*proj-norm*(dx*dx+dy*dy-r*r);
 if(disc<=1e-9)return false;
 double lo=(proj-sqrt(disc))/norm,hi=(proj+sqrt(disc))/norm;
 lo=max(lo,0.);hi=min(hi,(d-x)/a);
 if(sign<0)hi=min(hi,y);else lo=max(lo,-y);
 return lo<hi-1e-9;
}
int main(){
 double d,vd,r,w,vs; int n;
 if(scanf("%lf%lf%d%lf%lf%lf",&d,&vd,&n,&r,&w,&vs)!=6)return 0;
 double a=vd/vs, s=sqrt(1+a*a), p=2*w, eps=1e-9;
 vector<Disk> b;
 int copies=(int)ceil(d/(a*p))+n+5;
 for(int i=0;i<n;i++){
  double dep,pos; int f; scanf("%lf%lf%d",&dep,&pos,&f);
  double first=f==1?2*w-pos:pos;
  for(int k=-1;k<copies;k++) if(first+k*p+r>0)
   b.push_back(Disk(d-dep,first+k*p,i));
 }
 vector<int> q,seen(b.size(),0);
 for(unsigned i=0;i<b.size();i++)
  if(rayhit(0,0,a,1,b[i],r,d)){seen[i]=1;q.push_back(i);}
 double ans=d/vd;
 for(unsigned head=0;head<q.size();head++){
  int i=q[head];
  if((b[i].x<r-eps && b[i].y>=0) || b[i].y>(copies-2)*p) {puts("IMPOSSIBLE");return 0;}

  ans=max(ans,(b[i].y+(d-b[i].x+r*s)/a)/vs);
  for(unsigned j=0;j<b.size();j++)if(!seen[j]){
   double x=b[j].x-b[i].x,y=b[j].y-b[i].y;
   bool hit=x*x+y*y<4*r*r-eps;
   if(rayhit(b[i].x-r/s,b[i].y+a*r/s,a,1,b[j],r,d))hit=true;
   if(rayhit(b[i].x-r/s,b[i].y-a*r/s,a,-1,b[j],r,d))hit=true;
   if(hit){seen[j]=1;q.push_back(j);}
  }
 }
 printf("%.10f\n",ans);
}
