// POJ 3052 - Geodes
// Model: gpt-6-astra
// Intersect the two extruded convex projections. Its horizontal section
// is a rectangle whose sides are the two polygon widths. Every vertex
// height is integral, so both widths are linear on each unit-height slab.
// Simpson's rule integrates their quadratic product exactly on that slab.
// Shared minimum/maximum heights ensure the intersection has both full
// projections. Horizontal edges and collinear vertices are allowed.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Point { int x,z; };
void readWidths(double w[1001]) {
    int n; scanf("%d", &n);
    vector<Point> p(n);
    double lo[1001],hi[1001];
    for(int z=0;z<=1000;++z) { lo[z]=1e100; hi[z]=-1e100; }
    for(int i=0;i<n;++i) { scanf("%d%d", &p[i].x,&p[i].z); p[i].z+=500; }
    for(int i=0;i<n;++i) {
        Point a=p[i],b=p[(i+1)%n];
        if(a.z>b.z) swap(a,b);
        if(a.z==b.z) {
            lo[a.z]=min(lo[a.z],double(min(a.x,b.x)));
            hi[a.z]=max(hi[a.z],double(max(a.x,b.x)));
        } else {
            for(int z=a.z;z<=b.z;++z) {
                double x=a.x+double(b.x-a.x)*(z-a.z)/(b.z-a.z);
                lo[z]=min(lo[z],x); hi[z]=max(hi[z],x);
            }
        }
    }
    for(int z=0;z<=1000;++z) w[z]=hi[z]>=lo[z]?hi[z]-lo[z]:-1;
}
int main() {
    int t; scanf("%d",&t);
    while(t--) {
        double a[1001],b[1001],ans=0;
        readWidths(a); readWidths(b);
        for(int z=0;z<1000;++z) {
            if(a[z]<0 || a[z+1]<0 || b[z]<0 || b[z+1]<0) continue;
            ans+=(a[z]*b[z]+(a[z]+a[z+1])*(b[z]+b[z+1])+a[z+1]*b[z+1])/6;
        }
        printf("%.2f\n",ans);
    }
    return 0;
}
