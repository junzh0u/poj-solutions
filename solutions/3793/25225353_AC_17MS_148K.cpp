// POJ 3793 - Joe's Triangular Gardens
// Model: gpt-6.1-sol
// An affine image of an equilateral triangle's incircle has shape matrix
// Q = sum((vertex-centroid)*(vertex-centroid)^T)/6. Its eigenvalues are
// the squared semiaxes. The major eigenvector gives the focal direction.
// Keep the actual input precision: the almost-equilateral sample is not
// a circle. Order the computed foci lexicographically before rounding.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
int main() {
    int n; if (scanf("%d", &n)!=1) return 0;
    for(int tc=1;tc<=n;++tc) {
        double x[3],y[3],cx=0,cy=0;
        for(int i=0;i<3;++i) { scanf("%lf%lf",x+i,y+i); cx+=x[i]/3; cy+=y[i]/3; }
        double a=0,b=0,d=0;
        for(int i=0;i<3;++i) {
            double u=x[i]-cx,v=y[i]-cy;
            a+=u*u/6; b+=u*v/6; d+=v*v/6;
        }
        double gap=sqrt((a-d)*(a-d)+4*b*b);
        double dx,dy;
        if(a>=d) {
            dx=sqrt((gap+a-d)/2);
            dy=dx==0?0:b/dx;
        } else {
            dy=sqrt((gap-a+d)/2);
            if(b<0) dy=-dy;
            dx=dy==0?0:b/dy;
        }
        if(dx<0 || (dx==0 && dy<0)) { dx=-dx; dy=-dy; }
        double f1x=cx-dx,f1y=cy-dy,f2x=cx+dx,f2y=cy+dy;
        // Suppress a printed negative zero without changing focal order.
        if(fabs(f1x)<0.0000001) f1x=0;
        if(fabs(f1y)<0.0000001) f1y=0;
        if(fabs(f2x)<0.0000001) f2x=0;
        if(fabs(f2y)<0.0000001) f2y=0;
        printf("%d %.2f %.2f %.2f %.2f %.2f\n",tc,f1x,f1y,f2x,f2y,2*sqrt((a+d+gap)/2));
    }
    return 0;
}
