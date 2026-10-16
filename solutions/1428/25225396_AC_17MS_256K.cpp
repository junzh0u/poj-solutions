// POJ 1428 - Hermes' Colony
// Model: gpt-6.1-sol
// Enumerate the three full Steiner topologies using Melzak's equilateral
// construction, plus all trees with fewer junctions. Fermat points handle
// angles of at least 120 degrees by collapsing to the corresponding city.
// Input order need not be cyclic; all pairings and both orientations are used.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
struct P {
    double x,y,z;
    P(double a=0,double b=0,double c=0):x(a),y(b),z(c){}
    P operator+(P b)const{return P(x+b.x,y+b.y,z+b.z);}
    P operator-(P b)const{return P(x-b.x,y-b.y,z-b.z);}
    P operator*(double k)const{return P(x*k,y*k,z*k);}
};
double dot(P a,P b){return a.x*b.x+a.y*b.y+a.z*b.z;}
P cross(P a,P b){return P(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
double norm(P a){return sqrt(dot(a,a));}
double dist(P a,P b){return norm(a-b);}
P fermat(P a,P b,P c){
    P p[3]={a,b,c}; double w[3],sum=0;
    for(int i=0;i<3;i++){
        P u=p[(i+1)%3]-p[i],v=p[(i+2)%3]-p[i];
        double lu=norm(u),lv=norm(v);
        if(lu<1e-12||lv<1e-12)return p[i];
        double co=dot(u,v)/(lu*lv);
        if(co<=-0.5)return p[i];
        double si=norm(cross(u,v))/(lu*lv);
        w[i]=dist(p[(i+1)%3],p[(i+2)%3])/(si+sqrt(3.0)*co);
        sum+=w[i];
    }
    return (a*w[0]+b*w[1]+c*w[2])*(1/sum);
}
double triangle(P a,P b,P c){P s=fermat(a,b,c);return dist(a,s)+dist(b,s)+dist(c,s);}
double solve(P *p,int n,P normal){
    if(n==3)return triangle(p[0],p[1],p[2]);
    double ans=1e100;
    // All ordinary spanning trees (16 labelled trees, selected from 6 edges).
    int u[6]={0,0,0,1,1,2},v[6]={1,2,3,2,3,3};
    for(int mask=0;mask<64;mask++){
        int par[4]={0,1,2,3},cnt=0;double len=0;
        for(int k=0;k<6;k++)if(mask&(1<<k)){
            int a=u[k],b=v[k];while(par[a]!=a)a=par[a];while(par[b]!=b)b=par[b];
            if(a!=b){par[a]=b;cnt++;len+=dist(p[u[k]],p[v[k]]);}else {cnt=-10;break;}
        }
        if(cnt==3)ans=min(ans,len);
    }
    for(int i=0;i<4;i++){
        P q[3];int k=0;for(int j=0;j<4;j++)if(j!=i)q[k++]=p[j];
        double f=triangle(q[0],q[1],q[2]);
        for(int j=0;j<3;j++)ans=min(ans,f+dist(p[i],q[j]));
    }
    for(int partner=1;partner<4;partner++){
        P a=p[0],b=p[partner],q[2];int k=0;
        for(int j=1;j<4;j++)if(j!=partner)q[k++]=p[j];
        P c=q[0],d=q[1],mid=(c+d)*0.5;
        P off=cross(normal,d-c)*(sqrt(3.0)*0.5);
        for(int sign=-1;sign<=1;sign+=2){
            P e=mid+off*sign;
            P s=fermat(a,b,e),t=fermat(c,d,s);
            double len=dist(a,s)+dist(b,s)+dist(s,t)+dist(c,t)+dist(d,t);
            ans=min(ans,len);
        }
    }
    return ans;
}
int main(){
    double a,b,c,d;if(scanf("%lf%lf%lf%lf",&a,&b,&c,&d)!=4)return 0;
    P normal(a,b,c);normal=normal*(1/norm(normal));
    int tests;scanf("%d",&tests);
    for(int tc=1;tc<=tests;tc++){
        int n;scanf("%d",&n);P p[4];
        for(int i=0;i<n;i++)scanf("%lf%lf%lf",&p[i].x,&p[i].y,&p[i].z);
        printf("Province # %d : %.2f\n",tc,solve(p,n,normal));
    }
    return 0;
}
