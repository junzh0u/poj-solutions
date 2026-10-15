// POJ 2852 - Model Rocket Height
// Model: gpt-6.1-sol
// Represent each sight line by a point and a unit direction in 3D.
// The shortest connecting segment is perpendicular to both directions;
// solve its two normal equations and average the endpoint heights.
// Use the stated closest sight lines, rather than intersecting their
// horizontal projections; the lines need not intersect in space.
#include <cstdio>
#include <cmath>
struct Vec { double x,y,z; };
double dot(Vec a, Vec b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
Vec direction(double e,double a) {
    Vec v={cos(e)*cos(a),cos(e)*sin(a),sin(e)}; return v;
}
int main() {
    int n; double ha,hb;
    if(scanf("%d%lf%lf",&n,&ha,&hb)!=3) return 0;
    const double rad=acos(-1.0)/180.0;
    for(int i=1;i<=n;++i) {
        double a,b,g,d; scanf("%lf%lf%lf%lf",&a,&b,&g,&d);
        Vec u=direction(a*rad,g*rad),v=direction(b*rad,d*rad);
        Vec w={100.0,0.0,hb-ha};
        double c=dot(u,v),p=dot(w,u),q=dot(w,v);
        Vec cross={u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x};
        double den=dot(cross,cross);
        double t=(p-c*q)/den,s=(c*p-q)/den;
        double h=(ha+t*u.z+hb+s*v.z)*0.5;
        printf("%d: %.0f\n",i,h);
    }
    return 0;
}
