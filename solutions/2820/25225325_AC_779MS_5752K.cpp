// POJ 2820 - TN's Kingdom II - Construction
// Model: gpt-6.1-sol
// Boruvka's Euclidean MST with exact nearest-outside-component queries.
// A balanced k-d tree prunes bounding boxes and subtrees in one component.
// A Euclidean MST has no proper crossings, so it satisfies the road rule.
// Squared integer distances fit exactly in double, avoiding 32-bit overflow.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
struct Point { int x,y; bool operator<(const Point& b)const {return x!=b.x?x<b.x:y<b.y;} bool operator==(const Point& b)const{return x==b.x&&y==b.y;} };
struct Node {int l,r,x0,x1,y0,y1,group;};
vector<Point> p; vector<Node> t; vector<int> parent,comp,bto; vector<double> bd;
int axis;
bool cmp(const Point&a,const Point&b){return axis==0?(a.x!=b.x?a.x<b.x:a.y<b.y):(a.y!=b.y?a.y<b.y:a.x<b.x);}
int build(int l,int r){if(l>=r)return -1; int x0=p[l].x,x1=x0,y0=p[l].y,y1=y0; for(int i=l+1;i<r;++i){x0=min(x0,p[i].x);x1=max(x1,p[i].x);y0=min(y0,p[i].y);y1=max(y1,p[i].y);} axis=(x1-x0<y1-y0);int m=(l+r)/2;nth_element(p.begin()+l,p.begin()+m,p.begin()+r,cmp);t[m].x0=x0;t[m].x1=x1;t[m].y0=y0;t[m].y1=y1;t[m].l=build(l,m);t[m].r=build(m+1,r);return m;}
int findroot(int a){int r=a;while(parent[r]!=r)r=parent[r];while(parent[a]!=a){int b=parent[a];parent[a]=r;a=b;}return r;}
int mark(int u){if(u<0)return -1; int g=comp[u],a=mark(t[u].l),b=mark(t[u].r);if((t[u].l>=0&&a!=g)||(t[u].r>=0&&b!=g))g=-1;return t[u].group=g;}
double box(int u,const Point&q){if(u<0)return 1e100;double x=max(0,max(t[u].x0-q.x,q.x-t[u].x1)),y=max(0,max(t[u].y0-q.y,q.y-t[u].y1));return x*x+y*y;}
void query(int u,int v,double &best,int &who){if(u<0||t[u].group==comp[v])return;double x=p[u].x-p[v].x,y=p[u].y-p[v].y,d=x*x+y*y;if(comp[u]!=comp[v]&&d<best){best=d;who=u;}int a=t[u].l,b=t[u].r;double da=box(a,p[v]),db=box(b,p[v]);if(da>db){swap(a,b);swap(da,db);}if(da<best)query(a,v,best,who);if(db<best)query(b,v,best,who);}
int main(){int n;if(scanf("%d",&n)!=1)return 0;p.resize(n);for(int i=0;i<n;++i)scanf("%d%d",&p[i].x,&p[i].y);sort(p.begin(),p.end());p.erase(unique(p.begin(),p.end()),p.end());n=p.size();t.resize(n);parent.resize(n);comp.resize(n);bto.resize(n);bd.resize(n);int root=build(0,n);for(int i=0;i<n;++i)parent[i]=i;int left=n;double ans=0;while(left>1){for(int i=0;i<n;++i){comp[i]=findroot(i);bd[i]=1e100;bto[i]=-1;}mark(root);for(int i=0;i<n;++i){double d=bd[comp[i]];int j=-1;query(root,i,d,j);if(j>=0){bd[comp[i]]=d;bto[comp[i]]=j;}}for(int i=0;i<n;++i)if(bto[i]>=0){int a=findroot(i),b=findroot(bto[i]);if(a!=b){parent[a]=b;--left;ans+=sqrt(bd[i]);}}}printf("%.4f\n",ans);return 0;}
