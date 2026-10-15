// POJ 3391 - The Bug Sensor Problem
// Model: gpt-6.1-sol
// Build a Euclidean MST by Boruvka's cut rule. A balanced kd-tree finds
// each component's shortest outgoing edge, pruning homogeneous subtrees.
// Removing the k-1 largest MST edges gives k radio-connected components.
// Coordinates are integral; squared distances fit exactly in double.
// The receiver transmitters may be placed at any sensors, one per component.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
struct Point { int x,y; };
struct Node { int l,r,p,lo[2],hi[2],comp; };
struct Edge { int a,b; double w; Edge(int A=0,int B=0,double W=0):a(A),b(B),w(W){} };
vector<Point> pts;
vector<Node> tr;
vector<int> ids,par,sz,comp;
int axis;
struct Compare { bool operator()(int a,int b) const { return axis==0 ? (pts[a].x!=pts[b].x?pts[a].x<pts[b].x:pts[a].y<pts[b].y) : (pts[a].y!=pts[b].y?pts[a].y<pts[b].y:pts[a].x<pts[b].x); } };
int root(int a) { return par[a]==a?a:par[a]=root(par[a]); }
bool join(int a,int b) { a=root(a);b=root(b);if(a==b)return false;if(sz[a]<sz[b])swap(a,b);par[b]=a;sz[a]+=sz[b];return true; }
int build(int l,int r,int dep) {
 if(l>=r)return -1;
 int m=(l+r)/2;axis=dep%2;nth_element(ids.begin()+l,ids.begin()+m,ids.begin()+r,Compare());
 int v=(int)tr.size();tr.push_back(Node());tr[v].p=ids[m];
 int a=build(l,m,dep+1),b=build(m+1,r,dep+1);tr[v].l=a;tr[v].r=b;
 tr[v].lo[0]=tr[v].hi[0]=pts[ids[m]].x;tr[v].lo[1]=tr[v].hi[1]=pts[ids[m]].y;
 for(int j=0;j<2;j++){int c=j?a:b;if(c<0)continue;for(int d=0;d<2;d++){tr[v].lo[d]=min(tr[v].lo[d],tr[c].lo[d]);tr[v].hi[d]=max(tr[v].hi[d],tr[c].hi[d]);}}
 return v;
}
int paint(int v) {
 if(v<0)return -2;
 int c=comp[tr[v].p],a=paint(tr[v].l),b=paint(tr[v].r);
 if((a!=-2&&a!=c)||(b!=-2&&b!=c))c=-1;
 return tr[v].comp=c;
}
double dist(int a,int b) { double x=pts[a].x-pts[b].x,y=pts[a].y-pts[b].y;return x*x+y*y; }
double bound(int v,int p) {
 if(v<0)return 1e100;
 double ans=0;int xy[2]={pts[p].x,pts[p].y};for(int d=0;d<2;d++){double z=0;if(xy[d]<tr[v].lo[d])z=tr[v].lo[d]-xy[d];if(xy[d]>tr[v].hi[d])z=xy[d]-tr[v].hi[d];ans+=z*z;}return ans;
}
void query(int v,int p,int c,double &best,int &who) {
 if(v<0||tr[v].comp==c)return;
 int q=tr[v].p;
 if(comp[q]!=c){double w=dist(p,q);if(w<best){best=w;who=q;}}
 int a=tr[v].l,b=tr[v].r;double da=bound(a,p),db=bound(b,p);
 if(da>db){swap(a,b);swap(da,db);}
 if(da<best)query(a,p,c,best,who);
 if(db<best)query(b,p,c,best,who);
}
int main(){
 int tests;if(scanf("%d",&tests)!=1)return 0;
 while(tests--){int k,x,y;scanf("%d",&k);pts.clear();while(scanf("%d",&x)==1&&x!=-1){scanf("%d",&y);Point p;p.x=x;p.y=y;pts.push_back(p);}
 int n=(int)pts.size();if(k>=n||n<2){puts("0");continue;}
 ids.resize(n);par.resize(n);sz.assign(n,1);comp.resize(n);for(int i=0;i<n;i++)ids[i]=par[i]=i;
 tr.clear();tr.reserve(n);build(0,n,0);int count=n;vector<double> weights;weights.reserve(n-1);
 vector<Edge> edges(n);
 while(count>1){for(int i=0;i<n;i++)comp[i]=root(i);paint(0);for(int i=0;i<n;i++)edges[i]=Edge(-1,-1,1e100);
 for(int i=0;i<n;i++){int c=comp[i],q=-1;double w=edges[c].w;query(0,i,c,w,q);if(q!=-1)edges[c]=Edge(i,q,w);}
 for(int i=0;i<n;i++)if(edges[i].a!=-1&&join(edges[i].a,edges[i].b)){weights.push_back(edges[i].w);--count;}
 }
 sort(weights.begin(),weights.end());double w=weights[n-k-1];int ans=(int)sqrt(w);while((double)ans*ans<w)++ans;printf("%d\n",ans);
 }
 return 0;
}
