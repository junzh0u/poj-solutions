// POJ 3862 - Asteroids
// Model: gpt-6-astra
// Build an incremental triangulated convex hull from a nondegenerate tetrahedron.
// Tetrahedra from an interior point give the volume-weighted mass center.
// The answer is the sum of the two minimum center-to-face distances:
// their centered inscribed balls cannot overlap, and closest faces can touch.
// Integer determinants are below 2^53, so double evaluates signs exactly;
// Strict visibility preserves the triangulation of coplanar polygonal faces.
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
using namespace std;
struct P {
    double x,y,z;
    P(double a=0,double b=0,double c=0):x(a),y(b),z(c){}
    P operator+(P b)const{return P(x+b.x,y+b.y,z+b.z);}
    P operator-(P b)const{return P(x-b.x,y-b.y,z-b.z);}
    P operator*(double t)const{return P(x*t,y*t,z*t);}
};
double dot(P a,P b){return a.x*b.x+a.y*b.y+a.z*b.z;}
P cross(P a,P b){return P(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
struct Face { int a,b,c; Face(int x,int y,int z):a(x),b(y),c(z){} };
double solve(){
    int n;cin>>n;vector<P> p(n);
    for(int i=0;i<n;++i)cin>>p[i].x>>p[i].y>>p[i].z;
    int k=2;
    while(dot(cross(p[1]-p[0],p[k]-p[0]),cross(p[1]-p[0],p[k]-p[0]))==0)++k;
    swap(p[2],p[k]);k=3;
    while(dot(cross(p[1]-p[0],p[2]-p[0]),p[k]-p[0])==0)++k;
    swap(p[3],p[k]);
    P inside=(p[0]+p[1]+p[2]+p[3])*0.25;
    vector<Face> f;
    for(int i=0;i<4;++i){
        int v[3],q=0;for(int j=0;j<4;++j)if(j!=i)v[q++]=j;
        if(dot(cross(p[v[1]]-p[v[0]],p[v[2]]-p[v[0]]),p[i]-p[v[0]])>0)swap(v[1],v[2]);
        f.push_back(Face(v[0],v[1],v[2]));
    }
    for(int i=4;i<n;++i){
        int edge[60][60]={{0}};vector<Face> next;
        for(size_t j=0;j<f.size();++j){
            Face t=f[j];
            if(dot(cross(p[t.b]-p[t.a],p[t.c]-p[t.a]),p[i]-p[t.a])>0){
                ++edge[t.a][t.b];++edge[t.b][t.c];++edge[t.c][t.a];
            }else next.push_back(t);
        }
        for(int a=0;a<i;++a)for(int b=0;b<i;++b)
            if(edge[a][b]&&!edge[b][a])next.push_back(Face(a,b,i));
        f.swap(next);
    }
    P moment;double volume=0;
    for(size_t j=0;j<f.size();++j){
        Face t=f[j];P a=p[t.a],b=p[t.b],c=p[t.c];
        double w=fabs(dot(a-inside,cross(b-inside,c-inside)));
        volume+=w;moment=moment+(inside+a+b+c)*(w/4);
    }
    P mass=moment*(1/volume);double best=1e100;
    for(size_t j=0;j<f.size();++j){
        Face t=f[j];P normal=cross(p[t.b]-p[t.a],p[t.c]-p[t.a]);
        best=min(best,fabs(dot(normal,mass-p[t.a]))/sqrt(dot(normal,normal)));
    }
    return best;
}
int main(){double a=solve(),b=solve();cout<<setprecision(14)<<(a+b)<<endl;}
