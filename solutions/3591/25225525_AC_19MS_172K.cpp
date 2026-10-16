// POJ 3591 - Ants and Aphides
// Model: gpt-6.1-sol
// Intersect the holes, then find the shortest route around the convex
// rain footprint in both unfoldings of the two exposed vertical faces.
// The judge's discussion specifies using the footprint without clipping
// it to the roof rectangle; redundant collinear input points are removed.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double E=1e-8;
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double t)const{return P(x*t,y*t);} bool operator<(P b)const{return x<b.x || (x==b.x && y<b.y);} };
double cross(P a,P b){return a.x*b.y-a.y*b.x;}
double dist(P a,P b){return hypot(a.x-b.x,a.y-b.y);}
vector<P> hull(vector<P> p){sort(p.begin(),p.end());vector<P> q; int n=p.size();for(int i=0;i<n;i++){while(q.size()>1 && cross(q.back()-q[q.size()-2],p[i]-q.back())<=E)q.pop_back();q.push_back(p[i]);} int s=q.size();for(int i=n-2;i>=0;i--){while((int)q.size()>s && cross(q.back()-q[q.size()-2],p[i]-q.back())<=E)q.pop_back();q.push_back(p[i]);}if(q.size()>1)q.pop_back();return q;}
vector<P> clip(vector<P> p,P a,P b){vector<P> q;if(p.empty())return q; for(int i=0;i<(int)p.size();i++){P u=p[i],v=p[(i+1)%p.size()];double c=cross(b-a,u-a),d=cross(b-a,v-a);if(c>=-E)q.push_back(u);if((c>E && d<-E)||(c<-E && d>E))q.push_back(u+(v-u)*(c/(c-d)));}return q;}
double route(vector<P> p,P b){if(p.size()<3)return dist(P(),b); p.push_back(P());p.push_back(b);p=hull(p);int a=-1,c=-1;for(int i=0;i<(int)p.size();i++){if(dist(p[i],P())<E)a=i;if(dist(p[i],b)<E)c=i;}if(a<0||c<0)return 1e100;double total=0,part=0;for(int i=0;i<(int)p.size();i++){double d=dist(p[i],p[(i+1)%p.size()]);total+=d;}for(int i=a;i!=c;i=(i+1)%p.size())part+=dist(p[i],p[(i+1)%p.size()]);return min(part,total-part);}
int main(){int T;scanf("%d",&T);while(T--){double L,W,H,x,y,z;int K;scanf("%lf%lf%lf%d",&L,&W,&H,&K);vector<P> p;for(int k=0;k<K;k++){int n;scanf("%d",&n);vector<P> q;for(int i=0;i<n;i++){double a,b;scanf("%lf%lf",&a,&b);q.push_back(P(a,b));}q=hull(q);if(!k)p=q;else for(int i=0;i<(int)q.size();i++)p=clip(p,q[i],q[(i+1)%q.size()]);}scanf("%lf%lf%lf",&x,&y,&z);double d=H-z,ans;if(fabs(x-L)<E)ans=min(route(p,P(L+d,y)),route(p,P(L+W-y,W+d)));else ans=min(route(p,P(x,W+d)),route(p,P(L+d,W+L-x)));printf("%.3f\n",ans+1e-9);}return 0;}
