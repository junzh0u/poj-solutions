// POJ 3596 - Illuminated Planet
// Model: gpt-6.1-sol
// A surface normal n is visible when n.P > r and illuminated when
// n.S > r-R, because some positive part of the star is above its horizon.
// These are spherical caps; positive area overlaps iff their angular
// radii sum exceeds the angle between their axes. Tangency alone is No.
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
struct V { double x,y,z; };
V sub(V a,V b) { V c={a.x-b.x,a.y-b.y,a.z-b.z}; return c; }
double dot(V a,V b) { return a.x*b.x+a.y*b.y+a.z*b.z; }
int main() {
    V c,s,p; double r,R;
    if (!(cin>>c.x>>c.y>>c.z>>r)) return 0;
    cin>>s.x>>s.y>>s.z>>R>>p.x>>p.y>>p.z;
    s=sub(s,c); p=sub(p,c);
    double ds=sqrt(dot(s,s)), dp=sqrt(dot(p,p));
    V cross={s.y*p.z-s.z*p.y,s.z*p.x-s.x*p.z,s.x*p.y-s.y*p.x};
    double theta=atan2(sqrt(dot(cross,cross)),dot(s,p));
    double a=acos(max(-1.0,min(1.0,r/dp)));
    double b=acos(max(-1.0,min(1.0,(r-R)/ds)));
    cout<<(a>0 && a+b-theta>1e-12 ? "Yes" : "No")<<endl;
    return 0;
}
