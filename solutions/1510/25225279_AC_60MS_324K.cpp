// POJ 1510 - Hares and Foxes
// Model: gpt-6.1-sol
// Each coordinate satisfies the matrix characteristic recurrence. Classify
// its two parity subsequences, including repeated roots and vanishing modes.
// A finite nonzero limit, an oscillation, or just one divergent species is chaos.
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;
typedef long double Real;
const Real EPS=1e-12L;
int signum(Real x) { return x>0 ? 1 : -1; }
bool zero(Real x, Real scale) { return fabsl(x)<=EPS*max((Real)1,scale); }
// 0 denotes zero limit; +/-1 infinity; 2 anything else.
int parity(Real u, Real v, Real p, Real q) {
    if (zero(p-q,max(p,q))) {
        u+=v; v=0;
    }
    if(p<q) { swap(p,q); swap(u,v); }
    Real coeff[2]={u,v}, base[2]={p,q};
    for(int i=0;i<2;++i) {
        if(zero(coeff[i],fabsl(u)+fabsl(v))) continue;
        if(base[i]>1+EPS) return signum(coeff[i]);
        if(base[i]>=1-EPS) return 2;
        return 0;
    }
    return 0;
}
int classify(Real x, Real y, Real t, Real det, Real disc) {
    if(zero(x,fabsl(x)) && zero(y,fabsl(y))) return 0;
    if(disc < -EPS*max((Real)1,t*t+fabsl(det)))
        return det<1-EPS ? 0 : 2;
    if(zero(disc,t*t+fabsl(det))) {
        Real r=t/2;
        if(fabsl(r)<1-EPS) return 0;
        Real b=y-r*x;
        if(r<0) return 2;
        if(!zero(b,fabsl(y)+fabsl(r*x))) return signum(b);
        if(r>1+EPS) return signum(x);
        return 2;
    }
    Real s=sqrtl(disc), r=(t+s)/2, z=(t-s)/2;
    // Avoid subtractive cancellation in the smaller root.
    if(fabsl(r)>fabsl(z) && r!=0) z=det/r;
    else if(z!=0) r=det/z;
    Real u=(y-z*x)/(r-z), v=(r*x-y)/(r-z);
    if(zero(y-z*x,fabsl(y)+fabsl(z*x))) u=0;
    if(zero(r*x-y,fabsl(r*x)+fabsl(y))) v=0;
    int even=parity(u,v,r*r,z*z);
    int odd=parity(u*r,v*z,r*r,z*z);
    return even==odd ? even : 2;
}
int main() {
    int n; cin>>n;
    while(n--) {
        Real a,b,c,d,h,f; cin>>a>>b>>c>>d>>h>>f;
        Real t=a+c, det=a*c+b*d, disc=(a-c)*(a-c)-4*b*d;
        int hs=classify(h,a*h-b*f,t,det,disc);
        int fs=classify(f,d*h+c*f,t,det,disc);
        if(hs==0 && fs==0) cout<<"Ecological balance will develop.\n";
        else if(hs==-1 && fs==1) cout<<"Hares will die out while foxes will overgrow.\n";
        else if(hs==1 && fs==-1) cout<<"Hares will overgrow while foxes will die out.\n";
        else if(hs==-1 && fs==-1) cout<<"Both hares and foxes will die out.\n";
        else if(hs==1 && fs==1) cout<<"Both hares and foxes will overgrow.\n";
        else cout<<"Chaos will develop.\n";
    }
}
