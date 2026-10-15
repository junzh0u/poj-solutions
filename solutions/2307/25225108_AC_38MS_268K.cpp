// POJ 2307 - Treasure Map
// Model: gpt-6.1-sol
// Build the true-north walk, rotate its endpoint clockwise by the magnetic
// declination to locate the treasure, and minimize distance over every segment.
// Positive declination means magnetic north lies east of true north.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;
struct Point { double x,y; };
int main() {
    const char *names[] = {"N","NbE","NNE","NEbN","NE","NEbE","ENE","EbN",
        "E","EbS","ESE","SEbE","SE","SEbS","SSE","SbE",
        "S","SbW","SSW","SWbS","SW","SWbW","WSW","WbS",
        "W","WbN","WNW","NWbW","NW","NWbN","NNW","NbW"};
    const double pi=acos(-1.0);
    int n;
    while (scanf("%d",&n)==1 && n) {
        Point p[1001]; p[0].x=p[0].y=0;
        for (int i=1;i<=n;++i) {
            char name[16]; double length;
            scanf("%15s%lf",name,&length);
            int k=0; while(k<32 && strcmp(name,names[k])) ++k;
            double a=k*pi/16;
            p[i].x=p[i-1].x+length*sin(a);
            p[i].y=p[i-1].y+length*cos(a);
        }
        double d; scanf("%lf",&d); d*=pi/180;
        Point target;
        target.x=p[n].x*cos(d)+p[n].y*sin(d);
        target.y=p[n].y*cos(d)-p[n].x*sin(d);
        double best=1e300;
        for(int i=1;i<=n;++i) {
            double dx=p[i].x-p[i-1].x,dy=p[i].y-p[i-1].y;
            double vx=target.x-p[i-1].x,vy=target.y-p[i-1].y;
            double den=dx*dx+dy*dy;
            double t=den>0 ? (vx*dx+vy*dy)/den : 0;
            t=max(0.0,min(1.0,t));
            vx-=t*dx; vy-=t*dy;
            best=min(best,sqrt(vx*vx+vy*vy));
        }
        printf("%.2f\n",best);
    }
    return 0;
}
