// POJ 2682 - Tomato Automata
// Model: gpt-6.1-sol
// Collapse strictly nested loops bottom-up, multiplying the longest single
// iteration by its count. In each scope, reachable control-flow cycles mean
// infinity; otherwise a topological longest path gives its maximum length.
// The loop instruction prints on every iteration, and the last program line
// wraps to line one unless it is die. Unreachable cycles do not matter.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <cctype>
#include <algorithm>
using namespace std;
const double CAP=1000000001.0;
struct Cmd { string op; int arg; double count; Cmd():arg(0),count(1){} };
void solve(const vector<Cmd>& a) {
 int n=(int)a.size()-1;
 vector<int> endAt(n+1,0), st(1,0), order, pos(n+1);
 vector< vector<int> > nodes(n+1);
 vector<double> val(n+1,1);
 for(int i=1;i<=n;++i) if(a[i].op=="loop") endAt[a[i].arg]=i;
 for(int i=1;i<=n;++i) {
  if(endAt[i]) { nodes[st.back()].push_back(i); st.push_back(endAt[i]); }
  nodes[st.back()].push_back(i);
  if(a[i].op=="loop") { order.push_back(i); st.pop_back(); }
 }
 order.push_back(0);
 for(int z=0;z<(int)order.size();++z) {
  int scope=order[z], m=(int)nodes[scope].size();
  vector< vector<int> > edges(m);
  vector<double> w(m,1), dist(m,0);
  vector<int> seen(m,0), deg(m,0), reachable;
  for(int j=0;j<m;++j) pos[nodes[scope][j]]=j;
  for(int j=0;j<m;++j) {
   int line=nodes[scope][j];
   bool child=endAt[line] && endAt[line]!=scope;
   if(child) w[j]=val[endAt[line]];
   if(line==scope || (!child && a[line].op=="die")) continue;
   if(!child && (a[line].op=="ifgo" || a[line].op=="jump")) edges[j].push_back(pos[a[line].arg]);
   if(child || a[line].op!="jump") edges[j].push_back(j+1<m?j+1:0);
  }
  reachable.push_back(0); seen[0]=1; bool infinite=false;
  for(int k=0;k<(int)reachable.size();++k) {
   int u=reachable[k]; if(w[u]<0) infinite=true;
   for(int j=0;j<(int)edges[u].size();++j) {
    int v=edges[u][j]; ++deg[v];
    if(!seen[v]) {seen[v]=1; reachable.push_back(v);}
   }
  }
  queue<int> q;
  for(int j=0;j<m;++j) if(seen[j] && !deg[j]) q.push(j);
  dist[0]=w[0]; int done=0; double best=0;
  while(!q.empty()) {
   int u=q.front();q.pop();++done;best=max(best,dist[u]);
   for(int j=0;j<(int)edges[u].size();++j) {
    int v=edges[u][j];dist[v]=min(CAP,max(dist[v],dist[u]+w[v]));
    if(--deg[v]==0) q.push(v);
   }
  }
  if(done!=(int)reachable.size()) infinite=true;
  if(scope) val[scope]=infinite?-1:min(CAP,best*a[scope].count);
  else if(infinite) cout<<"infinity\n";
  else cout<<(int)best<<"\n";
 }
}
int main() {
 string line; vector<Cmd> a(1);
 while(getline(cin,line)) {
  istringstream in(line); Cmd c;
  if(!(in>>c.op)) {if(a.size()>1){solve(a);a.assign(1,Cmd());}continue;}
  for(int j=0;j<(int)c.op.size();++j)c.op[j]=(char)tolower(c.op[j]);
  if(c.op=="loop")in>>c.arg>>c.count;
  else if(c.op=="ifgo" || c.op=="jump")in>>c.arg;
  a.push_back(c);
 }
 if(a.size()>1)solve(a);
 return 0;
}
