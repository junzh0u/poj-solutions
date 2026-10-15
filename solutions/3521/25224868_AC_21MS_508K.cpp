// POJ 3521 - Geometric Map
// Model: gpt-6.1-sol
// Identify signs by their open endpoint, split streets at street junctions,
// and run Dijkstra on the permitted directed pieces. A sign forbids motion
// toward its acute-angle side; a perpendicular sign forbids both directions.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct P { int x,y; P(int X=0,int Y=0):x(X),y(Y){} bool operator<(const P& b)const{return x!=b.x?x<b.x:y<b.y;} bool operator==(const P& b)const{return x==b.x&&y==b.y;} };
P sub(P a,P b){return P(a.x-b.x,a.y-b.y);}
int dot(P a,P b){return a.x*b.x+a.y*b.y;}
int cross(P a,P b){return a.x*b.y-a.y*b.x;}
bool on(P p,P a,P b){return cross(sub(p,a),sub(b,a))==0&&dot(sub(p,a),sub(p,b))<=0;}
struct Seg {P a,b; bool street;};
struct Sign {P p,v;};
double adj[405][405];
int main(){int n;while(scanf("%d",&n)==1&&n){P start,goal;scanf("%d%d%d%d",&start.x,&start.y,&goal.x,&goal.y);vector<Seg>s(n);for(int i=0;i<n;++i)scanf("%d%d%d%d",&s[i].a.x,&s[i].a.y,&s[i].b.x,&s[i].b.y);
vector<Sign> signs;vector<P> pts;
for(int i=0;i<n;++i){bool a=false,b=false;for(int j=0;j<n;++j)if(i!=j){a=a||on(s[i].a,s[j].a,s[j].b);b=b||on(s[i].b,s[j].a,s[j].b);}s[i].street=a&&b;if(a&&b){pts.push_back(s[i].a);pts.push_back(s[i].b);}else{Sign t;t.p=a?s[i].a:s[i].b;t.v=a?sub(s[i].b,s[i].a):sub(s[i].a,s[i].b);signs.push_back(t);}}
sort(pts.begin(),pts.end());pts.erase(unique(pts.begin(),pts.end()),pts.end());int m=pts.size();for(int i=0;i<m;++i)for(int j=0;j<m;++j)adj[i][j]=1e100;
for(int k=0;k<n;++k)if(s[k].street){vector<pair<int,int> >order;for(int i=0;i<m;++i)if(on(pts[i],s[k].a,s[k].b))order.push_back(make_pair(dot(sub(pts[i],s[k].a),sub(s[k].b,s[k].a)),i));sort(order.begin(),order.end());for(unsigned q=1;q<order.size();++q){int a=order[q-1].second,b=order[q].second;P v=sub(pts[b],pts[a]);bool ab=true,ba=true;for(unsigned z=0;z<signs.size();++z)if(on(signs[z].p,pts[a],pts[b])){int d=dot(v,signs[z].v);if(d>=0)ab=false;if(d<=0)ba=false;}double len=sqrt((double)dot(v,v));if(ab)adj[a][b]=len;if(ba)adj[b][a]=len;}}
int st=lower_bound(pts.begin(),pts.end(),start)-pts.begin(),g=lower_bound(pts.begin(),pts.end(),goal)-pts.begin();vector<double>d(m,1e100);vector<int>prev(m,-1),used(m,0);d[st]=0;for(int k=0;k<m;++k){int u=-1;for(int i=0;i<m;++i)if(!used[i]&&(u<0||d[i]<d[u]))u=i;if(u<0||d[u]>1e99)break;used[u]=1;for(int v=0;v<m;++v)if(d[v]>d[u]+adj[u][v]){d[v]=d[u]+adj[u][v];prev[v]=u;}}
if(d[g]>1e99)puts("-1");else{vector<int>path;for(int u=g;u!=-1;u=prev[u])path.push_back(u);for(int i=(int)path.size()-1;i>=0;--i)printf("%d %d\n",pts[path[i]].x,pts[path[i]].y);puts("0");}}
return 0;}
