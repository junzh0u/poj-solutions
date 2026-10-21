// POJ 3808 - Malfatti Circles
// Model: gpt-6-astra
// A circle of radius r touches each adjacent side r*cot(angle/2)
// from its vertex. Two circles on a common side are tangent when the
// remaining gap between their contact points is 2*sqrt(r1*r2).
// Bisect the first radius; the other two follow from stable quadratic
// roots, and their mutual tangency gives a strictly decreasing residual.
// Output stays in input vertex order, including obtuse triangles.
#include <cstdio>
#include <cmath>

double other(double r, double t, double u, double side) {
    double gap = side - t*r;
    double q = gap / (sqrt(r + u*gap) + sqrt(r));
    return q*q;
}
int main() {
    double x[3], y[3];
    while (scanf("%lf%lf%lf%lf%lf%lf", x,y,x+1,y+1,x+2,y+2)==6) {
        if(x[0]==0 && y[0]==0 && x[1]==0 && y[1]==0 && x[2]==0 && y[2]==0) break;
        double a[3], t[3];
        for(int i=0;i<3;++i) {
            int j=(i+1)%3,k=(i+2)%3;
            double dx=x[j]-x[k],dy=y[j]-y[k];
            a[i]=sqrt(dx*dx+dy*dy);
        }
        double s=(a[0]+a[1]+a[2])/2;
        double area=fabs((x[1]-x[0])*(y[2]-y[0])-(y[1]-y[0])*(x[2]-x[0]))/2;
        double in=area/s;
        for(int i=0;i<3;++i) t[i]=(s-a[i])/in;
        double lo=0,hi=in;
        for(int it=0;it<100;++it) {
            double r=(lo+hi)/2;
            double b=other(r,t[0],t[1],a[2]);
            double c=other(r,t[0],t[2],a[1]);
            if(t[1]*b+t[2]*c+2*sqrt(b*c)>a[0]) lo=r;
            else hi=r;
        }
        double r=(lo+hi)/2;
        printf("%.9f %.9f %.9f\n",r,other(r,t[0],t[1],a[2]),other(r,t[0],t[2],a[1]));
    }
    return 0;
}
