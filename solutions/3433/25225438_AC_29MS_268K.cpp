// POJ 3433 - Road Accident
// Model: gpt-6.1-sol
// Split each car at both midlines into the four numbered corner parts.
// Continuous separating-axis tests find each pair's first contact time.
// Minimize the part numbers independently over all simultaneous contacts.
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>
using namespace std;
typedef long double Real;
struct P {
 Real x,y;
 P(Real a=0,Real b=0):x(a),y(b){}
 P operator+(P p)const{return P(x+p.x,y+p.y);}
 P operator-(P p)const{return P(x-p.x,y-p.y);}
 P operator*(Real s)const{return P(x*s,y*s);}
};
Real dot(P a,P b){return a.x*b.x+a.y*b.y;}
struct Box { P c,e,f; Real a,b; };
struct Car {Box q[4]; P vel;};
Car readcar(){
 Real x,y,u,v,w,s;cin>>x>>y>>u>>v>>w>>s;
 P back(x,y),d(u-x,v-y);Real len=sqrt(dot(d,d));
 P e=d*(1/len),f(e.y,-e.x); // forward and right
 Car c;c.vel=e*s;
 int front[4]={0,1,1,0},right[4]={0,0,1,1};
 for(int i=0;i<4;++i){
  c.q[i].c=back+e*(len*(front[i]?0.75L:0.25L))+f*(w*(right[i]?0.75L:0.25L));
  c.q[i].e=e;c.q[i].f=f;c.q[i].a=len/4;c.q[i].b=w/4;
 }
 return c;
}
Real contact(Box a,Box b,P vel){
 P ax[4]={a.e,a.f,b.e,b.f}; Real lo=0,hi=1e100L;
 for(int i=0;i<4;++i){
  P n=ax[i];Real r=a.a*fabs(dot(a.e,n))+a.b*fabs(dot(a.f,n))+
   b.a*fabs(dot(b.e,n))+b.b*fabs(dot(b.f,n));
  Real d=dot(b.c-a.c,n),v=dot(vel,n);
  if(fabs(v)<1e-20L){if(fabs(d)>r+1e-10L)return 1e100L;}
  else {Real t1=(-r-d)/v,t2=(r-d)/v;if(t1>t2)swap(t1,t2);lo=max(lo,t1);hi=min(hi,t2);}
 }
 if(lo>hi+1e-12L*(1+fabs(lo)))return 1e100L;
 return lo;
}
int main(){
 Car a=readcar(),b=readcar();Real t[4][4],best=1e100L;
 for(int i=0;i<4;++i)for(int j=0;j<4;++j){t[i][j]=contact(a.q[i],b.q[j],b.vel-a.vel);best=min(best,t[i][j]);}
 int p=4,q=4;
 for(int i=0;i<4;++i)for(int j=0;j<4;++j)
  if(fabs(t[i][j]-best)<=1e-10L*(1+fabs(best))){p=min(p,i+1);q=min(q,j+1);}
 cout<<p<<' '<<q<<'\n';
}
