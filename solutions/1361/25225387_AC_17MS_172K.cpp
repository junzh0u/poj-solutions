// POJ 1361 - JaWs
// Model: gpt-6.1-sol
// A pair of teeth forbids an upright triangular region of translations.
// Follow the upper envelope downhill; all its corners have half-integer x.
// Heights are stored in units of sqrt(3)/2, so envelope comparisons are exact.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int t; scanf("%d", &t);
    while(t--) {
        int l,u,a[10],b[10],p[10],q[10],sl=0,su=0;
        scanf("%d",&l);
        for(int i=0;i<l;++i){scanf("%d",a+i);p[i]=sl;sl+=a[i];}
        double x,y; scanf("%lf%lf%d",&x,&y,&u);
        for(int j=0;j<u;++j){scanf("%d",b+j);q[j]=su;su+=b[j];}
        int lo=-2*su,hi=2*sl;
        if(2*x<=lo){puts("WM");continue;}
        if(2*x>=hi){puts("MW");continue;}
        vector<int> h(hi-lo+1,0);
        for(int k=lo;k<=hi;++k)
            for(int i=0;i<l;++i)for(int j=0;j<u;++j){
                int left=p[i]-q[j]-b[j],right=p[i]+a[i]-q[j];
                h[k-lo]=max(h[k-lo],min(k-2*left,2*right-k));
            }
        double z=2*x-lo;
        int k=(int)floor(z),dir;
        if(fabs(z-k)<1e-8){
            if(h[k-1]>=h[k] && h[k+1]>=h[k]){
                printf("%.3f %.3f\n",x,h[k]*sqrt(3.0)/2);continue;
            }
            dir=h[k+1]<h[k]?1:-1;
        }else dir=h[k+1]<h[k]?1:-1;
        if(dir==1)++k;
        while(k>0 && k<hi-lo && h[k+dir]<h[k])k+=dir;
        if(k==0)puts("WM");
        else if(k==hi-lo)puts("MW");
        else printf("%.3f %.3f\n",(k+lo)/2.0,h[k]*sqrt(3.0)/2);
    }
    return 0;
}
