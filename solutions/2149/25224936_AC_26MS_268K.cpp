// POJ 2149 - Inherit the Spheres
// Model: gpt-6.1-sol
// Sweep all sphere endpoints and intersection-circle height extrema.
// Between successive events, union overlapping cross-sectional discs.
// Contained spheres and covered endpoints need no special event handling:
// only an actual change in the component count contributes an output bit.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
struct Sphere { double x,y,z,r; } s[100];
int p[100];
int root(int a) { return p[a]==a?a:p[a]=root(p[a]); }
int main() {
 int n;
 while(scanf("%d",&n)==1 && n) {
  vector<double> ev; ev.push_back(0); ev.push_back(36000);
  double dist[100][100];
  for(int i=0;i<n;++i) {
   scanf("%lf%lf%lf%lf",&s[i].x,&s[i].y,&s[i].z,&s[i].r);
   ev.push_back(s[i].z-s[i].r); ev.push_back(s[i].z+s[i].r);
  }
  for(int i=0;i<n;++i) for(int j=0;j<i;++j) {
   double dx=s[j].x-s[i].x,dy=s[j].y-s[i].y,dz=s[j].z-s[i].z;
   double h2=dx*dx+dy*dy,d2=h2+dz*dz,d=sqrt(d2);
   dist[i][j]=sqrt(h2);
   if(d<=fabs(s[i].r-s[j].r) || d>=s[i].r+s[j].r) continue;
   double a=(d2+s[i].r*s[i].r-s[j].r*s[j].r)/(2*d);
   double cz=s[i].z+a*dz/d;
   double dzr=sqrt(max(0.0,s[i].r*s[i].r-a*a)*h2/d2);
   ev.push_back(cz-dzr); ev.push_back(cz+dzr);
  }
  sort(ev.begin(),ev.end());
  string ans; int prev=0;
  for(size_t k=1;k<ev.size();++k) {
   if(ev[k]-ev[k-1]<1e-8) continue;
   double z=(ev[k]+ev[k-1])/2,rad[100]; int count=0;
   for(int i=0;i<n;++i) {
    p[i]=i; double q=s[i].r*s[i].r-(z-s[i].z)*(z-s[i].z);
    rad[i]=q>0?sqrt(q):-1;
    if(rad[i]>=0) ++count;
   }
   for(int i=0;i<n;++i) if(rad[i]>=0) for(int j=0;j<i;++j)
    if(rad[j]>=0 && rad[i]+rad[j]>=dist[i][j]) {
     int a=root(i),b=root(j); if(a!=b) { p[a]=b; --count; }
    }
   while(prev<count) { ans+='1'; ++prev; }
   while(prev>count) { ans+='0'; --prev; }
  }
  printf("%d\n%s\n",int(ans.size()),ans.c_str());
 }
 return 0;
}
