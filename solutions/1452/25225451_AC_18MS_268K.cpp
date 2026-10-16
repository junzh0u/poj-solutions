// POJ 1452 - Signal Box
// Model: gpt-6.1-sol
// Represent an arrival by its element and incoming port. Reachability selects
// feasible branches; a rule applies only when both branches reach its merge
// point from the back on a route to the destination. Otherwise prefer minus.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;
struct Element { string name; char type; int adj[3]; string neighbor[3]; };
struct Rule { int x,y; int sign; };
int main() {
 int cases; cin>>cases;
 for(int tc=1;tc<=cases;++tc) {
  string start,end; int n; cin>>start>>end>>n;
  vector<Element> a(n); map<string,int> id;
  for(int i=0;i<n;++i) {
   cin>>a[i].type>>a[i].name>>a[i].neighbor[0]>>a[i].neighbor[1];
   if(a[i].type=='W') cin>>a[i].neighbor[2];
   id[a[i].name]=i;
  }
  for(int i=0;i<n;++i) for(int j=0;j<(a[i].type=='W'?3:2);++j)
   a[i].adj[j]=a[i].neighbor[j]=="XXX"?-1:id[a[i].neighbor[j]];
  int p; cin>>p; vector<Rule> rules(p);
  for(int i=0;i<p;++i) { string fw,x,y,z; cin>>fw>>x>>y>>z; rules[i].x=id[x];rules[i].y=id[y];rules[i].sign=z=="+"?2:1; }
  int s=id[start]*3,t=id[end]*3,N=n*3;
  vector<vector<int> > g(N); vector<vector<int> > output(N);
  for(int i=0;i<n;++i) for(int port=0;port<(a[i].type=='W'?3:2);++port) {
   int u=i*3+port;
   if(i==id[end]) continue;
   vector<int> exits;
   if(a[i].type=='S') exits.push_back(1-port);
   else if(port==0) {exits.push_back(1);exits.push_back(2);} else exits.push_back(0);
   for(unsigned j=0;j<exits.size();++j) {
    int e=exits[j],v=a[i].adj[e]; if(v<0) continue;
    for(int k=0;k<(a[v].type=='W'?3:2);++k) if(a[v].adj[k]==i) {
     g[u].push_back(v*3+k); output[u].push_back(e); break;
    }
   }
  }
  vector<vector<char> > reach(N,vector<char>(N,0));
  for(int i=0;i<N;++i) { queue<int> q; q.push(i);reach[i][i]=1;
   while(!q.empty()) {int u=q.front();q.pop();for(unsigned j=0;j<g[u].size();++j) {int v=g[u][j];if(!reach[i][v]) {reach[i][v]=1;q.push(v);}}}
  }
  cout<<"Scenario #"<<tc<<":\n";
  if(!reach[s][t]) cout<<"NOT POSSIBLE\n";
  else {
   vector<int> signals,points,signs; int u=s;
   while(true) {
    int v=u/3,port=u%3;
    if(a[v].type=='S' && port==0) signals.push_back(v);
    if(u==t) break;
    int pick=-1;
    for(unsigned j=0;j<g[u].size();++j) if(reach[g[u][j]][t]) {if(pick<0) pick=j;else {
     int desired=1;
     for(int r=0;r<p;++r) if(rules[r].x==v) {
      int y=rules[r].y;bool left=false,right=false;
      for(int k=1;k<=2;++k) if(reach[y*3+k][t]) {
       if(reach[g[u][pick]][y*3+k]) left=true;
       if(reach[g[u][j]][y*3+k]) right=true;
      }
      if(left&&right) desired=rules[r].sign;
     }
     if(output[u][j]==desired) pick=j;
    }}
    if(a[v].type=='W') {points.push_back(v);signs.push_back(port==0?output[u][pick]:port);}
    u=g[u][pick];
   }
   for(unsigned i=0;i<signals.size();++i) cout<<a[signals[i]].name<<'\n';
   for(unsigned i=0;i<points.size();++i) cout<<a[points[i]].name<<' '<<(signs[i]==1?'-':'+')<<'\n';
  }
  cout<<'\n';
 }
}
