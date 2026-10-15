// POJ 3285 - Point of view in Flatland
// Model: gpt-6.1-sol
// Equal angular diameters mean equal distance/radius ratios. Subtracting
// the three squared-distance equations expresses the point linearly in
// its squared distance from the first center, leaving one quadratic.
// Choose the smaller valid distance, which gives the larger angular size.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
int main() {
    double x[3], y[3], r[3];
    while (scanf("%lf%lf%lf%lf%lf%lf%lf%lf%lf",x,y,r,x+1,y+1,r+1,x+2,y+2,r+2)==9) {
        if (r[0]==0 && r[1]==0 && r[2]==0) break;
        double ux=x[1]-x[0], uy=y[1]-y[0];
        double vx=x[2]-x[0], vy=y[2]-y[0];
        double scale=max(max(fabs(ux),fabs(uy)),max(fabs(vx),fabs(vy)));
        ux/=scale; uy/=scale; vx/=scale; vy/=scale;
        double det=2*(ux*vy-uy*vx);
        double c=ux*ux+uy*uy, d=vx*vx+vy*vy;
        double e=1-(r[1]/r[0])*(r[1]/r[0]);
        double f=1-(r[2]/r[0])*(r[2]/r[0]);
        double ax=(c*vy-d*uy)/det, ay=(ux*d-vx*c)/det;
        double bx=(e*vy-f*uy)/det, by=(ux*f-vx*e)/det;
        double A=bx*bx+by*by, B=2*(ax*bx+ay*by)-1, C=ax*ax+ay*ay;
        double best=-1;
        if (A==0) { if (B<0) best=-C/B; }
        else {
            double disc=B*B-4*A*C;
            if (disc>=0) {
                double q=-0.5*(B+(B>=0?sqrt(disc):-sqrt(disc)));
                double z[2]={q/A, q==0 ? -1 : C/q};
                for (int i=0;i<2;++i)
                    if(z[i]>0 && (best<0 || z[i]<best)) best=z[i];
            }
        }
        if (best<0) puts("No solution");
        else {
            double xx=x[0]+scale*(ax+bx*best), yy=y[0]+scale*(ay+by*best);
            if (fabs(xx)<0.0005) xx=0;
            if (fabs(yy)<0.0005) yy=0;
            printf("%.2f %.2f\n",xx,yy);
        }
    }
    return 0;
}
