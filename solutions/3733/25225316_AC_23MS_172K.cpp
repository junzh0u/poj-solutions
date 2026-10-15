// POJ 3733 - Shadow Area
// Model: gpt-6.1-sol
// Project each rectangular prism to its convex ground polygon, then integrate
// the union's vertical sections between all vertices and edge intersections.
// The sample excludes base area. The discuss board reports that the judge
// subtracts the SUM of base areas, even when buildings overlap; follow that rule.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
typedef long double Real;
struct P { Real x,y; P() {} P(Real a,Real b):x(a),y(b){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(Real t)const{return P(x*t,y*t);} bool operator<(const P&b)const{return x!=b.x?x<b.x:y<b.y;} };
Real cross(P a,P b){return a.x*b.y-a.y*b.x;}
Real norm(P a){return a.x*a.x+a.y*a.y;}
vector<P> hull(vector<P> a){sort(a.begin(),a.end()); vector<P> b; int n=a.size(); for(int i=0;i<n;i++){while(b.size()>1&&cross(b.back()-b[b.size()-2],a[i]-b.back())<=0)b.pop_back();b.push_back(a[i]);} int k=b.size();for(int i=n-2;i>=0;i--){while((int)b.size()>k&&cross(b.back()-b[b.size()-2],a[i]-b.back())<=0)b.pop_back();b.push_back(a[i]);}b.pop_back();return b;}
struct E {P a,b;E(P c,P d):a(c),b(d){}};
Real section(const vector<vector<P> >& poly, Real x){vector<pair<Real,Real> > intervals;for(size_t i=0;i<poly.size();i++){Real lo=1e100L,hi=-1e100L;for(size_t j=0;j<poly[i].size();j++){P a=poly[i][j],b=poly[i][(j+1)%poly[i].size()];if((a.x<x&&x<b.x)||(b.x<x&&x<a.x)){Real y=a.y+(b.y-a.y)*(x-a.x)/(b.x-a.x);lo=min(lo,y);hi=max(hi,y);}}if(lo<=hi)intervals.push_back(make_pair(lo,hi));}sort(intervals.begin(),intervals.end());Real total=0,lo=0,hi=0;for(size_t i=0;i<intervals.size();i++){if(i==0){lo=intervals[i].first;hi=intervals[i].second;}else if(intervals[i].first<=hi)hi=max(hi,intervals[i].second);else {total+=hi-lo;lo=intervals[i].first;hi=intervals[i].second;}}if(!intervals.empty())total+=hi-lo;return total;}
int main(){int T;scanf("%d",&T);for(int tc=1;tc<=T;tc++){double dh,dx,dy;scanf("%lf%lf%lf",&dh,&dx,&dy);int n;scanf("%d",&n);vector<vector<P> > poly;vector<E> edges;vector<Real> xs;Real bases=0;P origin(0,0);for(int i=0;i<n;i++){double v[7];for(int j=0;j<7;j++)scanf("%lf",&v[j]);P a[4];for(int j=0;j<3;j++)a[j]=P(v[2*j],v[2*j+1]);if(i==0)origin=a[0];for(int j=0;j<3;j++)a[j]=a[j]-origin;int u=0,w=1,z=2;if(norm(a[0]-a[2])>norm(a[u]-a[w])){u=0;w=2;z=1;}if(norm(a[1]-a[2])>norm(a[u]-a[w])){u=1;w=2;z=0;}a[3]=a[u]+a[w]-a[z];bases+=fabsl(cross(a[u]-a[z],a[w]-a[z]));P shift((Real)dx*v[6]/dh,(Real)dy*v[6]/dh);vector<P> pts;for(int j=0;j<4;j++){pts.push_back(a[j]);pts.push_back(a[j]+shift);}poly.push_back(hull(pts));for(size_t j=0;j<poly.back().size();j++){P p=poly.back()[j],q=poly.back()[(j+1)%poly.back().size()];edges.push_back(E(p,q));xs.push_back(p.x);}}
for(size_t i=0;i<edges.size();i++)for(size_t j=0;j<i;j++){P a=edges[i].a,b=edges[i].b,c=edges[j].a,d=edges[j].b;Real den=cross(b-a,d-c);if(den==0)continue;Real t=cross(c-a,d-c)/den,s=cross(c-a,b-a)/den;if(t>0&&t<1&&s>0&&s<1)xs.push_back(a.x+(b.x-a.x)*t);}
sort(xs.begin(),xs.end());Real area=0;for(size_t i=1;i<xs.size();i++)if(xs[i]>xs[i-1])area+=(xs[i]-xs[i-1])*section(poly,(xs[i]+xs[i-1])/2);Real answer=area-bases;if(fabsl(answer)<0.0000001L)answer=0;printf("Case %d: %.3Lf\n",tc,answer);}return 0;}
