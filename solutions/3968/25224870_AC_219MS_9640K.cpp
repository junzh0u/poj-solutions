// POJ 3968 - Jungle Outpost
// Model: gpt-6.1-sol
// Binary search the depth using linear half-plane intersection of k-step
// chords, whose directions follow the convex polygon's cyclic order.
// Protection is strict, so intersections of zero area are infeasible.
#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;
typedef long double R;
struct P{R x,y;P(){}P(R a,R b):x(a),y(b){}P operator+(P b)const{return P(x+b.x,y+b.y);}P operator-(P b)const{return P(x-b.x,y-b.y);}P operator*(R t)const{return P(x*t,y*t);}};
R cr(P a,P b){return a.x*b.y-a.y*b.x;}
struct L{P p,v;R a;};
vector<P>p;vector<L>ls,q;int n;
P meet(L a,L b){return a.p+a.v*(cr(b.p-a.p,b.v)/cr(a.v,b.v));}
bool out(L a,P b){return cr(a.v,b-a.p)<-1e-9L;}
bool ok(int k){int s=0;for(int i=0;i<n;++i){ls[i].p=p[i];ls[i].v=p[(i+k)%n]-p[i];ls[i].a=atan2l(ls[i].v.y,ls[i].v.x);if(ls[i].a<ls[s].a)s=i;}int h=0,t=0;
for(int z=0;z<n;++z){L a=ls[(s+z)%n];while(t-h>1&&out(a,meet(q[t-2],q[t-1])))--t;while(t-h>1&&out(a,meet(q[h],q[h+1])))++h;if(t>h&&fabsl(cr(q[t-1].v,a.v))<1e-12L){if(q[t-1].v.x*a.v.x+q[t-1].v.y*a.v.y<0)return false;if(out(a,q[t-1].p))q[t-1]=a;}else q[t++]=a;}
while(t-h>2&&out(q[h],meet(q[t-2],q[t-1])))--t;while(t-h>2&&out(q[t-1],meet(q[h],q[h+1])))++h;if(t-h<3||fabsl(cr(q[h].v,q[t-1].v))<1e-12L)return false;P base=meet(q[t-1],q[h]),last=meet(q[h],q[h+1]);R area=0;for(int i=h+1;i<t-1;++i){P now=meet(q[i],q[i+1]);area+=cr(last-base,now-base);last=now;}return area>1e-8L;}
int main(){if(scanf("%d",&n)!=1)return 0;p.resize(n);ls.resize(n);q.resize(n);for(int i=0;i<n;++i){int x,y;scanf("%d%d",&x,&y);p[i]=P(x,y);}reverse(p.begin(),p.end());int lo=1,hi=(n-1)/2;while(lo<hi){int mid=(lo+hi+1)/2;if(ok(mid))lo=mid;else hi=mid-1;}printf("%d\n",lo);}
