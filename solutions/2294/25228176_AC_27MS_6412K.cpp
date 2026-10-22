// POJ 2294 - Unfoldung
// Model: gpt-6-astra
// Recover local square-edge order from induced nonseparating cycles of the
// complete face graph, then develop the uncut hinges into the integer plane.
// Cut edges are essential to reconstruct the embedding. Overlap is allowed.
// Ambiguous local cycles are resolved by selecting a consistent surface
// rotation system: every edge has two sides and every square four corners.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=10005;
vector<int> g[N],u[N];
int n,seen[N],stamp,ban[N],path[7],corner[N][4][4],ord[N][4];
vector< vector<int> > faces;
vector< pair<int,int> > constraints[N];
int q[N],ori[N],xx[N],yy[N],vis[N];
int slot(int v,int w){for(int i=0;i<4;i++)if(g[v][i]==w)return i;return -1;}
bool adjacent(int v,int w){return find(g[v].begin(),g[v].end(),w)!=g[v].end();}
void cycle(int len){
 ++stamp;for(int i=0;i<len;i++)ban[path[i]]=stamp;
 int start=-1,targets=0;
 for(int i=0;i<len;i++)for(int j=0;j<4;j++){
  int w=g[path[i]][j];if(ban[w]!=stamp&&seen[w]!=-stamp){seen[w]=-stamp;++targets;start=w;}
 }
 if(start<0)return;
 int head=0,tail=0;q[tail++]=start;seen[start]=stamp;--targets;
 while(head<tail&&targets){int v=q[head++];for(int j=0;j<4;j++){
  int w=g[v][j];if(ban[w]==stamp||seen[w]==stamp)continue;
  if(seen[w]==-stamp)--targets;seen[w]=stamp;q[tail++]=w;
 }}
 if(targets)return;
 faces.push_back(vector<int>(path,path+len));
 for(int i=0;i<len;i++){
  int v=path[i],a=slot(v,path[(i+len-1)%len]),b=slot(v,path[(i+1)%len]);
  corner[v][a][b]=corner[v][b][a]=1;
 }
}
int edgeid[N][4],ecount,used[20010],chosen[60010],cmask[N];
vector<int> fedges[60010],fvertices[60010],fbits[60010],incident[20010];
vector< pair<int*,int> > changes;
int allowed[3]={0,0,0};
void change(int &v,int value){changes.push_back(make_pair(&v,v));v=value;}
void undo(unsigned mark){while(changes.size()>mark){*changes.back().first=changes.back().second;changes.pop_back();}}
bool valid(int f){
 if(chosen[f])return false;
 for(unsigned j=0;j<fedges[f].size();j++)if(used[fedges[f][j]]==2)return false;
 for(unsigned j=0;j<fvertices[f].size();j++){
  int v=fvertices[f][j],b=fbits[f][j];if(cmask[v]&b)return false;
  int mask=cmask[v]|b;bool ok=false;
  for(int k=0;k<3;k++)if((mask&allowed[k])==mask)ok=true;
  if(!ok)return false;
 }return true;
}
void selectface(int f){
 change(chosen[f],1);
 for(unsigned j=0;j<fedges[f].size();j++)change(used[fedges[f][j]],used[fedges[f][j]]+1);
 for(unsigned j=0;j<fvertices[f].size();j++){int v=fvertices[f][j];change(cmask[v],cmask[v]|fbits[f][j]);}
}
bool complete(){
 for(;;){
  bool progress=false;int branch=-1,best=100000;
  for(int e=0;e<ecount;e++)if(used[e]<2){
   vector<int> options;
   for(unsigned j=0;j<incident[e].size();j++)if(valid(incident[e][j]))options.push_back(incident[e][j]);
   int need=2-used[e];if((int)options.size()<need)return false;
   if((int)options.size()==need){
    for(unsigned j=0;j<options.size();j++){if(!valid(options[j]))return false;selectface(options[j]);}
    progress=true;
   }else if((int)options.size()<best){best=(int)options.size();branch=options[0];}
  }
  if(progress)continue;
  if(branch<0)return true;
  unsigned mark=changes.size();selectface(branch);if(complete())return true;undo(mark);
  change(chosen[branch],-1);if(complete())return true;undo(mark);return false;
 }
}
bool reconstruct(){
 ecount=0;
 for(int v=0;v<n;v++)for(int k=0;k<4;k++)if(v<g[v][k]){
  edgeid[v][k]=edgeid[g[v][k]][slot(g[v][k],v)]=ecount;incident[ecount++].clear();
 }
 if(faces.size()>60000)return false;
 for(unsigned i=0;i<faces.size();i++){
  fedges[i].clear();fvertices[i].clear();fbits[i].clear();
  vector<int>&p=faces[i];
  for(unsigned j=0;j<p.size();j++){
   int v=p[j],a=slot(v,p[(j+p.size()-1)%p.size()]),b=slot(v,p[(j+1)%p.size()]);
   int e=edgeid[v][b];fedges[i].push_back(e);incident[e].push_back((int)i);
   fvertices[i].push_back(v);fbits[i].push_back((1<<(a*4+b))|(1<<(b*4+a)));
  }
 }
 int perm[3][4]={{0,1,2,3},{0,1,3,2},{0,2,1,3}};
 for(int k=0;k<3;k++){allowed[k]=0;for(int j=0;j<4;j++){int a=perm[k][j],b=perm[k][(j+1)%4];allowed[k]|=(1<<(a*4+b))|(1<<(b*4+a));}}
 memset(chosen,0,sizeof(chosen));memset(used,0,sizeof(used));memset(cmask,0,sizeof(cmask));changes.clear();
 if(!complete())return false;
 vector< vector<int> > selected;
 for(unsigned i=0;i<faces.size();i++)if(chosen[i]==1)selected.push_back(faces[i]);
 faces.swap(selected);
 memset(corner,0,sizeof(corner));
 for(int v=0;v<n;v++)for(int a=0;a<4;a++)for(int b=0;b<4;b++)corner[v][a][b]=(cmask[v]>>(a*4+b))&1;
 return true;
}
void dfs(int len){
 int v=path[len-1];
 for(int j=0;j<4;j++){
  int w=g[v][j];if(w<=path[0])continue;
  bool ok=true;for(int i=1;i<len;i++)if(path[i]==w)ok=false;
  for(int i=1;i<len-1;i++)if(adjacent(path[i],w))ok=false;
  if(!ok)continue;
  path[len]=w;
  if(len>=2&&adjacent(path[0],w)){if(path[1]<w)cycle(len+1);}
  else if(len<5)dfs(len+1);
 }
}
int main(){
 int t;if(scanf("%d",&t)!=1)return 0;
 while(t--){
  int m;scanf("%d%d",&n,&m);for(int i=0;i<n;i++){g[i].clear();u[i].clear();}
  for(int i=0;i<m;i++){int a,b;char c;scanf("%d%c%d",&a,&c,&b);--a;--b;g[a].push_back(b);g[b].push_back(a);if(c=='-'){u[a].push_back(b);u[b].push_back(a);}}
  memset(vis,0,sizeof(vis));int h=0,z=0;q[z++]=0;vis[0]=1;int edges=0;
  while(h<z){int v=q[h++];edges+=(int)u[v].size();for(unsigned j=0;j<u[v].size();j++){int w=u[v][j];if(!vis[w]){vis[w]=1;q[z++]=w;}}}
  if(z<n){puts("DISCONNECTED");continue;}
  if(edges==2*(n-1)){puts("CAN UNFOLD");continue;}
  memset(corner,0,sizeof(corner));memset(seen,0,sizeof(seen));memset(ban,0,sizeof(ban));stamp=0;faces.clear();
  bool ok=true;for(int i=0;i<n;i++)if(g[i].size()!=4)ok=false;
  if(!ok){puts("CANNOT UNFOLD");continue;}
  for(int i=0;i<n;i++){path[0]=i;dfs(1);}
  bool ambiguous=false;
  for(int v=0;v<n;v++)for(int a=0;a<4;a++){int count=0;for(int b=0;b<4;b++)count+=corner[v][a][b];if(count!=2)ambiguous=true;}
  if(ambiguous&&!reconstruct()){puts("CANNOT UNFOLD");continue;}
  // Each recovered surface vertex fixes adjacent sides of every incident face.
  for(int v=0;v<n;v++){
   for(int a=0;a<4;a++){int count=0;for(int b=0;b<4;b++)count+=corner[v][a][b];if(count!=2)ok=false;}
   ord[v][0]=0;int a=0,prev=-1;
   for(int k=1;k<4;k++){int b=0;while(b<4&&(!corner[v][a][b]||b==prev))++b;if(b==4){ok=false;break;}ord[v][k]=b;prev=a;a=b;}
  }
  if(!ok){puts("CANNOT UNFOLD");continue;}
  // A facial cycle turns in the same direction at every incident dual vertex.
  for(int i=0;i<n;i++)constraints[i].clear();
  for(unsigned i=0;i<faces.size();i++){
   vector<int>&p=faces[i];int first=-1;
   for(unsigned j=0;j<p.size();j++){
    int v=p[j],a=slot(v,p[(j+p.size()-1)%p.size()]),b=slot(v,p[(j+1)%p.size()]);
    int k=0;while(ord[v][k]!=a)++k;
    int turn=(ord[v][(k+1)%4]==b);
    if(j==0)first=turn;else {constraints[p[0]].push_back(make_pair(v,first^turn));constraints[v].push_back(make_pair(p[0],first^turn));}
   }
  }
  memset(vis,0,sizeof(vis));h=z=0;q[z++]=0;vis[0]=1;ori[0]=0;
  while(h<z){int v=q[h++];for(unsigned j=0;j<constraints[v].size();j++){
   int w=constraints[v][j].first,r=ori[v]^constraints[v][j].second;
   if(!vis[w]){vis[w]=1;ori[w]=r;q[z++]=w;}else if(ori[w]!=r)ok=false;
  }}
  if(z<n)ok=false;
  for(int v=0;v<n;v++)if(ori[v])swap(ord[v][1],ord[v][3]);
  memset(vis,0,sizeof(vis));h=z=0;q[z++]=0;vis[0]=1;ori[0]=xx[0]=yy[0]=0;
  const int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1};
  while(h<z){int v=q[h++];for(unsigned j=0;j<u[v].size();j++){
   int w=u[v][j],a=slot(v,w),b=slot(w,v),k=0,r=0;
   while(ord[v][k]!=a)++k;while(ord[w][r]!=b)++r;
   int d=(ori[v]+k)%4,rotation=(d+2-r+4)%4,x=xx[v]+dx[d],y=yy[v]+dy[d];
   if(!vis[w]){vis[w]=1;ori[w]=rotation;xx[w]=x;yy[w]=y;q[z++]=w;}
   else if(ori[w]!=rotation||xx[w]!=x||yy[w]!=y)ok=false;
  }}
  puts(ok?"CAN UNFOLD":"CANNOT UNFOLD");
 }
}
