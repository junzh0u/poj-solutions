// POJ 3939 - Extended Manhattan Distance
// Model: gpt-6.1-sol
// Dijkstra on boundary intersections, with opposite-side street crossings.
// Outside travel is a visibility path around the rectangle's four corners.
#include <cstdio>
#include <cmath>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;
struct P { int x,y; P(int a=0,int b=0):x(a),y(b){} };
int xl,yl,xu,yu;
double eu(P a,P b){double x=a.x-b.x,y=a.y-b.y;return sqrt(x*x+y*y);}
double man(P a,P b){return abs(a.x-b.x)+abs(a.y-b.y);}
bool inside(P p){return p.x>=xl&&p.x<=xu&&p.y>=yl&&p.y<=yu;}
bool visible(P a,P b){
 double lo=0,hi=1;
 int av[2]={a.x,a.y},bv[2]={b.x,b.y},l[2]={xl,yl},u[2]={xu,yu};
 for(int k=0;k<2;k++){
  if(av[k]==bv[k]){if(av[k]<=l[k]||av[k]>=u[k])return true;}
  else {double t1=double(l[k]-av[k])/(bv[k]-av[k]),t2=double(u[k]-av[k])/(bv[k]-av[k]);if(t1>t2)swap(t1,t2);lo=max(lo,t1);hi=min(hi,t2);}
 }
 return hi-lo<1e-10;
}
vector<P> corners;
double outside(P a,P b){
 vector<P> p=corners;p.push_back(a);p.push_back(b);double d[6];bool used[6]={false};for(int i=0;i<6;i++)d[i]=1e100;d[4]=0;
 for(int z=0;z<6;z++){int v=-1;for(int i=0;i<6;i++)if(!used[i]&&(v<0||d[i]<d[v]))v=i;used[v]=true;for(int i=0;i<6;i++)if(visible(p[v],p[i]))d[i]=min(d[i],d[v]+eu(p[v],p[i]));}
 return d[5];
}
int main(){P a,b;while(scanf("%d%d%d%d%d%d%d%d",&xl,&yl,&xu,&yu,&a.x,&a.y,&b.x,&b.y)==8){if(!xl&&!yl&&!xu&&!yu&&!a.x&&!a.y&&!b.x&&!b.y)break;
 corners.clear();corners.push_back(P(xl,yl));corners.push_back(P(xu,yl));corners.push_back(P(xu,yu));corners.push_back(P(xl,yu));
 vector<P> p;vector<vector<pair<int,double> > > g;
 int w=xu-xl,h=yu-yl;
 for(int x=xl;x<=xu;x++)p.push_back(P(x,yl));
 for(int y=yl+1;y<=yu;y++)p.push_back(P(xu,y));
 for(int x=xu-1;x>=xl;x--)p.push_back(P(x,yu));
 for(int y=yu-1;y>yl;y--)p.push_back(P(xl,y));
 int n=p.size();g.resize(n);
 for(int i=0;i<n;i++){int j=(i+1)%n;g[i].push_back(make_pair(j,1));g[j].push_back(make_pair(i,1));}
 for(int x=0;x<=w;x++){int i=x,j=w+h+w-x;g[i].push_back(make_pair(j,double(h)));g[j].push_back(make_pair(i,double(h)));}
 for(int y=0;y<=h;y++){int i=(n-y)%n,j=w+y;g[i].push_back(make_pair(j,double(w)));g[j].push_back(make_pair(i,double(w)));}
 vector<double>d(n),end(n);priority_queue<pair<double,int>,vector<pair<double,int> >,greater<pair<double,int> > >q;
 for(int i=0;i<n;i++){d[i]=inside(a)?man(a,p[i]):outside(a,p[i]);end[i]=inside(b)?man(b,p[i]):outside(b,p[i]);q.push(make_pair(d[i],i));}
 double ans=inside(a)&&inside(b)?man(a,b):(!inside(a)&&!inside(b)?outside(a,b):1e100);
 while(!q.empty()){double v=q.top().first;int i=q.top().second;q.pop();if(v>d[i]+1e-9)continue;ans=min(ans,v+end[i]);for(size_t k=0;k<g[i].size();k++){int j=g[i][k].first;double z=v+g[i][k].second;if(z<d[j]){d[j]=z;q.push(make_pair(z,j));}}}
 printf("%.3f\n",ans);
 }return 0;}
