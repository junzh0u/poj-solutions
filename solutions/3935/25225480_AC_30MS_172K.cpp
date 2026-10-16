// POJ 3935 - Striking Ball
// Model: gpt-6.1-sol
// Unfold the side walls into a periodic line. For each remaining brick,
// find the first intersection with either of its two mirrored intervals
// during the time the ball is at the brick's height. Remove the earliest
// hit, fold its position back, and reflect the appropriate velocity.
// The angle is measured counterclockwise from the positive x-axis.
// The guaranteed corner clearance makes the struck face unambiguous.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct Brick { double x,y; int p; bool alive; } b[505];
const double EPS=1e-9, INF=1e100;
double first(double x,double dx,double lo,double hi,double a,double c,double period) {
    double q=x+dx*lo;
    if (fabs(dx)<1e-14) {
        double z=fmod(q-a,period); if(z<0) z+=period;
        return z<=c-a+EPS ? lo:INF;
    }
    double target;
    if(dx>0) {
        double k=ceil((q-c)/period);
        target=max(q,a+k*period);
    } else {
        double k=floor((q-a)/period);
        target=min(q,c+k*period);
    }
    double t=lo+(target-q)/dx;
    return t<=hi+EPS ? t:INF;
}
int main() {
    double W,H,w,h;
    int n;
    while(scanf("%lf%lf%lf%lf%d",&W,&H,&w,&h,&n)==5 && W>0) {
        for(int i=0;i<n;++i) { scanf("%lf%lf%d",&b[i].x,&b[i].y,&b[i].p); b[i].alive=true; }
        double x,y=0,angle;
        scanf("%lf%lf",&x,&angle);
        double dx=cos(angle*acos(-1.0)/180),dy=sin(angle*acos(-1.0)/180);
        int score=0,left=n;
        while(left) {
            double bound=dy>0?(H-y)/dy:-y/dy;
            double best=bound; int hit=-1; bool vertical=false;
            for(int i=0;i<n;++i) if(b[i].alive) {
                double t1=(b[i].y-y)/dy,t2=(b[i].y+h-y)/dy;
                if(t1>t2) swap(t1,t2);
                double lo=max(0.0,t1),hi=min(bound,t2);
                if(lo>hi+EPS) continue;
                double t=min(first(x,dx,lo,hi,b[i].x,b[i].x+w,2*W),
                             first(x,dx,lo,hi,-b[i].x-w,-b[i].x,2*W));
                if(t<best-EPS) {
                    best=t; hit=i;
                    // At the height-entry time, contact is horizontal;
                    // otherwise the moving x coordinate entered a side.
                    vertical=t>lo+EPS;
                }
            }
            double z=fmod(x+dx*best,2*W); if(z<0) z+=2*W;
            if(z>W) { x=2*W-z; dx=-dx; } else x=z;
            y+=dy*best;
            if(hit<0) {
                if(dy<0) break;
                y=H; dy=-dy;
            } else {
                score+=b[hit].p; b[hit].alive=false; --left;
                if(vertical) dx=-dx; else dy=-dy;
            }
        }
        printf("%d\n",score);
    }
    return 0;
}
