// POJ 2986 - A Triangle and a Circle
// Model: gpt-6-astra
// Sum signed circle-clipped edge contributions about the circle center.
// Split each edge at its circle intersections: inside pieces contribute
// triangles, outside pieces sectors. atan2 avoids inverse-cosine clamping.
// Input runs to EOF; either vertex order and degenerate edges are supported.
#include <cstdio>
#include <cmath>
#include <algorithm>
struct Point {
    double x, y;
    Point(double a=0, double b=0): x(a), y(b) {}
    Point operator+(Point b) const { return Point(x+b.x,y+b.y); }
    Point operator-(Point b) const { return Point(x-b.x,y-b.y); }
    Point operator*(double k) const { return Point(x*k,y*k); }
};
double dot(Point a, Point b) { return a.x*b.x+a.y*b.y; }
double cross(Point a, Point b) { return a.x*b.y-a.y*b.x; }
double edge(Point a, Point b, double r) {
    Point d=b-a;
    double len=dot(d,d);
    if (len==0) return 0;
    double t[4]={0,1,0,0};
    int n=2;
    double foot=-dot(a,d)/len;
    Point closest=a+d*foot;
    double h2=r*r-dot(closest,closest);
    if (h2>0) {
        double delta=std::sqrt(h2/len);
        double u=foot-delta, v=foot+delta;
        if (u>0 && u<1) t[n++]=u;
        if (v>0 && v<1) t[n++]=v;
    }
    std::sort(t,t+n);
    double ans=0;
    for (int i=1;i<n;++i) {
        Point p=a+d*t[i-1], q=a+d*t[i];
        Point m=a+d*((t[i-1]+t[i])*0.5);
        if (dot(m,m)<r*r) ans+=cross(p,q)*0.5;
        else ans+=r*r*std::atan2(cross(p,q),dot(p,q))*0.5;
    }
    return ans;
}
int main() {
    Point p[3],c; double r;
    while (std::scanf("%lf%lf%lf%lf%lf%lf%lf%lf%lf", &p[0].x,&p[0].y,
        &p[1].x,&p[1].y,&p[2].x,&p[2].y,&c.x,&c.y,&r)==9) {
        for (int i=0;i<3;++i) p[i]=p[i]-c;
        double area=0;
        for (int i=0;i<3;++i) area+=edge(p[i],p[(i+1)%3],r);
        std::printf("%.2f\n",std::fabs(area));
    }
    return 0;
}
