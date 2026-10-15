// POJ 3924 - Tornado
// Model: gpt-6.1-sol
// Each direction gives a segment of relative positions translated once per
// tornado period. Its squared distance is convex in the cycle number.
// Check integer neighbors of the continuous minimum, found on the strip's
// boundaries or at its intersection with the origin. A zero-length tornado
// path means a stationary point, as noted on the discussion board.
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
typedef long double Real;
struct P {
    Real x,y;
    P(Real a=0,Real b=0):x(a),y(b){}
    P operator+(P b)const{return P(x+b.x,y+b.y);}
    P operator-(P b)const{return P(x-b.x,y-b.y);}
    P operator*(Real a)const{return P(x*a,y*a);}
};
Real dot(P a,P b){return a.x*b.x+a.y*b.y;}
Real cross(P a,P b){return a.x*b.y-a.y*b.x;}
Real norm(P a){return sqrtl(dot(a,a));}
Real segment(P a,P e){
    Real t=dot(e,e)==0?0:-dot(a,e)/dot(e,e);
    t=max((Real)0,min((Real)1,t));
    return norm(a+e*t);
}
Real strip(P a,P e,P d){
    Real ans=segment(a,e);
    Real candidates[3];
    candidates[0]=dot(a,d)/dot(d,d);
    candidates[1]=dot(a+e,d)/dot(d,d);
    Real det=cross(d,e);
    candidates[2]=det==0?0:cross(a,e)/det;
    for(int i=0;i<3;++i){
        Real k=floorl(candidates[i]);
        for(int j=-2;j<=2;++j) if(k+j>=0)
            ans=min(ans,segment(a-d*(k+j),e));
    }
    return ans;
}
int main(){
    P w,w2,a,b; Real vw,vt,lo,hi;
    while(cin>>w.x>>w.y>>w2.x>>w2.y>>vw
             >>a.x>>a.y>>b.x>>b.y>>vt>>lo>>hi){
        P u=w2-w; Real wl=norm(u); if(wl!=0)u=u*(vw/wl);
        P delta=b-a; Real len=norm(delta),answer;
        if(wl==0) answer=segment(a-w,delta);
        else if(len==0){
            P r=a-w; Real t=max((Real)0,dot(r,u)/dot(u,u));
            answer=norm(r-u*t);
        }else{
            Real h=len/vt; P d=u*(2*h);
            answer=min(strip(a-w,delta-u*h,d),
                       strip(b-w-u*h,delta*((Real)-1)-u*h,d));
        }
        cout<<(answer<lo?"Dangerous":answer>hi?"Miss":"Perfect")<<'\n';
    }
}
