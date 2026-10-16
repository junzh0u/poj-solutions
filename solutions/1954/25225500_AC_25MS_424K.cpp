// POJ 1954 - Warez Test
// Model: gpt-6.1-sol
// A* over pushes, with shortest walking paths between pushes. Reverse-push
// distances and minimum target matching give an admissible lower bound.
// Boxes are indistinguishable. Equal-length answers use lexicographic order,
// as requested by the discussion despite the statement's uniqueness claim.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;
const int INF=1000000;
int R,C,N,nb,adj[225][4],td[225][225];
bool wall[225],goal[225];
vector<int> targets;
const char moves[]="ensw";
int dr[]={0,-1,1,0},dc[]={1,0,0,-1};
int matchcost(const string &s,int i,int mask,vector<int>&memo) {
 if(i==nb) return 0;
 if(memo[mask]>=0) return memo[mask];
 int best=INF;
 for(int j=0;j<nb;++j) if(!(mask&(1<<j)))
  best=min(best,td[j][(unsigned char)s[i]]+matchcost(s,i+1,mask|(1<<j),memo));
 return memo[mask]=best;
}
int heuristic(const string&s) {
 vector<int> memo(1<<nb,-1); int h=matchcost(s,0,0,memo);
 if(h==0 || h>=INF)return h;
 int p=(unsigned char)s[nb],near=INF;
 for(int i=0;i<nb;++i){int b=(unsigned char)s[i];
  near=min(near,abs(p/C-b/C)+abs(p%C-b%C)-1);}
 return h+max(0,near);
}
struct Item { string s,path; int g,f; };
struct Compare {bool operator()(const Item&a,const Item&b)const {
 if(a.f!=b.f) return a.f>b.f; return a.path>b.path;
}};
string solve(string start) {
 priority_queue<Item,vector<Item>,Compare> pq;
 map<string,pair<int,string> > best;
 Item first;first.s=start;first.g=0;first.path="";first.f=heuristic(start);
 pq.push(first);best[start]=make_pair(0,string());
 while(!pq.empty()) {
  Item cur=pq.top();pq.pop();
  if(best[cur.s]!=make_pair(cur.g,cur.path)) continue;
  bool done=true,occupied[225]={false};
  for(int i=0;i<nb;++i){int b=(unsigned char)cur.s[i];occupied[b]=true;if(!goal[b])done=false;}
  if(done)return cur.path;
  string walk[225];bool seen[225]={false};int q[225],head=0,tail=0;
  int p=(unsigned char)cur.s[nb];q[tail++]=p;seen[p]=true;
  while(head<tail){int u=q[head++];for(int d=0;d<4;++d){int v=adj[u][d];
   if(v<0||occupied[v]||seen[v])continue;seen[v]=true;walk[v]=walk[u]+moves[d];q[tail++]=v;}}
  for(int i=0;i<nb;++i){int b=(unsigned char)cur.s[i];for(int d=0;d<4;++d){
   int dest=adj[b][d],behind=adj[b][3-d];
   if(dest<0||behind<0||occupied[dest]||!seen[behind])continue;
   string ns=cur.s;ns[i]=(char)dest;sort(ns.begin(),ns.begin()+nb);ns[nb]=(char)b;
   int h=heuristic(ns);if(h>=INF)continue;
   string path=cur.path+walk[behind]+moves[d];int g=(int)path.size();
   map<string,pair<int,string> >::iterator it=best.find(ns);
   if(it!=best.end()&&it->second<=make_pair(g,path))continue;
   best[ns]=make_pair(g,path);Item next;next.s=ns;next.path=path;next.g=g;next.f=g+h;pq.push(next);
  }}
 }
 return "";
}
int main(){int cases;cin>>cases;for(int cs=1;cs<=cases;++cs){
 cin>>R>>C;N=R*C;targets.clear();fill(goal,goal+225,false);fill(wall,wall+225,true);
 for(int r=0;r<R;++r){string line;cin>>line;for(int c=0;c<C;++c){int v=r*C+c;wall[v]=line[c]=='X';if(line[c]=='T'){goal[v]=true;targets.push_back(v);}}}
 for(int v=0;v<N;++v)for(int d=0;d<4;++d){int r=v/C+dr[d],c=v%C+dc[d];adj[v][d]=(r<0||r>=R||c<0||c>=C||wall[r*C+c])?-1:r*C+c;}
 int r,c;cin>>r>>c;int player=r*C+c;cin>>nb;string s;
 for(int i=0;i<nb;++i){cin>>r>>c;s+=(char)(r*C+c);}sort(s.begin(),s.end());s+=(char)player;
 for(int j=0;j<nb;++j){fill(td[j],td[j]+225,INF);int q[225],head=0,tail=0;q[tail++]=targets[j];td[j][targets[j]]=0;
  while(head<tail){int v=q[head++];for(int d=0;d<4;++d){int u=adj[v][d];if(u<0||adj[u][d]<0||td[j][u]!=INF)continue;td[j][u]=td[j][v]+1;q[tail++]=u;}}}
 cout<<"Scenario #"<<cs<<":\n"<<solve(s)<<"\n\n";
 }return 0;}
