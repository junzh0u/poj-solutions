// POJ 2382 - Radio Coverage
// Model: gpt-6-astra
// Each compatible relay contributes its disk area outside the base disk.
// Enumerate all subsets, rejecting every pair whose interiors intersect.
// Do not discard centers just outside the base: the discussion documents
// coordinate rounding. Handle containment and disjointness before acos.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double PI = acos(-1.0);
struct Circle { double x, y, r; };
double distance(const Circle& a, const Circle& b) {
    double x=a.x-b.x, y=a.y-b.y;
    return sqrt(x*x+y*y);
}
double clamp(double x) { return max(-1.0,min(1.0,x)); }
double overlap(double R, double r, double d) {
    if (d >= R+r) return 0;
    if (d <= fabs(R-r)) return PI*min(R,r)*min(R,r);
    double a=acos(clamp((d*d+R*R-r*r)/(2*d*R)));
    double b=acos(clamp((d*d+r*r-R*R)/(2*d*r)));
    double product=(-d+R+r)*(d+R-r)*(d-R+r)*(d+R+r);
    return R*R*a+r*r*b-0.5*sqrt(max(0.0,product));
}
int main() {
    int n;
    Circle base, c[10];
    if (scanf("%d%lf%lf%lf",&n,&base.x,&base.y,&base.r)!=4) return 0;
    double gain[10]; int conflict[10]={0};
    for (int i=0;i<n;++i) {
        scanf("%lf%lf%lf",&c[i].x,&c[i].y,&c[i].r);
        gain[i]=max(0.0,PI*c[i].r*c[i].r-overlap(base.r,c[i].r,distance(base,c[i])));
        for (int j=0;j<i;++j)
            if (distance(c[i],c[j])+1e-7<c[i].r+c[j].r) {
                conflict[i]|=1<<j; conflict[j]|=1<<i;
            }
    }
    double best=0;
    for (int mask=0;mask<(1<<n);++mask) {
        double sum=0; bool valid=true;
        for (int i=0;i<n;++i) if (mask&(1<<i)) {
            if (mask&conflict[i]) { valid=false; break; }
            sum+=gain[i];
        }
        if (valid) best=max(best,sum);
    }
    printf("%.4f\n",PI*base.r*base.r+best);
    return 0;
}
