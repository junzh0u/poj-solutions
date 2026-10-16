// POJ 2863 - Mission Impossible
// Model: gpt-6.1-sol
// Overlapping radar disks form a graph. Angular potentials around an informer
// are consistent exactly when no graph cycle winds around that informer.
// The country border only defines the score; travel may cross that border.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} };
P operator-(P a,P b){return P(a.x-b.x,a.y-b.y);}
double dot(P a,P b){return a.x*b.x+a.y*b.y;}
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
P border[1001],person[1001],c[30]; double r[30],pot[30];
bool edge[30][30],seen[30]; int m;
bool reachable(P p){
 for(int i=0;i<m;i++) if(dot(c[i]-p,c[i]-p)<=r[i]*r[i]+1e-9) return false;
 for(int i=0;i<m;i++) seen[i]=false;
 for(int root=0;root<m;root++) if(!seen[root]){
  int q[30],head=0,tail=0; q[tail++]=root; seen[root]=true; pot[root]=0;
  while(head<tail){int u=q[head++];
   for(int v=0;v<m;v++) if(edge[u][v]){
    P a=c[u]-p,b=c[v]-p;
    double value=pot[u]+atan2(cross(a,b),dot(a,b));
    if(!seen[v]) {seen[v]=true;pot[v]=value;q[tail++]=v;}
    else if(fabs(value-pot[v])>1e-7) return false;
   }
  }
 }
 return true;
}
double distance2(P p,P a,P b){
 P d=b-a; double len=dot(d,d),t=len?dot(p-a,d)/len:0;
 t=max(0.0,min(1.0,t)); P v=P(p.x-a.x-t*d.x,p.y-a.y-t*d.y);return dot(v,v);
}
int main(){int b,n;
 while(scanf("%d",&b)==1){
  for(int i=0;i<b;i++) scanf("%lf%lf",&border[i].x,&border[i].y);
  scanf("%d",&n);for(int i=0;i<n;i++)scanf("%lf%lf",&person[i].x,&person[i].y);
  scanf("%d",&m);for(int i=0;i<m;i++)scanf("%lf%lf%lf",&c[i].x,&c[i].y,&r[i]);
  if(!b&&!n&&!m)break;
  for(int i=0;i<m;i++)for(int j=0;j<m;j++)edge[i][j]=i!=j&&dot(c[i]-c[j],c[i]-c[j])<=(r[i]+r[j])*(r[i]+r[j])+1e-9;
  int best=-1;double score=-1;
  for(int i=0;i<n;i++)if(reachable(person[i])){
   double value=1e100;for(int j=0;j<b;j++)value=min(value,distance2(person[i],border[j],border[(j+1)%b]));
   if(value>score+1e-8){score=value;best=i;}
  }
  if(best<0)puts("Mission impossible");else printf("Contact informer %d\n",best+1);
 }
 return 0;
}
