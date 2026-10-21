// POJ 3962 - Dome of Circus
// Model: gpt-6-astra
// At height h, the least radius is max_i rho_i*h/(h-z_i).
// Ignoring pi/3, volume is max_i rho_i^2*h^3/(h-z_i)^2.
// Each term is convex for h>z_i and its derivative has sign h-3*z_i.
// Bisect using a maximizing point's derivative, a valid subgradient.
// The optimum lies between max(z_i) and 3*max(z_i). Axis points only
// constrain height; the lower endpoint may itself be the optimum.
// Process input instances through EOF; the statement has no case count.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const int N=10005;
double rho[N],z[N];
int main() {
    int n;
    while(scanf("%d",&n)==1 && n>0) {
    double top=0;
    for(int i=0;i<n;++i) {
        double x,y;
        scanf("%lf%lf%lf",&x,&y,&z[i]);
        rho[i]=sqrt(x*x+y*y);
        top=max(top,z[i]);
    }
    double lo=top,hi=3*top;
    for(int it=0;it<120;++it) {
        double h=(lo+hi)/2, r=-1;
        int active=-1;
        for(int i=0;i<n;++i) if(rho[i]>0) {
            double cur=rho[i]*h/(h-z[i]);
            if(cur>r) { r=cur; active=i; }
        }
        if(h<3*z[active]) lo=h; else hi=h;
    }
    double h=hi,r=0;
    for(int i=0;i<n;++i) if(rho[i]>0)
        r=max(r,rho[i]*h/(h-z[i]));
    printf("%.3f %.3f\n",h,r);
    }
    return 0;
}
