// POJ 1871 - Bullet Hole
// Model: gpt-6.1-sol
// Intersect the bullet ray with each closed mini-cube, including tangencies.
// Holes form one monotone-height chain. A cell drains down to its lowest
// hole or the lowest exterior outlet, whichever is higher. A ray origin
// inside a cell is not itself a hole. All geometric comparisons use 1e-6.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const double EPS=1e-6;
double p[3],v[3];
bool clip(double x,double y,double z,double s,double &a,double &b) {
    double l[3]={x,y,z}; a=-1e30; b=1e30;
    for(int k=0;k<3;k++) {
        if(v[k]==0) { if(p[k]<l[k]-EPS || p[k]>l[k]+s+EPS) return false; }
        else {
            double u=(l[k]-p[k])/v[k], w=(l[k]+s-p[k])/v[k];
            if(u>w) swap(u,w);
            a=max(a,u); b=min(b,w);
        }
    }
    if(a<0 && b>=0) {
        for(int k=0;k<3;k++)
            if(v[k]==0 && (fabs(p[k]-l[k])<EPS || fabs(p[k]-l[k]-s)<EPS)) a=0;
    }
    return a<=b+1e-10 && b>=-1e-10;
}
int main() {
    int n,d,trial=0; double q[3];
    while(scanf("%d",&n)==1 && n) {
        scanf("%d%lf%lf%lf%lf%lf%lf",&d,&p[0],&p[1],&p[2],&q[0],&q[1],&q[2]);
        for(int k=0;k<3;k++) v[k]=q[k]-p[k];
        double a,b,ans=0,s=double(d)/n;
        if(clip(0,0,0,d,a,b)) {
            double outlet=p[2]+b*v[2];
            if(a>=-1e-10) outlet=min(outlet,p[2]+a*v[2]);
            for(int i=0;i<n;i++) for(int j=0;j<n;j++) for(int k=0;k<n;k++) {
                if(!clip(i*s,j*s,k*s,s,a,b)) continue;
                double hole=p[2]+b*v[2];
                if(a>=-1e-10) hole=min(hole,p[2]+a*v[2]);
                double level=max(outlet,hole);
                ans+=s*s*max(0.0,min(s,(k+1)*s-level));
            }
        }
        if(trial) puts("");
        printf("Trial %d, Volume = %.2f\n",++trial,ans+1e-9);
    }
    return 0;
}
