// POJ 4042 - Regular Convex Polygon
// Model: gpt-6.1-sol
// An inscribed triangle angle is an integer multiple of pi/n. Try n in
// increasing order, minimizing the two rounding residuals. A tiny residual
// stops early; otherwise retain the closest fit to the rounded coordinates.
// POJ's statement is empty; the SWERC 2011 statement and jury material
// specify triples terminated by END and n <= 1000.
#include <cstdio>
#include <cmath>
struct Point { double x, y; };
double angle(Point a, Point b, Point c) {
    double ux=b.x-a.x, uy=b.y-a.y;
    double vx=c.x-a.x, vy=c.y-a.y;
    return atan2(fabs(ux*vy-uy*vx), ux*vx+uy*vy);
}
int main() {
    Point p[3];
    const double pi=acos(-1.0);
    while (scanf("%lf%lf", &p[0].x, &p[0].y)==2) {
        if (scanf("%lf%lf%lf%lf", &p[1].x, &p[1].y,
                  &p[2].x, &p[2].y)!=4) break;
        double a=angle(p[0],p[1],p[2])/pi;
        double b=angle(p[1],p[0],p[2])/pi;
        double bestError=1e100;
        int best=3;
        for (int n=3; n<=1000; ++n) {
            double x=a*n, y=b*n;
            double ex=fabs(x-floor(x+0.5));
            double ey=fabs(y-floor(y+0.5));
            double error=ex>ey?ex:ey;
            if (error<bestError) {
                bestError=error; best=n;
                if (error<1e-7) break;
            }
        }
        printf("%d\n",best);
    }
    return 0;
}
