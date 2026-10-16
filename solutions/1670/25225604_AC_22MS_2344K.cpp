// POJ 1670 - Obstructed Rook Circuits
// Model: gpt-6-astra
// Grow both ends of a Hamiltonian path after choosing the start's two edges.
// Residual degree-two squares force the next edge; premature closure is forbidden.
// Coordinates are (column,row). A circuit needs two distinct neighbors per square.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
vector< vector<int> > adj;
vector<int> deg, live, nxt;
int R,C;
void change(int u,int d) {
 live[u]=(d==1);
 for(int i=0;i<(int)adj[u].size();++i) deg[adj[u][i]]+=d;
}
int choices(int a,int b,int *v) {
 int k=0,force=-1;
 for(int i=0;i<(int)adj[a].size();++i) {
  int x=adj[a][i];
  if(!live[x] || x==b) continue;
  v[k++]=x;
  if(deg[x]<2) return 0;
  if(deg[x]==2) { if(force!=-1) return 0; force=x; }
 }
 if(force!=-1) {v[0]=force; return 1;}
 return k;
}
bool search(int a,int b,int remain) {
 if(remain==2) {
  for(int i=0;i<(int)adj[a].size();++i) if(adj[a][i]==b) {nxt[a]=b;return true;}
  return false;
 }
 int va[4],vb[4];
 int na=choices(a,b,va),nb=choices(b,a,vb);
 if(!na || !nb) return false;
 bool left=na<=nb;
 int u=left?a:b, count=left?na:nb, *v=left?va:vb;
 change(u,-1);
 for(int i=0;i<count;++i) {
  int x=v[i];
  if(left) nxt[u]=x; else nxt[x]=u;
  if(search(left?x:a,left?b:x,remain-1)) {change(u,1);return true;}
 }
 change(u,1);return false;
}
int main() {
 int B,sx,sy;
 while(scanf("%d%d%d%d%d",&R,&C,&B,&sx,&sy)==5 && R) {
  int n=R*C,start=sy*C+sx,total=n-B,parity=0;
  live.assign(n,1);deg.assign(n,0);nxt.assign(n,-1);adj.assign(n,vector<int>());
  for(int i=0,x,y;i<B;++i){scanf("%d%d",&x,&y);live[y*C+x]=0;}
  for(int y=0;y<R;++y)for(int x=0;x<C;++x)if(live[y*C+x]){
   int u=y*C+x;parity+=((x+y)%2?1:-1);
   int dx[]={0,1,-1,0},dy[]={-1,0,0,1};
   for(int d=0;d<4;++d){int xx=x+dx[d],yy=y+dy[d];if(xx>=0&&xx<C&&yy>=0&&yy<R&&live[yy*C+xx])adj[u].push_back(yy*C+xx);}
   deg[u]=adj[u].size();
  }
  bool possible=total>=4 && parity==0 && live[start],ok=false;
  for(int i=0;i<n;++i)if(live[i]&&deg[i]<2)possible=false;
  if(possible) {
   change(start,-1);
   for(int i=0;i<(int)adj[start].size()&&!ok;++i)for(int j=i+1;j<(int)adj[start].size()&&!ok;++j){
    int a=adj[start][i],b=adj[start][j];nxt[start]=a;nxt[b]=start;
    ok=search(a,b,total-1);
   }
   change(start,1);
  }
  if(!ok)puts("NO SOLUTION\n");
  else {
   int u=start;
   for(int i=0;i<total;++i){int v=nxt[u];putchar(v==u-C?'N':v==u+C?'S':v==u+1?'E':'W');if(i%40==39||i==total-1)putchar('\n');u=v;}
   putchar('\n');
  }
 }
 return 0;
}
