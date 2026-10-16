// POJ 3676 - Tomorrow Genius 2008
// Model: gpt-6.1-sol
// Fill an extreme corner of the remaining region by backtracking over
// orientation-preserving triangle placements. Maintain its directed boundary
// by splitting and cancelling opposite segments; SAT forbids interior overlap.
// All supplied triangles are used, as suggested by "piece them together".
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double E=1e-7;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double a)const{return P(x*a,y*a);} };
double cr(P a,P b){return a.x*b.y-a.y*b.x;}
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
bool eq(P a,P b){return dot(a-b,a-b)<E*E;}
struct Seg {P a,b; Seg(P x,P y):a(x),b(y){} };
struct Tri {P p[3];};
int n; double w,h; Tri t[10]; vector<Tri> placed;
bool on(P p,Seg s){P v=s.b-s.a; return fabs(cr(p-s.a,v))<E*sqrt(dot(v,v)) && dot(p-s.a,p-s.b)<E;}
struct Along {P a,v; Along(P aa,P vv):a(aa),v(vv){} bool operator()(P x,P y)const{return dot(x-a,v)<dot(y-a,v);} };
vector<Seg> subtractTri(vector<Seg> b,Tri q){
 for(int i=0;i<3;i++) b.push_back(Seg(q.p[(i+1)%3],q.p[i]));
 vector<Seg> s;
 for(unsigned i=0;i<b.size();i++){
  vector<P> pts; pts.push_back(b[i].a);pts.push_back(b[i].b);
  for(unsigned j=0;j<b.size();j++) if(on(b[j].a,b[i])) pts.push_back(b[j].a);
  sort(pts.begin(),pts.end(),Along(b[i].a,b[i].b-b[i].a));
  for(unsigned j=1;j<pts.size();j++) if(!eq(pts[j-1],pts[j])) s.push_back(Seg(pts[j-1],pts[j]));
 }
 vector<int> dead(s.size(),0);
 for(unsigned i=0;i<s.size();i++) if(!dead[i]) for(unsigned j=i+1;j<s.size();j++) if(!dead[j]&&eq(s[i].a,s[j].b)&&eq(s[i].b,s[j].a)){dead[i]=dead[j]=1;break;}
 b.clear();for(unsigned i=0;i<s.size();i++)if(!dead[i])b.push_back(s[i]);
 bool changed=true;
 while(changed){changed=false;for(unsigned i=0;i<b.size()&&!changed;i++)for(unsigned j=0;j<b.size();j++)if(i!=j&&eq(b[i].b,b[j].a)&&fabs(cr(b[i].b-b[i].a,b[j].b-b[j].a))<E&&dot(b[i].b-b[i].a,b[j].b-b[j].a)>0){b[i].b=b[j].b;b.erase(b.begin()+j);changed=true;break;}}
 return b;
}
bool overlap(Tri a,Tri b){
 for(int k=0;k<2;k++){Tri c=k?b:a;for(int i=0;i<3;i++){
  P v=c.p[(i+1)%3]-c.p[i];P axis(-v.y,v.x); double al=1e100,ah=-1e100,bl=1e100,bh=-1e100;
  for(int j=0;j<3;j++){double z=dot(a.p[j],axis);al=min(al,z);ah=max(ah,z);z=dot(b.p[j],axis);bl=min(bl,z);bh=max(bh,z);}
  if(min(ah,bh)-max(al,bl)<=E*sqrt(dot(axis,axis)))return false;
 }}return true;
}
bool same(Tri a,Tri b){for(int k=0;k<3;k++){bool ok=true;for(int j=0;j<3;j++)if(fabs(dot(a.p[(j+1)%3]-a.p[j],a.p[(j+1)%3]-a.p[j])-dot(b.p[(j+k+1)%3]-b.p[(j+k)%3],b.p[(j+k+1)%3]-b.p[(j+k)%3]))>E)ok=false;if(ok)return true;}return false;}
bool dfs(int mask,vector<Seg> b){
 if(mask==(1<<n)-1)return b.empty(); if(b.empty())return false;
 unsigned e=0;for(unsigned i=1;i<b.size();i++)if(b[i].a.x<b[e].a.x-E||(fabs(b[i].a.x-b[e].a.x)<E&&b[i].a.y<b[e].a.y-E))e=i;
 P a=b[e].a, v=b[e].b-a;v=v*(1/sqrt(dot(v,v)));
 for(int i=0;i<n;i++)if(!(mask>>i&1)){
  bool duplicate=false;for(int j=0;j<i;j++)if(!(mask>>j&1)&&same(t[i],t[j]))duplicate=true;if(duplicate)continue;
  for(int k=0;k<3;k++){
   P z=t[i].p[(k+1)%3]-t[i].p[k];double len=sqrt(dot(z,z));double co=dot(z,v)/len,si=cr(z,v)/len;Tri q;bool ok=true;
   for(int j=0;j<3;j++){P d=t[i].p[j]-t[i].p[k];q.p[j]=a+P(co*d.x-si*d.y,si*d.x+co*d.y);if(q.p[j].x<-E||q.p[j].x>w+E||q.p[j].y<-E||q.p[j].y>h+E)ok=false;}
   if(!ok)continue;for(unsigned j=0;j<placed.size();j++)if(overlap(q,placed[j]))ok=false;if(!ok)continue;
   vector<Seg> nb=subtractTri(b,q);placed.push_back(q);if(dfs(mask|(1<<i),nb))return true;placed.pop_back();
  }
 }
 return false;
}
int main(){if(scanf("%d%lf%lf",&n,&w,&h)!=3)return 0;double area=0;for(int i=0;i<n;i++){for(int j=0;j<3;j++)scanf("%lf%lf",&t[i].p[j].x,&t[i].p[j].y);double a=cr(t[i].p[1]-t[i].p[0],t[i].p[2]-t[i].p[0]);if(a<0)swap(t[i].p[1],t[i].p[2]);area+=fabs(a);}
 vector<Seg>b;b.push_back(Seg(P(0,0),P(w,0)));b.push_back(Seg(P(w,0),P(w,h)));b.push_back(Seg(P(w,h),P(0,h)));b.push_back(Seg(P(0,h),P(0,0)));
 puts(fabs(area-2*w*h)<E&&dfs(0,b)?"YES":"NO");return 0;}
