// POJ 3011 - Secrets in Shadows
// Model: gpt-6-astra
// Project centers onto (sin(theta), cos(theta)); theta runs clockwise
// from east. The union width is 2 + sum(min(adjacent gap, 2)).
// Split at all projection-order and gap-two events. Within each open
// interval the width is A*sin(theta)+B*cos(theta)+C, so endpoints and
// its stationary point exhaust the possible extrema.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>
using namespace std;
typedef long double Real;
const Real PI = acosl(-1.0L);
struct Point { int x,y; } p[105];
int n;
Real norm(Real a) { a=fmodl(a,PI); return a<0 ? a+PI : a; }
Real lo,hi,amin,amax;
void consider(Real a,Real v) {
    if(v<lo) { lo=v; amin=a; }
    if(v>hi) { hi=v; amax=a; }
}
int main() {
    while(scanf("%d",&n)==1 && n) {
        for(int i=0;i<n;++i) scanf("%d%d",&p[i].x,&p[i].y);
        vector<Real> e; e.push_back(0); e.push_back(PI);
        for(int i=0;i<n;++i) for(int j=0;j<i;++j) {
            Real dx=p[i].x-p[j].x,dy=p[i].y-p[j].y;
            Real a=atan2l(-dy,dx), d=sqrtl(dx*dx+dy*dy);
            if(d==0) continue;
            e.push_back(norm(a));
            Real b=asinl(min(1.0L,2/d));
            e.push_back(norm(a+b)); e.push_back(norm(a-b));
        }
        sort(e.begin(),e.end());
        e.erase(unique(e.begin(),e.end()),e.end());
        lo=1e20L; hi=-1;
        for(size_t k=1;k<e.size();++k) {
            Real l=e[k-1],r=e[k],m=(l+r)/2;
            Real s=sinl(m),c=cosl(m);
            pair<Real,int> q[105];
            for(int i=0;i<n;++i) q[i]=make_pair(p[i].x*s+p[i].y*c,i);
            sort(q,q+n);
            int A=0,B=0,C=2;
            for(int i=1;i<n;++i) {
                if(q[i].first-q[i-1].first<2) {
                    A+=p[q[i].second].x-p[q[i-1].second].x;
                    B+=p[q[i].second].y-p[q[i-1].second].y;
                } else C+=2;
            }
            consider(l,A*sinl(l)+B*cosl(l)+C);
            consider(r,A*sinl(r)+B*cosl(r)+C);
            if(A || B) {
                Real a=norm(atan2l((Real)A,(Real)B));
                if(a>l && a<r) consider(a,A*sinl(a)+B*cosl(a)+C);
            }
        }
        printf("%.15f\n%.15f\n",(double)amin,(double)amax);
    }
    return 0;
}
