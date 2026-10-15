// POJ 2701 - Lapux the Floating Island
// Model: gpt-6.1-sol
// Minimize the distance from each flight segment to every island edge.
// Disjoint segments attain their minimum at an endpoint projection.
// R1 is the maximum radius, despite the typo in the input description.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Point { double x,y; };
double distance(Point p, Point a, Point b) {
    double x=b.x-a.x, y=b.y-a.y;
    double den=x*x+y*y;
    double t=den ? ((p.x-a.x)*x+(p.y-a.y)*y)/den : 0;
    t=max(0.0,min(1.0,t));
    x=p.x-a.x-t*x; y=p.y-a.y-t*y;
    return sqrt(x*x+y*y);
}
int main() {
    double r0,r1; int n;
    while(scanf("%lf%lf%d",&r0,&r1,&n)==3 && n) {
        Point p[20][20], path[20]; int count[20];
        for(int i=0;i<n;++i) {
            scanf("%d",&count[i]);
            for(int j=0;j<count[i];++j) scanf("%lf%lf",&p[i][j].x,&p[i][j].y);
        }
        int m; scanf("%d",&m);
        for(int i=0;i<m;++i) scanf("%lf%lf",&path[i].x,&path[i].y);
        for(int k=1;k<m;++k) {
            double d=r1; Point a=path[k-1],b=path[k];
            for(int i=0;i<n;++i) for(int j=0;j<count[i];++j) {
                Point c=p[i][j],e=p[i][(j+1)%count[i]];
                d=min(d,distance(a,c,e)); d=min(d,distance(b,c,e));
                d=min(d,distance(c,a,b)); d=min(d,distance(e,a,b));
            }
            if(k>1) printf(" ");
            printf("%d",d+1e-8<r0 ? 0 : (int)floor(d+0.5+1e-8));
        }
        printf("\n");
    }
    return 0;
}
