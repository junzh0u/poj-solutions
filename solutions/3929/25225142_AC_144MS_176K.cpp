// POJ 3929 - Timer
// Model: gpt-6.1-sol
// Integrate circular caps along the horizontal cone axis, then binary-search
// the water level. V is the empty upper volume, not the remaining water.
// A squared integration coordinate and a small-angle series avoid cancellation
// near a full container; all geometry is normalized by the base radius.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
double level;
double f(double u) {
    if (u == 0) return 0;
    double t=level+(1-level)*u*u;
    double theta=acos(min(1.0,level/t));
    double area;
    if(theta<0.1) {
        double z=theta*theta;
        area=theta*z*(2.0/3+z*(-2.0/15+z*(4.0/315+z*(-2.0/2835+z*4.0/155925))));
    } else area=theta-sin(theta)*cos(theta);
    return t*t*area*2*(1-level)*u;
}
double simpson(double l,double r,double fl,double fm,double fr,double whole,double eps,int dep) {
    double m=(l+r)/2, a=f((l+m)/2), b=f((m+r)/2);
    double left=(m-l)*(fl+4*a+fm)/6, right=(r-m)*(fm+4*b+fr)/6;
    double delta=left+right-whole;
    if(dep==0 || fabs(delta)<15*eps) return left+right+delta/15;
    return simpson(l,m,fl,a,fm,left,eps/2,dep-1)+simpson(m,r,fm,b,fr,right,eps/2,dep-1);
}
double volume(double a) {
    level=a;
    double x=f(0),y=f(0.5),z=f(1);
    return simpson(0,1,x,y,z,(x+4*y+z)/6,1e-12*pow(1-a,2.5),20);
}
int main() {
    int cases; scanf("%d",&cases);
    while(cases--) {
        double h,d,v; scanf("%lf%lf%lf",&h,&d,&v);
        double r=d/2, target=v/(h*r*r),lo=0,hi=1;
        if(v==0) { printf("%.5f\n",d); continue; }
        for(int i=0;i<55;i++) {
            double m=(lo+hi)/2;
            if(volume(m)>target) lo=m; else hi=m;
        }
        printf("%.5f\n",r*(1+(lo+hi)/2));
    }
    return 0;
}
