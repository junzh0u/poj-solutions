// POJ 3405 - Convex hull
// Model: gpt-6-astra
// Partition outward-normal angles at every pairwise support-function equality.
// On each interval the largest support belongs to one circle. Integrate its
// circular arc, then the straight common tangent to the next selected circle.
// Zero radii, duplicate circles, containment, and the empty set are allowed.
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long double Real;
struct Circle { Real x,y,r; };
struct Point { Real x,y; Point(Real a=0,Real b=0):x(a),y(b){} };
Real cross(Point a,Point b) { return a.x*b.y-a.y*b.x; }
Point point(Circle c,Real a) { return Point(c.x+c.r*cosl(a),c.y+c.r*sinl(a)); }
int main() {
    int n; if (!(cin>>n)) return 0;
    vector<Circle> c(n);
    for(int i=0;i<n;++i) cin>>c[i].x>>c[i].y>>c[i].r;
    if(!n) { cout<<"0.0000\n"; return 0; }
    Real ox=c[0].x,oy=c[0].y;
    for(int i=0;i<n;++i) { c[i].x-=ox; c[i].y-=oy; }
    Real tau=2*acosl(-1.0L);
    vector<Real> a; a.push_back(0); a.push_back(tau);
    for(int i=0;i<n;++i) for(int j=0;j<i;++j) {
        Real dx=c[i].x-c[j].x,dy=c[i].y-c[j].y;
        Real d=sqrtl(dx*dx+dy*dy),dr=c[j].r-c[i].r;
        if(d==0 || fabsl(dr)>=d) continue;
        Real phi=atan2l(dy,dx),alpha=acosl(dr/d);
        for(int s=-1;s<=1;s+=2) {
            Real t=fmodl(phi+s*alpha+tau,tau); a.push_back(t);
        }
    }
    sort(a.begin(),a.end());
    a.erase(unique(a.begin(),a.end()),a.end());
    vector<int> who;
    for(size_t k=0;k+1<a.size();++k) {
        Real t=(a[k]+a[k+1])/2,ux=cosl(t),uy=sinl(t);
        int best=0; Real h=c[0].x*ux+c[0].y*uy+c[0].r;
        for(int i=1;i<n;++i) {
            Real v=c[i].x*ux+c[i].y*uy+c[i].r;
            if(v>h) { h=v; best=i; }
        }
        who.push_back(best);
    }
    Real area2=0;
    for(size_t k=0;k<who.size();++k) {
        Circle v=c[who[k]];
        Point p=point(v,a[k]),q=point(v,a[k+1]);
        area2+=v.x*(q.y-p.y)-v.y*(q.x-p.x)+v.r*v.r*(a[k+1]-a[k]);
        Circle w=c[who[(k+1)%who.size()]];
        area2+=cross(q,point(w,a[k+1]));
    }
    cout<<fixed<<setprecision(4)<<max(0.0L,area2/2)<<'\n';
}
