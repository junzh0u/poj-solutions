// POJ 2177 - Ghost Busters
// Model: gpt-6-astra
// Each sphere defines a closed cap of firing directions on the unit sphere.
// A maximum overlap contains a cap center or an intersection of two boundaries.
// Test both boundary intersections for each pair, then retain all hit spheres.
// The radius bound puts every cap in the permitted octant; tangent hits count.
// O(N^3) time and O(N) space, including coincident and nested caps.
#include <cmath>
#include <cstdio>
#include <vector>
using namespace std;
typedef long double Real;
struct V {
    Real x,y,z;
    V(Real a=0,Real b=0,Real c=0):x(a),y(b),z(c){}
    V operator+(V b)const{return V(x+b.x,y+b.y,z+b.z);}
    V operator-(V b)const{return V(x-b.x,y-b.y,z-b.z);}
    V operator*(Real k)const{return V(x*k,y*k,z*k);}
};
Real dot(V a,V b){return a.x*b.x+a.y*b.y+a.z*b.z;}
V cross(V a,V b){return V(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
Real norm(V a){return sqrt(dot(a,a));}
V center[100],axis[100];
Real radius[100],cs[100],sn[100];
int n;
vector<int> best;
V bestRay;
void fire(V d){
    if(d.x < -1e-12L || d.y < -1e-12L || d.z < -1e-12L)return;
    Real dd=dot(d,d);
    vector<int> hit;
    for(int i=0;i<n;++i){
        V c=cross(center[i],d);
        if(dot(c,c) <= radius[i]*radius[i]*dd*(1+1e-9L))hit.push_back(i+1);
    }
    if(hit.size()>best.size()){best=hit;bestRay=d;}
}
int main(){
    if(scanf("%d",&n)!=1)return 0;
    for(int i=0;i<n;++i){
        int x,y,z,r;
        scanf("%d%d%d%d",&x,&y,&z,&r);
        center[i]=V(x,y,z);radius[i]=r;
        Real len=norm(center[i]);
        axis[i]=center[i]*(1/len);
        sn[i]=r/len;cs[i]=sqrt(1-sn[i]*sn[i]);
    }
    for(int i=0;i<n;++i)fire(axis[i]);
    for(int i=0;i<n;++i)for(int j=i+1;j<n;++j){
        V w=cross(axis[i],axis[j]);
        Real s=norm(w);
        if(s<1e-18L)continue;
        w=w*(1/s);
        V v=cross(w,axis[i]);
        Real t=(cs[j]-cs[i]*dot(axis[i],axis[j]))/s;
        Real h2=sn[i]*sn[i]-t*t;
        if(h2 < -1e-16L)continue;
        Real h=sqrt(h2>0?h2:0);
        V base=axis[i]*cs[i]+v*t;
        fire(base+w*h);fire(base-w*h);
    }
    printf("%d\n",int(best.size()));
    for(unsigned i=0;i<best.size();++i)printf("%s%d",i?" ":"",best[i]);
    puts("");
#ifdef VERIFY
    fprintf(stderr,"%.18Lg %.18Lg %.18Lg\n",bestRay.x,bestRay.y,bestRay.z);
#endif
    return 0;
}
