// POJ 3577 - Global Positioning System
// Model: gpt-6.1-sol
// Subtract squared range equations, then eliminate the unknown initial range.
// The resulting line intersects the unit circle of source directions.
// Recover the nonnegative range and verify each candidate against all sensors.
// Times are relative to the first arrival; velocity is converted to km/s.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;
typedef long double Real;
struct P { Real x,y; P(Real a=0,Real b=0):x(a),y(b){} };
Real dot(P a,P b){return a.x*b.x+a.y*b.y;}
Real norm(P a){return sqrtl(dot(a,a));}
int main(){
    Real c,x[3],y[3],t[3]; int cs=0;
    while(cin>>c && c!=0){
        for(int i=0;i<3;++i) cin>>x[i]>>y[i];
        for(int i=0;i<3;++i) cin>>t[i];
        int z=0; for(int i=1;i<3;++i) if(t[i]<t[z]) z=i;
        int a=(z+1)%3,b=(z+2)%3;
        P v[2]={P(x[a]-x[z],y[a]-y[z]),P(x[b]-x[z],y[b]-y[z])};
        Real d[2]={(t[a]-t[z])*c/1000,(t[b]-t[z])*c/1000};
        Real k[2]; for(int i=0;i<2;++i){Real l=norm(v[i]); k[i]=(l-d[i])*(l+d[i])/2;}
        P ans; Real best=1e100L;
        if(fabsl(k[0])+fabsl(k[1])>1e-24L){
            P n(k[1]*v[0].x-k[0]*v[1].x,k[1]*v[0].y-k[0]*v[1].y);
            Real h=k[0]*d[1]-k[1]*d[0], len=norm(n);
            Real co=h/len; co=max((Real)-1,min((Real)1,co));
            Real si=sqrtl(max((Real)0,(1-co)*(1+co)));
            n.x/=len; n.y/=len;
            for(int s=-1;s<=1;s+=2){
                P u(n.x*co-s*n.y*si,n.y*co+s*n.x*si);
                int j=fabsl(k[0])>fabsl(k[1])?0:1;
                Real den=dot(v[j],u)+d[j];
                if(den==0) continue;
                Real r=k[j]/den; if(r < -1e-8L) continue;
                r=max((Real)0,r); P p(r*u.x,r*u.y);
                Real err=0;
                for(int i=0;i<2;++i){P q(p.x-v[i].x,p.y-v[i].y);err+=fabsl(norm(q)-r-d[i]);}
                if(err<best){best=err;ans=p;}
            }
        }
        Real ax=ans.x+x[z],ay=ans.y+y[z];
        if(fabsl(ax)<0.0000005L) ax=0;
        if(fabsl(ay)<0.0000005L) ay=0;
        cout<<"Case "<<++cs<<":\n"<<fixed<<setprecision(6)<<ax<<' '<<ay<<'\n';
    }
}
