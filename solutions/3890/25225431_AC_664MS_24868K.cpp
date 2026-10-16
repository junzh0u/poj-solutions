// POJ 3890 - No Smoking, Please
// Model: gpt-6.1-sol
// Minimum s-t cut with passage cost area+1 in thousands of euros.
// Zero area means no passage and costs zero. Implicit grid residual arcs
// and iterative Dinic searches keep both memory and call-stack use bounded.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Caps { unsigned short a[4]; Caps() { a[0]=a[1]=a[2]=a[3]=0; } };
int main() {
 int cases; scanf("%d", &cases);
 while(cases--) {
  int r,c,er,ec,kr,kc; scanf("%d%d%d%d%d%d",&r,&c,&er,&ec,&kr,&kc);
  int w=c+2,n=(r+2)*w,delta[4]={1,-1,w,-w};
  vector<Caps> cap(n);
  for(int i=0;i<r;i++) for(int j=0;j<c-1;j++) {
   int x; scanf("%d",&x); int u=(i+1)*w+j+1;
   cap[u].a[0]=cap[u+1].a[1]=x?x+1:0;
  }
  for(int i=0;i<r-1;i++) for(int j=0;j<c;j++) {
   int x; scanf("%d",&x); int u=(i+1)*w+j+1;
   cap[u].a[2]=cap[u+w].a[3]=x?x+1:0;
  }
  int s=(er+1)*w+ec+1,t=(kr+1)*w+kc+1,flow=0;
  vector<int> level(n),q(n),path(n),low(n);
  vector<unsigned char> ptr(n);
  while(s!=t) {
   fill(level.begin(),level.end(),-1); int head=0,tail=0;
   q[tail++]=s; level[s]=0;
   while(head<tail) {
    int u=q[head++];
    for(int d=0;d<4;d++) if(cap[u].a[d]) {
     int v=u+delta[d]; if(level[v]<0) {level[v]=level[u]+1;q[tail++]=v;}
    }
   }
   if(level[t]<0) break;
   fill(ptr.begin(),ptr.end(),0);
   for(;;) {
    int depth=0; path[0]=s; low[0]=10000;
    while(depth>=0 && path[depth]!=t) {
     int u=path[depth];
     while(ptr[u]<4) {
      int d=ptr[u],v=u+delta[d];
      if(cap[u].a[d] && level[v]==level[u]+1) break;
      ++ptr[u];
     }
     if(ptr[u]==4) {level[u]=-1;--depth;}
     else {int d=ptr[u],v=u+delta[d];path[depth+1]=v;low[depth+1]=min(low[depth],(int)cap[u].a[d]);++depth;}
    }
    if(depth<0) break;
    int add=low[depth];flow+=add;
    for(int i=0;i<depth;i++) {int u=path[i],d=ptr[u],v=path[i+1];cap[u].a[d]-=add;cap[v].a[d^1]+=add;}
   }
  }
  printf("%d\n",flow*1000);
 }
 return 0;
}
