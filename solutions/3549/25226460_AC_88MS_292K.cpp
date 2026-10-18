// POJ 3549 - GSM phone
// Model: gpt-6-astra
// A shortest path is straight except at exposed disk intersections: a bend
// on a smooth circular boundary can be shortened inside that disk.
// Keep only intersections not strictly inside another disk. Connect visible
// vertices by testing whether disk intervals cover the entire segment, then
// run Dijkstra. Closed intervals allow travel through tangent contacts.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
typedef long double Real;
const Real EPS=1e-10L;
struct Point { Real x,y; Point(Real a=0,Real b=0):x(a),y(b){} };
Point operator+(Point a,Point b){return Point(a.x+b.x,a.y+b.y);}
Point operator-(Point a,Point b){return Point(a.x-b.x,a.y-b.y);}
Point operator*(Point a,Real k){return Point(a.x*k,a.y*k);}
Real dot(Point a,Point b){return a.x*b.x+a.y*b.y;}
Real norm(Point a){return sqrtl(dot(a,a));}
struct Disk {Point p; Real r;};
vector<Disk> c;
vector<Point> v;
bool covered(Point a,Point b){
 Real len=norm(b-a); if(len<EPS)return true;
 Point u=(b-a)*(1/len);
 vector<pair<Real,Real> > in;
 for(unsigned i=0;i<c.size();++i){
  Point w=c[i].p-a; Real t=dot(w,u),h=fabsl(w.x*u.y-w.y*u.x);
  if(h>c[i].r+EPS)continue;
  Real d=sqrtl(max((Real)0,(c[i].r-h)*(c[i].r+h)));
  Real l=t-d,r=t+d;
  if(r>=-EPS && l<=len+EPS)in.push_back(make_pair(l,r));
 }
 sort(in.begin(),in.end()); Real end=0;
 for(unsigned i=0;i<in.size();++i){
  if(in[i].first>end+EPS)return false;
  end=max(end,in[i].second); if(end>=len-EPS)return true;
 }
 return false;
}
void add(Point p,int a,int b){
 for(unsigned k=0;k<c.size();++k)if((int)k!=a&&(int)k!=b)
  if(norm(p-c[k].p)<c[k].r-EPS)return;
 v.push_back(p);
}
int main(){
 Point a,b; int n;
 while(scanf("%Lf%Lf%Lf%Lf",&a.x,&a.y,&b.x,&b.y)==4){
  if(scanf("%d",&n)!=1)return 0;
  c.resize(n); v.clear(); v.push_back(a);v.push_back(b);
  for(int i=0;i<n;++i)scanf("%Lf%Lf%Lf",&c[i].p.x,&c[i].p.y,&c[i].r);
  if(covered(a,b)){printf("%.5Lf\n",norm(b-a));continue;}
  for(int i=0;i<n;++i)for(int j=0;j<i;++j){
   Point w=c[j].p-c[i].p; Real d=norm(w),r=c[i].r,s=c[j].r;
   if(d<EPS||d>r+s+EPS||d<fabsl(r-s)-EPS)continue;
   Real x=(d+(r-s)*(r+s)/d)/2;
   Real h=sqrtl(max((Real)0,(r-x)*(r+x)));
   Point u=w*(1/d),p=c[i].p+u*x,q=Point(-u.y,u.x)*h;
   add(p+q,i,j); if(h>EPS)add(p-q,i,j);
  }
  int m=v.size(); vector<Real> dist(m,1e100L);vector<int> used(m,0);dist[0]=0;
  for(int z=0;z<m;++z){
   int u=-1;for(int i=0;i<m;++i)if(!used[i]&&(u<0||dist[i]<dist[u]))u=i;
   if(u<0||u==1)break; used[u]=1;
   for(int i=0;i<m;++i)if(!used[i]){
    Real nd=dist[u]+norm(v[i]-v[u]);
    if(nd<dist[i]&&covered(v[u],v[i]))dist[i]=nd;
   }
  }
  printf("%.5Lf\n",dist[1]);
 }
}
