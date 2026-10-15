// POJ 2235 - Triangularize the Convex Hull
// Model: gpt-6.1-sol
// Delaunay triangulations maximize the minimum angle. Enumerate all
// noncollinear triples whose open circumcircle contains no input point.
// Cocircular triples may all be included: Delaunay triangulations have
// the same minimum angle, including all choices inside cocircular faces.
// Integer-coordinate incircle determinants are evaluated exactly in double.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { double x,y; } p[105];
double cross(double ax,double ay,double bx,double by) { return ax*by-ay*bx; }
int main() {
    int n;
    const double pi=acos(-1.0);
    while(scanf("%d",&n)==1) {
        for(int i=0;i<n;++i) scanf("%lf%lf",&p[i].x,&p[i].y);
        double answer=pi;
        for(int i=0;i<n;++i) for(int j=i+1;j<n;++j) for(int k=j+1;k<n;++k) {
            double area=cross(p[j].x-p[i].x,p[j].y-p[i].y,p[k].x-p[i].x,p[k].y-p[i].y);
            if(area==0) continue;
            bool empty=true;
            for(int t=0;t<n;++t) if(t!=i && t!=j && t!=k) {
                double ax=p[i].x-p[t].x, ay=p[i].y-p[t].y;
                double bx=p[j].x-p[t].x, by=p[j].y-p[t].y;
                double cx=p[k].x-p[t].x, cy=p[k].y-p[t].y;
                double det=(ax*ax+ay*ay)*cross(bx,by,cx,cy)
                          -(bx*bx+by*by)*cross(ax,ay,cx,cy)
                          +(cx*cx+cy*cy)*cross(ax,ay,bx,by);
                if((area>0 && det>0) || (area<0 && det<0)) { empty=false; break; }
            }
            if(!empty) continue;
            int v[3]={i,j,k};
            for(int a=0;a<3;++a) {
                Point o=p[v[a]], u=p[v[(a+1)%3]], w=p[v[(a+2)%3]];
                double angle=atan2(fabs(area),(u.x-o.x)*(w.x-o.x)+(u.y-o.y)*(w.y-o.y));
                answer=min(answer,angle);
            }
        }
        printf("%.2f\n",answer*180/pi);
    }
    return 0;
}
