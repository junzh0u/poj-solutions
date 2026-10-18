// POJ 3732 - Paint Me Less
// Model: gpt-6-astra
// Iterative deepening over region-merging recolorings. Each remaining
// nonzero color requires a move; canonical states share failure bounds.
// Regions can be chosen anywhere, including zero regions. Only merging
// moves are needed before the final recoloring to zero.
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;
struct Move { int p,c,score; Move(int P,int C,int S):p(P),c(C),score(S){} };
int R,C,N; vector<vector<int> > adj; vector<Move> path;
map<string,int> memo;
string key(const vector<int>& a) {
 map<int,int> m; m[0]=0; int k=1; string s;
 for(int i=0;i<N;i++) {if(!m.count(a[i]))m[a[i]]=k++; s+=char(m[a[i]]);}
 return s;
}
bool cmp(const Move&a,const Move&b){return a.score>b.score;}
bool dfs(const vector<int>& a,int left) {
 map<int,int> counts; for(int i=0;i<N;i++)counts[a[i]]++;
 int bound=counts.size()-(counts.count(0)?1:0);
 if(!bound)return true;
 if(bound>left)return false;
 if(counts.size()==1){path.push_back(Move(N-1,0,0));return true;}
 string s=key(a); map<string,int>::iterator it=memo.find(s);
 if(it!=memo.end()&&it->second>=left)return false;
 vector<int> id(N,-1); vector<vector<int> > reg;
 for(int i=0;i<N;i++)if(id[i]<0){
  int t=reg.size();reg.push_back(vector<int>(1,i));id[i]=t;
  for(unsigned j=0;j<reg[t].size();j++){int u=reg[t][j];for(unsigned z=0;z<adj[u].size();z++){int v=adj[u][z];if(id[v]<0&&a[v]==a[u]){id[v]=t;reg[t].push_back(v);}}}
 }
 vector<Move> moves;
 for(unsigned t=0;t<reg.size();t++){
  int p=reg[t][0],old=a[p]; map<int,int> targets;
  for(unsigned j=0;j<reg[t].size();j++){int u=reg[t][j];for(unsigned z=0;z<adj[u].size();z++){int v=adj[u][z];if(id[v]!=(int)t)targets[a[v]]|=0;}}
  for(map<int,int>::iterator q=targets.begin();q!=targets.end();q++){
   int c=q->first; int b=bound-(old!=0&&counts[old]==(int)reg[t].size()?1:0);
   if(b>left-1)continue;
   int gain=0;vector<int> seen(reg.size(),0);
   for(unsigned j=0;j<reg[t].size();j++){int u=reg[t][j];for(unsigned z=0;z<adj[u].size();z++){int v=adj[u][z];if(a[v]==c&&!seen[id[v]]){seen[id[v]]=1;gain++;}}}
   moves.push_back(Move(p,c,gain*100+(bound-b)*10+reg[t].size()));
  }
 }
 stable_sort(moves.begin(),moves.end(),cmp);
 for(unsigned j=0;j<moves.size();j++){
  Move m=moves[j];vector<int>b=a;vector<int>& cells=reg[id[m.p]];
  for(unsigned z=0;z<cells.size();z++)b[cells[z]]=m.c;
  path.push_back(m);if(dfs(b,left-1))return true;path.pop_back();
 }
 if(memo.size()<300000)memo[s]=left;
 return false;
}
int main(){while(scanf("%d%d",&R,&C)==2){N=R*C;vector<int>a(N);for(int i=0;i<N;i++)scanf("%d",&a[i]);adj.assign(N,vector<int>());
 for(int i=0;i<R;i++)for(int j=0;j<C;j++){int p=i*C+j;if(i)adj[p].push_back(p-C);if(i+1<R)adj[p].push_back(p+C);if(j)adj[p].push_back(p-1);if(j+1<C)adj[p].push_back(p+1);}
 map<int,int> colors;for(int i=0;i<N;i++)if(a[i])colors[a[i]]=1;
 memo.clear();for(int d=colors.size();;d++){path.clear();if(dfs(a,d))break;}
 printf("%d\n",(int)path.size());for(unsigned i=0;i<path.size();i++)printf("%d %d %d\n",path[i].p/C+1,path[i].p%C+1,path[i].c);
 }return 0;}
