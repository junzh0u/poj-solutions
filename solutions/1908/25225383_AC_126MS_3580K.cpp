// POJ 1908 - Grid Speed
// Model: gpt-6.1-sol
// Shortest routes are monotone. Dynamic programming keeps minimum fuel
// for each intersection and exact time in units of grid_distance/210 minutes.
// Arrival constraints apply before rounding; displayed minutes round upward.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;
int main() {
 int cases; scanf("%d", &cases);
 for(int cs=1;cs<=cases;++cs){
  int n,d,h[11],v[11],xs,ys,xt,yt,lo,hi;
  scanf("%d%d",&n,&d);
  for(int i=1;i<=n;++i) scanf("%d",&h[i]);
  for(int i=1;i<=n;++i) scanf("%d",&v[i]);
  scanf("%d%d%d%d%d%d",&xs,&ys,&xt,&yt,&lo,&hi);
  int w=abs(xt-xs),z=abs(yt-ys),sx=xt>=xs?1:-1,sy=yt>=ys?1:-1;
  int cap=min((w+z)*2520,hi*210/d),low=(lo*210+d-1)/d;
  const double INF=1e100;
  vector< vector<double> > a(w+1,vector<double>(cap+1,INF)), b=a;
  a[0][0]=0;
  for(int y=0;y<=z;++y){
   for(int x=0;x<=w;++x){
    int limits[2]={h[ys+sy*y],v[xs+sx*x]};
    for(int dir=0;dir<2;++dir){
     if((dir==0 && x==w)||(dir==1 && y==z)) continue;
     vector<double> &dest=dir==0?a[x+1]:b[x];
     for(int k=1;k*5<=limits[dir];++k){
      int dt=2520/k; double fuel=d/(80.0-0.75*k*k);
      for(int t=0;t+dt<=cap;++t) if(a[x][t]<INF){
       double val=a[x][t]+fuel;
       if(val<dest[t+dt])dest[t+dt]=val;
      }
     }
    }
   }
   if(y<z){a.swap(b);for(int x=0;x<=w;++x)fill(b[x].begin(),b[x].end(),INF);}
  }
  int first=-1,best=-1;
  for(int t=low;t<=cap;++t)if(a[w][t]<INF){
   if(first<0) first=t;
   if(best<0 || a[w][t]<a[w][best]-1e-9) best=t;
  }
  printf("Scenario %d:\n",cs);
  if(first<0)puts("IMPOSSIBLE");
  else {
   printf("The earliest  arrival: %d minutes, fuel %.2f gallons\n",(first*d+209)/210,a[w][first]);
   printf("The economical travel: %d minutes, fuel %.2f gallons\n",(best*d+209)/210,a[w][best]);
  }
 }
 return 0;
}
