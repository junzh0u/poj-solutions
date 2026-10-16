// POJ 1901 - Hypertransmission
// Model: gpt-6-astra
// Control translated from NEERC 2003 jury h_al_short.dpr.
// Separate same-party and opposite-party counters; batch equal radii.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Edge { int d,b,e; bool operator<(const Edge& v)const{return d<v.d;} } a[500001];
int x[1001],y[1001],z[1001],p[1001],np[1001],nm[1001];
int main(){
 int n,ac=0;
 if(scanf("%d",&n)!=1)return 0;
 for(int i=1;i<=n;i++)scanf("%d%d%d%d",x+i,y+i,z+i,p+i);
 for(int i=1;i<=n;i++)for(int j=i+1;j<=n;j++){
  a[ac].d=(x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])+(z[i]-z[j])*(z[i]-z[j]);
  a[ac].b=i;a[ac].e=j;++ac;
 }
 sort(a,a+ac);
 int maxd=0,maxr=0,dest=0;
 a[ac].d=2147483647;
 for(int i=1;i<=n;i++)np[i]=1;
 for(int i=0;i<ac;i++){
  int b=a[i].b,e=a[i].e;
  if(p[b]==p[e]){
   ++np[b];if(np[b]==nm[b])--dest;
   ++np[e];if(np[e]==nm[e])--dest;
  }else{
   if(np[b]==nm[b])++dest;++nm[b];
   if(np[e]==nm[e])++dest;++nm[e];
  }
  if(a[i].d<a[i+1].d && dest>maxd){maxd=dest;maxr=a[i].d;}
 }
 printf("%d\n%.10f\n",maxd,sqrt((double)maxr));
 return 0;
}
