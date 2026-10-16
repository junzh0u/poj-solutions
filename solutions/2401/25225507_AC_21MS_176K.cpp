// POJ 2401 - Street Polygon
// Model: gpt-6.1-sol
// Partition each boundary edge at lines through pairs of polygon vertices.
// Within each open cell weak visibility is constant. For a fixed point,
// partition the opposite edges by rays through vertices and test each cell.
// The two common endpoints are excluded, as the discussion's NO cases require.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
const double E=1e-9;
struct P{double x,y; P(){} P(double a,double b):x(a),y(b){} P operator+(P b)const{return P(x+b.x,y+b.y);} P operator-(P b)const{return P(x-b.x,y-b.y);} P operator*(double t)const{return P(x*t,y*t);} };
double cross(P a,P b){return a.x*b.y-a.y*b.x;} double dot(P a,P b){return a.x*b.x+a.y*b.y;}
vector<P> v; int n,s,t;
bool inside(P p){bool z=false;for(int i=0;i<n;i++){P a=v[i],b=v[(i+1)%n];if(fabs(cross(b-a,p-a))<E && dot(p-a,p-b)<E)return true;if((a.y>p.y)!=(b.y>p.y) && p.x<(b.x-a.x)*(p.y-a.y)/(b.y-a.y)+a.x)z=!z;}return z;}
void cut(vector<double>& a,P p,P d,P q,P e){double den=cross(d,e);if(fabs(den)>E){double x=cross(q-p,e)/den;if(x>E&&x<1-E)a.push_back(x);}}
bool visible(P p,P q){P d=q-p;if(dot(d,d)<E)return true;vector<double>a;a.push_back(0);a.push_back(1);for(int i=0;i<n;i++){P b=v[i],e=v[(i+1)%n]-b;double den=cross(d,e);if(fabs(den)>E){double x=cross(b-p,e)/den,y=cross(b-p,d)/den;if(x>E&&x<1-E&&y>-E&&y<1+E)a.push_back(x);}else if(fabs(cross(b-p,d))<E){double x=dot(b-p,d)/dot(d,d);if(x>E&&x<1-E)a.push_back(x);}}
sort(a.begin(),a.end());for(unsigned i=1;i<a.size();i++)if(a[i]-a[i-1]>E&&!inside(p+d*((a[i]+a[i-1])/2)))return false;return true;}
bool sees(P p,const vector<int>& chain){for(unsigned i=1;i+1<chain.size();i++)if(visible(p,v[chain[i]]))return true;for(unsigned i=1;i<chain.size();i++){P a=v[chain[i-1]],d=v[chain[i]]-a;vector<double>c;c.push_back(0);c.push_back(1);for(int j=0;j<n;j++)cut(c,a,d,p,v[j]-p);sort(c.begin(),c.end());for(unsigned j=1;j<c.size();j++)if(c[j]-c[j-1]>E&&visible(p,a+d*((c[j]+c[j-1])/2)))return true;}return false;}
bool check(const vector<int>&a,const vector<int>&b){for(unsigned i=1;i+1<a.size();i++)if(!sees(v[a[i]],b))return false;for(unsigned i=1;i<a.size();i++){P p=v[a[i-1]],d=v[a[i]]-p;vector<double>c;c.push_back(0);c.push_back(1);for(int j=0;j<n;j++)for(int k=j+1;k<n;k++)cut(c,p,d,v[j],v[k]-v[j]);sort(c.begin(),c.end());for(unsigned j=1;j+1<c.size();j++)if(!sees(p+d*c[j],b))return false;for(unsigned j=1;j<c.size();j++)if(c[j]-c[j-1]>E&&!sees(p+d*((c[j]+c[j-1])/2),b))return false;}return true;}
int main(){int T;scanf("%d",&T);while(T--){scanf("%d",&n);v.resize(n);for(int i=0;i<n;i++)scanf("%lf%lf",&v[i].x,&v[i].y);scanf("%d%d",&s,&t);--s;--t;vector<int>a,b;int i=s;a.push_back(i);while(i!=t){i=(i+1)%n;a.push_back(i);}i=s;b.push_back(i);while(i!=t){i=(i+n-1)%n;b.push_back(i);}puts(check(a,b)&&check(b,a)?"YES":"NO");}}
