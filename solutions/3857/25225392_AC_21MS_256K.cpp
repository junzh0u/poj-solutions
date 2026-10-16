// POJ 3857 - Solar Eclipse
// Model: gpt-6.1-sol
// Inflate each landed craft to radius 2R. The nearest feasible point is
// the origin, a radial minimum on an exposed circle arc, or an arc endpoint
// (a pairwise circle intersection). Check every candidate against all disks.
// A circle centered at the origin has constant boundary distance; test one
// arbitrary boundary point as well as all intersection endpoints.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { double x,y; Point(double a=0,double b=0):x(a),y(b){} };
Point p[105]; int n; double r,ans;
void test(Point q) {
    double v=sqrt(q.x*q.x+q.y*q.y);
    if(v>=ans) return;
    for(int i=0;i<n;++i) {
        double dx=q.x-p[i].x,dy=q.y-p[i].y;
        if(sqrt(dx*dx+dy*dy)<r-1e-8) return;
    }
    ans=v;
}
int main(){
    double R;
    while(scanf("%d%lf",&n,&R)==2 && (n || R!=0)) {
        r=2*R; ans=1e100;
        for(int i=0;i<n;++i) scanf("%lf%lf",&p[i].x,&p[i].y);
        test(Point());
        for(int i=0;i<n;++i){
            double d=sqrt(p[i].x*p[i].x+p[i].y*p[i].y);
            if(d==0) test(Point(r,0));
            else test(Point(p[i].x*(1-r/d),p[i].y*(1-r/d)));
            for(int j=0;j<i;++j){
                double dx=p[j].x-p[i].x,dy=p[j].y-p[i].y;
                double dd=dx*dx+dy*dy;
                if(dd==0 || dd>4*r*r) continue;
                double h=sqrt(max(0.0,r*r-dd/4)),len=sqrt(dd);
                double x=(p[i].x+p[j].x)/2,y=(p[i].y+p[j].y)/2;
                test(Point(x-dy/len*h,y+dx/len*h));
                test(Point(x+dy/len*h,y-dx/len*h));
            }
        }
        printf("%.6f\n",ans);
    }
    return 0;
}
