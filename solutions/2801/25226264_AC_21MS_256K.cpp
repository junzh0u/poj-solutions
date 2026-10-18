// POJ 2801 - Knockdown
// Model: gpt-6-astra
// Maximize the distance to the nearest site on the unit sphere. A maximum
// is a site antipode, a stationary point on a two-site bisector, or a
// three-site bisector intersection. Evaluate both orientations; antipodal
// pairs additionally need an arbitrary perpendicular direction. If other
// sites cut that circle, its endpoints occur among the triple candidates.
// Distances are angular (great-circle), not straight-line chords.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct V {
    double x,y,z;
    V(double a=0,double b=0,double c=0):x(a),y(b),z(c){}
    V operator+(V b)const{return V(x+b.x,y+b.y,z+b.z);}
    V operator-(V b)const{return V(x-b.x,y-b.y,z-b.z);}
    V operator*(double t)const{return V(x*t,y*t,z*t);}
};
double dot(V a,V b){return a.x*b.x+a.y*b.y+a.z*b.z;}
V cross(V a,V b){return V(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
V p[20]; int n; double best=1;
void check(V v){
    double len=sqrt(dot(v,v));
    if(len<1e-12)return;
    v=v*(1/len);
    double nearest=-1;
    for(int i=0;i<n;++i)nearest=max(nearest,dot(v,p[i]));
    best=min(best,nearest);
}
void both(V v){check(v);check(v*(-1));}
int main(){
    if(scanf("%d",&n)!=1)return 0;
    const double pi=acos(-1.0);
    for(int i=0;i<n;++i){
        double a,b;scanf("%lf%lf",&a,&b);a*=pi/180;b*=pi/180;
        p[i]=V(cos(a)*cos(b),cos(a)*sin(b),sin(a));
    }
    for(int i=0;i<n;++i){
        both(p[i]);
        for(int j=0;j<i;++j){
            both(p[i]+p[j]);
            both(cross(p[i],V(1,0,0)));
            both(cross(p[i],V(0,1,0)));
            for(int k=0;k<j;++k)
                both(cross(p[i]-p[j],p[i]-p[k]));
        }
    }
    printf("%.10f\n",acos(max(-1.0,min(1.0,best))));
    return 0;
}
