// POJ 2099 - Fool's Day
// Model: gpt-6.1-sol
// Center alignment suffices by central symmetry and convexity. For each
// envelope strip, the rotated card width is a sum of two absolute projections.
// Every boundary of feasible rotations solves one of four sinusoidal equations;
// test all their roots, and both faces of the postcard. Either diagonal defines
// the same parallelogram up to reflection. Tangency is allowed.
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;
const double EPS=1e-8;
struct V { double x,y; V(double X=0,double Y=0):x(X),y(Y){} };
double dot(V a,V b){return a.x*b.x+a.y*b.y;}
V second(double a,double b,double d){double x=(d*d-a*a-b*b)/(2*a);return V(x,sqrt(b*b-x*x));}
bool fit(double a,double b,double d,double c,double e,double f,int face){
 V v=second(a,b,d), q=second(c,e,f);q.y*=face;
 V n[2]={V(0,1),V(-v.y/b,v.x/b)};
 double w[2]={v.y,a*v.y/b}, A[2][2],B[2][2];
 V edge[2]={V(c,0),q};
 vector<double> angle;angle.push_back(0);
 for(int i=0;i<2;++i){
  for(int j=0;j<2;++j){A[i][j]=dot(n[i],edge[j]);B[i][j]=-n[i].x*edge[j].y+n[i].y*edge[j].x;}
  for(int s=-1;s<=1;s+=2)for(int t=-1;t<=1;t+=2){
   double x=s*A[i][0]+t*A[i][1],y=s*B[i][0]+t*B[i][1],r=sqrt(x*x+y*y);
   if(r+EPS<w[i] || r==0)continue;
   double z=w[i]/r;if(z>1)z=1;
   double delta=acos(z),base=atan2(y,x);
   angle.push_back(base+delta);angle.push_back(base-delta);
  }
 }
 for(unsigned k=0;k<angle.size();++k){
  double co=cos(angle[k]),si=sin(angle[k]);bool ok=true;
  for(int i=0;i<2;++i)if(fabs(A[i][0]*co+B[i][0]*si)+fabs(A[i][1]*co+B[i][1]*si)>w[i]+EPS)ok=false;
  if(ok)return true;
 }
 return false;
}
int main(){int n;scanf("%d",&n);while(n--){double a,b,d,c,e,f;scanf("%lf%lf%lf%lf%lf%lf",&a,&b,&d,&c,&e,&f);puts(fit(a,b,d,c,e,f,1)||fit(a,b,d,c,e,f,-1)?"Yes":"No");}return 0;}
