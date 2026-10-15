// POJ 1921 - Paper Cut
// Model: gpt-6.1-sol
// Split convex paper layers at each fold, retaining original coordinates.
// Unfold the final cut and count planar faces by Euler's formula. Shared
// endpoints and overlapping segments are merged, including degenerate folds.
#include <cstdio>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
const double EPS=1e-8;
struct P { double x,y; P(double a=0,double b=0):x(a),y(b){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double t)const{return P(x*t,y*t);} };
double cross(P a,P b){return a.x*b.y-a.y*b.x;} double dot(P a,P b){return a.x*b.x+a.y*b.y;}
struct V { P p,o; V(P a=P(),P b=P()):p(a),o(b){} };
struct L {P a,b; L(P x=P(),P y=P()):a(x),b(y){} };
L fold[21],cut; int n; vector<L> seg;
double side(P p,L l){return cross(l.b-l.a,p-l.a)/sqrt(dot(l.b-l.a,l.b-l.a));}
vector<V> clip(const vector<V>&p,L l,int sign){vector<V> q; for(int i=0;i<(int)p.size();++i){V a=p[i],b=p[(i+1)%p.size()];double x=side(a.p,l)*sign,y=side(b.p,l)*sign; if(x>=-EPS)q.push_back(a);if((x>EPS&&y<-EPS)||(x<-EPS&&y>EPS)){double t=x/(x-y);q.push_back(V(a.p+(b.p-a.p)*t,a.o+(b.o-a.o)*t));}}return q;}
double area(const vector<V>&p){double s=0;for(int i=0;i<(int)p.size();++i)s+=cross(p[i].p,p[(i+1)%p.size()].p);return fabs(s);}
void solve(vector<V> p,int k){if(p.size()<3||area(p)<1e-15)return;if(k==n){vector<P> v;for(int i=0;i<(int)p.size();++i){V a=p[i],b=p[(i+1)%p.size()];double x=side(a.p,cut),y=side(b.p,cut);if(fabs(x)<EPS)v.push_back(a.o);if((x>EPS&&y<-EPS)||(x<-EPS&&y>EPS)){double t=x/(x-y);v.push_back(a.o+(b.o-a.o)*t);}}if(v.size()>1){int a=0,b=1;double best=0;for(int i=0;i<(int)v.size();++i)for(int j=0;j<i;++j){double d=dot(v[i]-v[j],v[i]-v[j]);if(d>best){best=d;a=i;b=j;}}if(best>EPS*EPS)seg.push_back(L(v[a],v[b]));}return;}
vector<V> right=clip(p,fold[k],-1),left=clip(p,fold[k],1);P d=fold[k].b-fold[k].a;double dd=dot(d,d);for(int i=0;i<(int)left.size();++i){P u=left[i].p-fold[k].a;left[i].p=fold[k].a+d*(2*dot(u,d)/dd)-u;}solve(right,k+1);solve(left,k+1);}
vector<P> points; vector<int> parent;
int vertex(P p){for(int i=0;i<(int)points.size();++i)if(dot(p-points[i],p-points[i])<EPS*EPS)return i;int i=points.size();points.push_back(p);parent.push_back(i);return i;}
int root(int a){return parent[a]==a?a:parent[a]=root(parent[a]);}
int countFaces(){points.clear();parent.clear();int m=seg.size();vector<vector<double> > ts(m);for(int i=0;i<m;++i){ts[i].push_back(0);ts[i].push_back(1);}for(int i=0;i<m;++i)for(int j=0;j<i;++j){P a=seg[i].a,b=seg[j].a,u=seg[i].b-a,v=seg[j].b-b;double z=cross(u,v);if(fabs(z)>EPS*sqrt(dot(u,u)*dot(v,v))){double t=cross(b-a,v)/z,s=cross(b-a,u)/z;if(t>=-EPS&&t<=1+EPS&&s>=-EPS&&s<=1+EPS){ts[i].push_back(max(0.,min(1.,t)));ts[j].push_back(max(0.,min(1.,s)));}}else if(fabs(cross(b-a,u))<EPS*sqrt(dot(u,u))){double t=dot(b-a,u)/dot(u,u),s=dot(seg[j].b-a,u)/dot(u,u);if(t>=0&&t<=1)ts[i].push_back(t);if(s>=0&&s<=1)ts[i].push_back(s);t=dot(a-b,v)/dot(v,v);s=dot(seg[i].b-b,v)/dot(v,v);if(t>=0&&t<=1)ts[j].push_back(t);if(s>=0&&s<=1)ts[j].push_back(s);}}
set<pair<int,int> > edges;for(int i=0;i<m;++i){sort(ts[i].begin(),ts[i].end());int last=-1;for(int j=0;j<(int)ts[i].size();++j){int id=vertex(seg[i].a+(seg[i].b-seg[i].a)*ts[i][j]);if(last>=0&&id!=last){int a=min(last,id),b=max(last,id);edges.insert(make_pair(a,b));parent[root(a)]=root(b);}last=id;}}int c=0;for(int i=0;i<(int)parent.size();++i)if(root(i)==i)++c;return (int)edges.size()-(int)points.size()+c;}
int main(){int t;scanf("%d",&t);while(t--){scanf("%d",&n);for(int i=0;i<=n;++i){L l;scanf("%lf%lf%lf%lf",&l.a.x,&l.a.y,&l.b.x,&l.b.y);if(i<n)fold[i]=l;else cut=l;}seg.clear();vector<V> p;P corners[4]={P(0,0),P(1,0),P(1,1),P(0,1)};for(int i=0;i<4;++i)p.push_back(V(corners[i],corners[i]));solve(p,0);for(int i=0;i<4;++i)seg.push_back(L(corners[i],corners[(i+1)%4]));printf("%d\n",countFaces());}return 0;}
