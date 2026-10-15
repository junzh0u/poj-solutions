// POJ 2107 - Gear Profiles
// Model: gpt-6.1-sol
// Evaluate the pictured tangent formulas on each 3-by-3 window, then
// normalize their cross product. Input and output both sweep columns first.
// Use double precision; the board records that the old float data was fixed.
#include <cstdio>
#include <cmath>
struct Vec {
    double x,y,z;
    Vec(double a=0,double b=0,double c=0):x(a),y(b),z(c) {}
    Vec operator-(const Vec& b) const { return Vec(x-b.x,y-b.y,z-b.z); }
    Vec operator*(double k) const { return Vec(x*k,y*k,z*k); }
};
double dot(const Vec& a,const Vec& b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
Vec cross(const Vec& a,const Vec& b) {
    return Vec(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}
Vec p[50][50];
int main() {
    int X,Y;
    if(scanf("%d%d",&X,&Y)!=2) return 0;
    for(int c=0;c<Y;++c) for(int r=0;r<X;++r)
        scanf("%lf%lf%lf",&p[c][r].x,&p[c][r].y,&p[c][r].z);
    printf("%d\n",X*Y);
    for(int c=0;c+2<Y;++c) for(int r=0;r+2<X;++r) {
        Vec D=p[c][r+1], E=p[c+1][r+1], F=p[c+2][r+1];
        Vec B=p[c+1][r], H=p[c+1][r+2];
        double hu=dot(E-D,D-F), vu=dot(E-B,B-H);
        Vec hf=cross(D-F,E-D), vf=cross(B-H,E-B);
        Vec ht=cross(F-E,hf)-(F-E)*hu;
        Vec vt=cross(H-E,vf)-(H-E)*vu;
        Vec n=cross(ht,vt);
        double factor=1.0/sqrt(dot(n,n));
        printf("%.4f %.4f %.4f\n",n.x*factor,n.y*factor,n.z*factor);
    }
    return 0;
}
