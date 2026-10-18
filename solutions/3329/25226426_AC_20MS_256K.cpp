// POJ 3329 - Twirl Around
// Model: gpt-6-astra
// Event-driven clockwise rotation. Events are endpoint/edge contacts or
// vertex/interior contacts. At each event test the possible contact pivots;
// if none permits a clockwise motion, the bar is stuck. Segment containment
// is checked between every boundary intersection, including concave walls.
// Test halfway to the next event: containment is constant in that open
// angular interval. Retain tiny positive events at nearly aligned corners.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0), EPS=1e-9;
struct P {
 double x,y;
 P(double a=0,double b=0):x(a),y(b){}
 P operator+(P b)const{return P(x+b.x,y+b.y);}
 P operator-(P b)const{return P(x-b.x,y-b.y);}
 P operator*(double k)const{return P(x*k,y*k);}
};
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
double norm(P a){return sqrt(dot(a,a));}
P rot(P a,double t){return P(a.x*cos(t)+a.y*sin(t),a.y*cos(t)-a.x*sin(t));}
vector<P> poly;
bool on(P p,P a,P b){P e=b-a;return fabs(cross(e,p-a))<EPS*norm(e)&&dot(p-a,p-b)<EPS;}
bool inside(P p){
 bool in=false;
 for(int i=0;i<(int)poly.size();++i){P a=poly[i],b=poly[(i+1)%poly.size()];
  if(on(p,a,b))return true;
  if((a.y>p.y)!=(b.y>p.y))if(p.x<(b.x-a.x)*(p.y-a.y)/(b.y-a.y)+a.x)in=!in;
 }
 return in;
}
bool fits(P a,P b){
 if(!inside(a)||!inside(b))return false;
 vector<double> t; t.push_back(0);t.push_back(1);P d=b-a;
 for(int i=0;i<(int)poly.size();++i){P p=poly[i],e=poly[(i+1)%poly.size()]-p;
  double den=cross(d,e);
  if(fabs(den)>1e-12){double u=cross(p-a,e)/den,v=cross(p-a,d)/den;
   if(u>0&&u<1&&v>=0&&v<=1)t.push_back(u);
  }
 }
 sort(t.begin(),t.end());
 for(int i=1;i<(int)t.size();++i)if(!inside(a+d*((t[i]+t[i-1])*0.5)))return false;
 return true;
}
double angle(P from,P to){double t=atan2(cross(to,from),dot(from,to));if(t<0)t+=2*PI;return t;}
void consider(double t,double &best){if(t>1e-12&&t<best)best=t;}
double nextEvent(P a,P b,P c){
 double best=2*PI;P ends[2]={a,b};
 for(int k=0;k<2;++k){P v=ends[k]-c;double r=norm(v);if(r<1e-8)continue;
  for(int i=0;i<(int)poly.size();++i){P p=poly[i],e=poly[(i+1)%poly.size()]-p;
   double ee=dot(e,e),s=dot(c-p,e)/ee;P foot=p+e*s;
   double h=r*r-dot(foot-c,foot-c);if(h<-1e-8)continue;
   double ds=sqrt(max(0.0,h)/ee);
   for(int sign=-1;sign<=1;sign+=2){double q=s+sign*ds;if(q>=-1e-10&&q<=1+1e-10)consider(angle(v,p+e*q-c),best);}
  }
 }
 P u=(b-a)*(1.0/norm(b-a));double lo=dot(a-c,u),hi=dot(b-c,u);
 for(int i=0;i<(int)poly.size();++i){P v=poly[i]-c;double r=norm(v);if(r<1e-8)continue;
  if(r<=hi+1e-9)consider(angle(u,v),best);
  if(-r>=lo-1e-9)consider(angle(u,v*(-1)),best);
 }
 return best;
}
int main(){double L,R;int n;while(scanf("%lf%lf%d",&L,&R,&n)==3&&n){
 poly.clear();for(int i=0;i<n;++i){double x,y;scanf("%lf%lf",&x,&y);poly.push_back(P(x,y));}
 P a(0,0),b(0,L);double left=2*PI*R;
 for(int iter=0;left>1e-9&&iter<100000;++iter){
  vector<P> piv;
  for(int i=0;i<n;++i){if(on(a,poly[i],poly[(i+1)%n])){piv.push_back(a);break;}}
  for(int i=0;i<n;++i){if(on(b,poly[i],poly[(i+1)%n])){piv.push_back(b);break;}}
  for(int i=0;i<n;++i)if(on(poly[i],a,b))piv.push_back(poly[i]);
  bool found=false;
  for(int i=0;i<(int)piv.size();++i){P c=piv[i];double t=nextEvent(a,b,c),probe=t*0.5;
   if(!fits(c+rot(a-c,probe),c+rot(b-c,probe)))continue;
   t=min(t,left);a=c+rot(a-c,t);b=c+rot(b-c,t);left-=t;found=true;break;
  }
  if(!found)break;
 }
 printf("%.10f %.10f\n",a.x,a.y);
}return 0;}
