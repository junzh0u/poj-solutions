// POJ 3755 - The Training of Chain Lightning
// Model: gpt-6.1-sol
// Follow each possible initial pillar with Mars absent. At every step the
// speeds that make Mars the next target form a closed interval. The union
// through time t covers (0,V) exactly when every speed can be hit by t.
// The first target must be a pillar; Mars chooses his speed before Sram
// chooses that pillar. Equal distances favor Mars.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct Interval { double l,r; int t; };
struct Less { bool operator()(const Interval&a,const Interval&b)const {return a.l<b.l;} };
int main(){
 int n; double a,v,x[30],y[30],d[30][30];
 while(scanf("%d",&n)==1 && n){
  scanf("%lf%lf",&a,&v);
  for(int i=0;i<n;++i)scanf("%lf%lf",x+i,y+i);
  for(int i=0;i<n;++i)for(int j=0;j<n;++j){double dx=x[i]-x[j],dy=y[i]-y[j];d[i][j]=dx*dx+dy*dy;}
  vector<Interval> seg;
  for(int s=0;s<n;++s)if(x[s]*x[s]+y[s]*y[s]<=2500.0){
   bool used[30]={false}; int cur=s; used[cur]=true;
   for(int t=1;t<=n;++t){
    int next=-1; double r2=2500.0;
    for(int j=0;j<n;++j)if(!used[j] && d[cur][j]<=r2){r2=d[cur][j];next=j;}
    if(y[cur]*y[cur]<=r2){
     double h=sqrt(r2-y[cur]*y[cur]);
     Interval z; z.l=(x[cur]-h-a)/t;z.r=(x[cur]+h-a)/t;z.t=t;
     if(z.r>0 && z.l<v)seg.push_back(z);
    }
    if(next<0)break;
    cur=next;used[cur]=true;
   }
  }
  sort(seg.begin(),seg.end(),Less());
  int ans=-1;
  for(int t=1;t<=n;++t){
   double reach=0;
   for(size_t i=0;i<seg.size();++i)if(seg[i].t<=t){
    if(seg[i].l>reach+1e-10)break;
    if(seg[i].r>reach)reach=seg[i].r;
   }
   if(reach>=v-1e-10){ans=t;break;}
  }
  printf("%d\n",ans);
 }
 return 0;
}
