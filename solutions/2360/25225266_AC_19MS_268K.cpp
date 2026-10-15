// POJ 2360 - Trains
// Model: gpt-6.1-sol
// Run time-dependent Dijkstra from every origin departure, then retain
// the nondominated departure/arrival pairs, including next-day departures.
// Routes run forward daily; cumulative route times may cross midnight.
// Dynamic station storage also handles the board's corrected S <= 30.
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
#include <functional>
using namespace std;
struct Edge { int to, departure, duration; Edge(int v,int d,int w):to(v),departure(d),duration(w){} };
int minutes(const string &s) { size_t p=s.find(':'); int h=0,m=0; for(size_t i=0;i<p;++i) h=h*10+s[i]-'0'; for(size_t i=p+1;i<s.size();++i) m=m*10+s[i]-'0'; return h*60+m; }
int station(map<string,int>& ids, vector<vector<Edge> >& g,const string& s) { if(ids.count(s)) return ids[s]; int v=g.size(); ids[s]=v; g.push_back(vector<Edge>()); return v; }
int main() {
 int cases; cin>>cases;
 for(int tc=0;tc<cases;++tc) {
  map<string,int> ids; vector<vector<Edge> > g; int routes; cin>>routes;
  for(int r=0;r<routes;++r) {
   int n; string tm,name; cin>>n>>tm>>name; int time=minutes(tm),u=station(ids,g,name);
   for(int j=1;j<n;++j) { cin>>tm>>name; int w=minutes(tm),v=station(ids,g,name); g[u].push_back(Edge(v,time%1440,w)); time+=w; u=v; }
  }
  string a,b; cin>>a>>b; int start=station(ids,g,a),target=station(ids,g,b);
  vector<int> dep; for(size_t j=0;j<g[start].size();++j) dep.push_back(g[start][j].departure);
  sort(dep.begin(),dep.end()); dep.erase(unique(dep.begin(),dep.end()),dep.end());
  const int INF=1000000000; vector<int> arrival(dep.size(),INF);
  for(size_t k=0;k<dep.size();++k) {
   vector<int> d(g.size(),INF); d[start]=dep[k];
   typedef pair<int,int> State; priority_queue<State,vector<State>,greater<State> > q; q.push(State(d[start],start));
   while(!q.empty()) {
    int now=q.top().first,u=q.top().second; q.pop(); if(now!=d[u]) continue; if(u==target) break;
    for(size_t j=0;j<g[u].size();++j) { Edge e=g[u][j]; int wait=(e.departure-now%1440+1440)%1440; int nd=now+wait+e.duration; if(nd<d[e.to]) { d[e.to]=nd; q.push(State(nd,e.to)); } }
   }
   arrival[k]=d[target];
  }
  int best=INF; for(size_t k=0;k<arrival.size();++k) if(arrival[k]!=INF) best=min(best,arrival[k]+1440);
  vector<int> keep;
  for(int k=(int)dep.size()-1;k>=0;--k) { if(arrival[k]<best) keep.push_back(k); best=min(best,arrival[k]); }
  if(tc) cout<<'\n';
  for(int j=(int)keep.size()-1;j>=0;--j) { int k=keep[j],travel=arrival[k]-dep[k]; cout<<setfill('0')<<setw(2)<<dep[k]/60<<':'<<setw(2)<<dep[k]%60<<' '<<travel/60<<':'<<setw(2)<<travel%60<<'\n'; }
 }
 return 0;
}
