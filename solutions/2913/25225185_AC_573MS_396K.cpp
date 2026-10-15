// POJ 2913 - Fly Monkey
// Model: gpt-6.1-sol
// Clip the first wire to the distance reachable from Pipi, then minimize
// the squared distance over two segments. Check the four boundary minima
// and the interior stationary point. Direct residuals avoid subtracting
// large squared distances; cross products avoid a Gram determinant loss.
// Wires are finite segments, including when they intersect or overlap.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;
typedef long double Real;
struct V {
    Real x,y,z;
    V() {}
    V(Real a,Real b,Real c):x(a),y(b),z(c) {}
    V operator+(const V& b)const{return V(x+b.x,y+b.y,z+b.z);}
    V operator-(const V& b)const{return V(x-b.x,y-b.y,z-b.z);}
    V operator*(Real k)const{return V(x*k,y*k,z*k);}
};
Real dot(V a,V b){return a.x*b.x+a.y*b.y+a.z*b.z;}
V cross(V a,V b){return V(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
Real norm(V a){return sqrtl(dot(a,a));}
Real clamp(Real a,Real lo,Real hi){return max(lo,min(hi,a));}
int main(){
    V a,b,c,e,p; Real d;
    cout<<fixed<<setprecision(3);
    while(cin>>a.x>>a.y>>a.z>>b.x>>b.y>>b.z>>c.x>>c.y>>c.z>>e.x>>e.y>>e.z>>p.x>>p.y>>p.z>>d){
        V u=b-a,v=e-c; u=u*(1/norm(u));
        Real len=norm(v); v=v*(1/len);
        Real lo=-min(d,norm(p-a)),hi=min(d,norm(b-p));
        V w=p-c;
        Real ans=1e100L;
        for(int i=0;i<2;++i){
            Real s=i?hi:lo;
            Real t=clamp(dot(w+u*s,v),0,len);
            V r=w+u*s-v*t; ans=min(ans,dot(r,r));
            t=i?len:0;
            s=clamp(-dot(w-v*t,u),lo,hi);
            r=w+u*s-v*t; ans=min(ans,dot(r,r));
        }
        V n=cross(u,v); Real det=dot(n,n);
        if(det>0){
            Real s=-dot(cross(w,v),n)/det;
            Real t=dot(w+u*s,v);
            if(s>=lo && s<=hi && t>=0 && t<=len){
                V r=w+u*s-v*t; ans=min(ans,dot(r,r));
            }
        }
        cout<<sqrtl(max((Real)0,ans))<<'\n';
    }
}
