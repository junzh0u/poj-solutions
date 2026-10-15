// POJ 3289 - Moonshine
// Model: gpt-6.1-sol
// Preserve the upright volume, then bisect the horizontal liquid depth.
// Circular segments give the cylinders; adaptive Simpson integration gives
// the conic shoulder. All horizontal cross-sections share the same axis.
// Zero shoulder height and zero neck radius are handled without division.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0);
double y;
double area(double r) {
    if(r<=0 || y>=r) return 0;
    if(y<=-r) return PI*r*r;
    return r*r*acos(y/r)-y*sqrt(max(0.0,r*r-y*y));
}
double integrate(double a,double b,double fa,double fm,double fb,double old,double eps,int dep) {
    double m=(a+b)/2, l=(a+m)/2, r=(m+b)/2;
    double fl=area(l),fr=area(r);
    double left=(m-a)*(fa+4*fl+fm)/6;
    double right=(b-m)*(fm+4*fr+fb)/6;
    double delta=left+right-old;
    if(dep==0 || fabs(delta)<=15*eps) return left+right+delta/15;
    return integrate(a,m,fa,fl,fm,left,eps/2,dep-1)+integrate(m,b,fm,fr,fb,right,eps/2,dep-1);
}
double shoulder(double a,double b) {
    if(a==b) return area(a);
    double fa=area(a),fb=area(b),fm=area((a+b)/2);
    return integrate(a,b,fa,fm,fb,(b-a)*(fa+4*fm+fb)/6,1e-9,24)/(b-a);
}
int main() {
    double k,hb,db,hn,dn,h;
    while(scanf("%lf%lf%lf%lf%lf%lf",&k,&hb,&db,&hn,&dn,&h)==6) {
        if(k==0&&hb==0&&db==0&&hn==0&&dn==0&&h==0) break;
        double rb=db/2,rn=dn/2,hc=max(0.0,h-hb-hn);
        double v=PI*rb*rb*min(k,hb);
        double z=min(max(0.0,k-hb),hc);
        if(hc>0) {
            double rz=rb+(rn-rb)*z/hc;
            v+=PI*z*(rb*rb+rb*rz+rz*rz)/3;
        }
        v+=PI*rn*rn*max(0.0,k-hb-hc);
        double lo=0,hi=db;
        if(k<=0) hi=0;
        else if(k>=h) lo=db;
        else for(int it=0;it<45;++it) {
            double s=(lo+hi)/2; y=rb-s;
            double vol=hb*area(rb)+hn*area(rn);
            if(hc>0) vol+=hc*shoulder(rn,rb);
            if(vol<v) lo=s; else hi=s;
        }
        printf("%.2f\n",(lo+hi)/2);
    }
    return 0;
}
