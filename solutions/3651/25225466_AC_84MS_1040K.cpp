// POJ 3651 - Slink
// Model: gpt-6.1-sol
// Maintain all allowed edge patterns at each cell and vertex, enforcing
// agreement on shared edges by arc consistency. Branch only if necessary.
// A completed assignment must form one cycle. Render straight vertices as
// lines, and turns as plus signs, within the required padded hash border.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;
struct Factor { vector<int> e; };
struct Arc { int a,b,s[16]; };
vector<Factor> f;
vector<Arc> arcs;
vector<vector<int> > incoming;
vector<int> eu,ev,answer;
int R,C,H[21][20],V[20][21],nums[20][20];
int bits(int x) { int n=0;for(;x;x&=x-1)++n;return n; }
void add(vector<int> e,int target,vector<int>& d) {
 Factor z;z.e=e; f.push_back(z);int dom=0;
 for(int m=0;m<(1<<(int)e.size());++m) if(target<0?(bits(m)==0||bits(m)==2):(bits(m)==target))dom|=1<<m;
 d.push_back(dom);
}
bool propagate(vector<int>& d) {
 queue<int> q;vector<int> in(arcs.size(),1);
 for(int k=0;k<(int)arcs.size();++k)q.push(k);
 while(!q.empty()) {
  int k=q.front();q.pop();in[k]=0;const Arc &a=arcs[k];int nd=d[a.a];
  for(int m=0;m<16;++m) if((nd&(1<<m)) && !(a.s[m]&d[a.b]))nd^=1<<m;
  if(nd==0)return false;
  if(nd!=d[a.a]) {d[a.a]=nd; for(int j=0;j<(int)incoming[a.a].size();++j) {int t=incoming[a.a][j];if(!in[t]){in[t]=1;q.push(t);}}}
 }
 return true;
}
bool connected(const vector<int>& d) {
 answer.assign(eu.size(),0);
 for(int a=0;a<(int)f.size();++a) {int m=0;while(!(d[a]&(1<<m)))++m;for(int j=0;j<(int)f[a].e.size();++j)answer[f[a].e[j]]=(m>>j)&1;}
 vector<vector<int> > g((R+1)*(C+1));int start=-1,count=0;
 for(int e=0;e<(int)eu.size();++e)if(answer[e]){g[eu[e]].push_back(ev[e]);g[ev[e]].push_back(eu[e]);start=eu[e];++count;}
 if(start<0)return false;
 vector<int> seen(g.size(),0);queue<int> q;q.push(start);seen[start]=1;int vertices=0;
 while(!q.empty()){int x=q.front();q.pop();++vertices;for(int j=0;j<(int)g[x].size();++j){int y=g[x][j];if(!seen[y]){seen[y]=1;q.push(y);}}}
 return vertices==count;
}
bool solve(vector<int> d) {
 if(!propagate(d))return false;
 int best=-1,n=100;
 for(int a=0;a<(int)d.size();++a){int b=bits(d[a]);if(b>1&&b<n){best=a;n=b;}}
 if(best<0)return connected(d);
 for(int m=0;m<16;++m)if(d[best]&(1<<m)){vector<int> nd=d;nd[best]=1<<m;if(solve(nd))return true;}
 return false;
}
int main(){int tc=0;while(cin>>R>>C && R){
 for(int i=0;i<R;++i)for(int j=0;j<C;++j)cin>>nums[i][j];
 eu.clear();ev.clear();f.clear();arcs.clear();vector<int>d;
 for(int i=0;i<=R;++i)for(int j=0;j<C;++j){H[i][j]=eu.size();eu.push_back(i*(C+1)+j);ev.push_back(i*(C+1)+j+1);}
 for(int i=0;i<R;++i)for(int j=0;j<=C;++j){V[i][j]=eu.size();eu.push_back(i*(C+1)+j);ev.push_back((i+1)*(C+1)+j);}
 for(int i=0;i<R;++i)for(int j=0;j<C;++j){vector<int>e;e.push_back(H[i][j]);e.push_back(H[i+1][j]);e.push_back(V[i][j]);e.push_back(V[i][j+1]);add(e,nums[i][j],d);}
 for(int i=0;i<=R;++i)for(int j=0;j<=C;++j){vector<int>e;if(i)e.push_back(V[i-1][j]);if(i<R)e.push_back(V[i][j]);if(j)e.push_back(H[i][j-1]);if(j<C)e.push_back(H[i][j]);add(e,-1,d);}
 incoming.assign(f.size(),vector<int>());
 for(int a=0;a<(int)f.size();++a)for(int b=0;b<(int)f.size();++b)if(a!=b){
  vector<int>p,t;for(int i=0;i<(int)f[a].e.size();++i)for(int j=0;j<(int)f[b].e.size();++j)if(f[a].e[i]==f[b].e[j]){p.push_back(i);t.push_back(j);}
  if(p.empty())continue;Arc z;z.a=a;z.b=b;
  for(int m=0;m<16;++m){z.s[m]=0;for(int n=0;n<16;++n){bool ok=true;for(int j=0;j<(int)p.size();++j)if(((m>>p[j])&1)!=((n>>t[j])&1))ok=false;if(ok)z.s[m]|=1<<n;}}
  incoming[b].push_back(arcs.size());arcs.push_back(z);
 }
 if(!solve(d))return 1;
 vector<string>out(2*R+5,string(4*C+5,' '));
 int height=out.size(),width=out[0].size();
 out[0]=out[height-1]=string(width,'#');for(int i=1;i<height-1;++i)out[i][0]=out[i][width-1]='#';
 for(int i=0;i<R;++i)for(int j=0;j<C;++j)out[3+2*i][4+4*j]='0'+nums[i][j];
 for(int i=0;i<=R;++i)for(int j=0;j<C;++j)if(answer[H[i][j]])for(int k=0;k<=4;++k)out[2+2*i][2+4*j+k]='-';
 for(int i=0;i<R;++i)for(int j=0;j<=C;++j)if(answer[V[i][j]])for(int k=0;k<=2;++k)out[2+2*i+k][2+4*j]='|';
 for(int i=0;i<=R;++i)for(int j=0;j<=C;++j){bool h=(j&&answer[H[i][j-1]])||(j<C&&answer[H[i][j]]);bool v=(i&&answer[V[i-1][j]])||(i<R&&answer[V[i][j]]);if(h&&v)out[2+2*i][2+4*j]='+';}
 cout<<++tc<<'\n';for(int i=0;i<height;++i)cout<<out[i]<<'\n';
 }return 0;}
