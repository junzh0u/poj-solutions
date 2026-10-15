// POJ 3166 - Jumping Frog
// Model: gpt-6.1-sol
// Build the undirected visibility graph and run Dijkstra's algorithm.
// A straight line cannot pass three alternating barriers. Test at most two
// intervening barriers with exact integer arithmetic; touching is forbidden.
// Undirected edges allow the backward jumps explicitly needed by the sample.
#include <cstdio>
#include <cmath>
#include <vector>
#include <queue>
#include <functional>
#include <utility>
using namespace std;
struct Point { int x,y,s; Point(int a,int b,int c):x(a),y(b),s(c){} };
struct Edge { int v; double w; Edge(int a,double b):v(a),w(b){} };
int main(){
 int n; char str[60];
 while(scanf("%d",&n)==1){
  vector<Point> p; int wall[51],x=0;
  for(int s=0;s<n;++s){
   scanf("%s",str); int k=0;
   for(;str[k];++k){char c=str[k]; int y=c>='a'?'a'-c:c-'A'; p.push_back(Point(x+k+1,y,s));}
   x+=k+1; wall[s]=x;
  }
  int m=p.size(); vector<vector<Edge> > g(m);
  for(int i=0;i<m;++i) for(int j=i+1;j<m && p[j].s-p[i].s<=2;++j){
   bool ok=true; int dx=p[j].x-p[i].x;
   for(int b=p[i].s;b<p[j].s;++b){
    int y=p[i].y*dx+(p[j].y-p[i].y)*(wall[b]-p[i].x);
    if((b%2==0 && y<=0)||(b%2==1 && y>=0)){ok=false;break;}
   }
   if(ok){double dy=p[j].y-p[i].y; double d=sqrt(double(dx)*dx+dy*dy);g[i].push_back(Edge(j,d));g[j].push_back(Edge(i,d));}
  }
  vector<double> d(m,1e100); d[0]=0;
  typedef pair<double,int> Entry; priority_queue<Entry,vector<Entry>,greater<Entry> > q; q.push(Entry(0,0));
  while(!q.empty()){
   Entry e=q.top();q.pop(); int u=e.second;if(e.first>d[u])continue;if(u==m-1)break;
   for(unsigned k=0;k<g[u].size();++k){Edge a=g[u][k];if(d[a.v]>d[u]+a.w){d[a.v]=d[u]+a.w;q.push(Entry(d[a.v],a.v));}}
  }
  if(d[m-1]>1e90)puts("-1");else printf("%.4f\n",d[m-1]);
 }
 return 0;
}
