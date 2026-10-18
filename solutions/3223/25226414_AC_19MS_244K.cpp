// POJ 3223 - Football Game
// Model: gpt-6-astra
// Parameterize UV and evaluate both endpoints and every stationary angle.
// In front of the directed goal AB means its right half-plane. The angle
// is atan2(N,D), with N linear and D quadratic, so its derivative numerator
// is quadratic. Both goalposts lie strictly to the left of directed UV,
// hence the segment never passes through either post or through the goal.
#include <cstdio>
#include <cmath>
#include <algorithm>
struct Point { double x,y; };
Point sub(Point a,Point b) { Point c={a.x-b.x,a.y-b.y}; return c; }
double cross(Point a,Point b) { return a.x*b.y-a.y*b.x; }
double dot(Point a,Point b) { return a.x*b.x+a.y*b.y; }
Point A,B,U,V;
double answer;
void test(double t) {
    if(t<0 || t>1) return;
    Point p={U.x+(V.x-U.x)*t,U.y+(V.y-U.y)*t};
    Point a=sub(A,p),b=sub(B,p);
    double n=-cross(a,b);
    if(n<0) return;
    answer=std::max(answer,atan2(n,dot(a,b)));
}
int main() {
    while(scanf(" (%lf , %lf ) (%lf , %lf ) (%lf , %lf ) (%lf , %lf )",
        &A.x,&A.y,&B.x,&B.y,&U.x,&U.y,&V.x,&V.y)==8) {
        Point a=sub(A,U),b=sub(B,U),v=sub(V,U);
        double n0=-cross(a,b),n1=-cross(sub(B,A),v);
        double d0=dot(a,b),d1=-dot(v,a)-dot(v,b),d2=dot(v,v);
        double qa=-n1*d2,qb=-2*n0*d2,qc=n1*d0-n0*d1;
        answer=0;
        test(0); test(1);
        if(n1!=0) test(-n0/n1);
        if(qa==0) {
            if(qb!=0) test(-qc/qb);
        } else {
            double disc=qb*qb-4*qa*qc;
            if(disc>=0) {
                double s=sqrt(disc);
                double q=-0.5*(qb+(qb>=0?s:-s));
                if(q!=0) { test(q/qa); test(qc/q); }
                else test(-qb/(2*qa));
            }
        }
        printf("%.3f\n",answer*180/acos(-1.0));
    }
    return 0;
}
