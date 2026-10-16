// POJ 3491 - Rotated Sky Charts
// Model: gpt-6.1-sol
// Sort polar coordinates, normalize the scale, and match circular sequences
// of radii and angular gaps with KMP. The common center is the origin;
// counterclockwise angles are measured in [0,360). Ignore stars at the center.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0), TAU=2*PI;
struct P { double a,r,g; };
bool lessP(const P& x,const P& y) {return x.a<y.a;}
bool lessR(const P& x,const P& y) {return x.r<y.r;}
void read(vector<P>& v,int n) {
 double mx=0;
 for(int i=0;i<n;i++) {double x,y; scanf("%lf%lf",&x,&y); P p; p.r=sqrt(x*x+y*y); if(p.r==0) continue; p.a=atan2(y,x); if(p.a<0) p.a+=TAU; p.g=0; v.push_back(p); mx=max(mx,p.r);}
 sort(v.begin(),v.end(),lessP);
 // Treat roundoff on a common ray consistently, then order its stars by radius.
 for(int i=0;i<(int)v.size();) {int j=i+1; while(j<(int)v.size() && v[j].a-v[i].a<1e-8) j++; sort(v.begin()+i,v.begin()+j,lessR); i=j;}
 for(int i=0;i<(int)v.size();i++) {v[i].r/=mx; int j=(i+1)%v.size(); v[i].g=v[j].a-v[i].a; if(v[i].g< -1e-8) v[i].g+=TAU; if(fabs(v[i].g)<1e-8) v[i].g=0;}
}
bool eq(const P& a,const P& b) {return fabs(a.r-b.r)<1e-6 && fabs(a.g-b.g)<1e-6;}
int main() {
 int n; while(scanf("%d",&n)==1) {
 vector<P>a,b; a.reserve(n);b.reserve(n);read(a,n);read(b,n);
 int m=a.size(); if(!m) {puts("0.00");continue;}
 vector<int> fail(m,0); for(int i=1,j=0;i<m;i++) {while(j && !eq(a[i],a[j]))j=fail[j-1]; if(eq(a[i],a[j]))j++;fail[i]=j;}
 double ans=TAU;
 for(int i=0,j=0;i<2*m-1;i++) {while(j && !eq(b[i%m],a[j]))j=fail[j-1];if(eq(b[i%m],a[j]))j++;if(j==m) {int st=i-m+1; double ang=b[st].a-a[0].a; if(ang< -1e-7)ang+=TAU; if(ang<0 || ang>TAU-1e-7)ang=0;ans=min(ans,ang);j=fail[j-1];}}
 printf("%.2f\n",ans*180/PI);
 }
 return 0;
}
