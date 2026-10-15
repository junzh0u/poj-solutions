// POJ 3743 - LL's cake
// Model: gpt-6.1-sol
// Split every chord at its intersections and build a planar half-edge graph.
// Circle arcs are edges too; face traversal integrates x dy - y dx exactly.
// A piece may have several arcs, so no single-segment assumption is made.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0);
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator*(double t)const{return P(x*t,y*t);} };
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
struct E {int to; double angle,area; E(int t,double g,double a):to(t),angle(g),area(a){} };
vector<P> p; vector<E> e; vector<vector<int> > adj;
int point(P a){for(int i=0;i<(int)p.size()&&i<200;i++) if(fabs(p[i].x-a.x)+fabs(p[i].y-a.y)<1e-9)return i; p.push_back(a);return p.size()-1;}
void edge(int a,int b,double x,double y,double area){int k=e.size();e.push_back(E(b,x,area));e.push_back(E(a,y,-area));adj[a].push_back(k);adj[b].push_back(k+1);}
bool cmp(int a,int b){return e[a].angle<e[b].angle;}
int main(){int T;scanf("%d",&T);while(T--){int n;scanf("%d",&n);p.clear();e.clear();vector<P>a(n),b(n);vector<vector<pair<double,int> > > cuts(n);vector<pair<double,int> > rim;
for(int i=0;i<n;i++){double x,y;scanf("%lf%lf",&x,&y);a[i]=P(10*cos(x),10*sin(x));b[i]=P(10*cos(y),10*sin(y));int u=point(a[i]),v=point(b[i]);cuts[i].push_back(make_pair(0.0,u));cuts[i].push_back(make_pair(1.0,v));}
int boundary=p.size();for(int i=0;i<boundary;i++)rim.push_back(make_pair(atan2(p[i].y,p[i].x),i));
for(int i=0;i<n;i++)for(int j=0;j<i;j++){P d=b[i]-a[i],f=b[j]-a[j];double z=cross(d,f);if(fabs(z)<1e-12)continue;double t=cross(a[j]-a[i],f)/z,s=cross(a[j]-a[i],d)/z;if(t>1e-10&&t<1-1e-10&&s>1e-10&&s<1-1e-10){int k=p.size();p.push_back(a[i]+d*t);cuts[i].push_back(make_pair(t,k));cuts[j].push_back(make_pair(s,k));}}
adj.assign(p.size(),vector<int>());
for(int i=0;i<n;i++){sort(cuts[i].begin(),cuts[i].end());for(int j=1;j<(int)cuts[i].size();j++){int u=cuts[i][j-1].second,v=cuts[i][j].second;P d=p[v]-p[u];edge(u,v,atan2(d.y,d.x),atan2(-d.y,-d.x),cross(p[u],p[v])*0.5);}}
sort(rim.begin(),rim.end());for(int i=0;i<boundary;i++){int j=(i+1)%boundary;double delta=rim[j].first-rim[i].first;if(delta<=0)delta+=2*PI;double x=rim[i].first+PI/2,y=rim[j].first-PI/2;x=atan2(sin(x),cos(x));y=atan2(sin(y),cos(y));edge(rim[i].second,rim[j].second,x,y,50*delta);}
vector<int> pos(e.size()),nxt(e.size());for(int i=0;i<(int)p.size();i++){sort(adj[i].begin(),adj[i].end(),cmp);for(int j=0;j<(int)adj[i].size();j++)pos[adj[i][j]]=j;}
for(int i=0;i<(int)e.size();i++){vector<int>&v=adj[e[i].to];nxt[i]=v[(pos[i^1]+v.size()-1)%v.size()];}
vector<int>seen(e.size());double best=0;for(int i=0;i<(int)e.size();i++)if(!seen[i]){int k=i;double area=0;do{seen[k]=1;area+=e[k].area;k=nxt[k];}while(k!=i);if(area>best)best=area;}printf("%.2f\n",best);
}return 0;}
