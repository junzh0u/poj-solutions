// POJ 3381 - Confectionery
// Model: gpt-6-astra
// Simulate constant-rate intervals, ending whenever a syrup disappears or
// an interface reaches a crack. At an interface a crack takes the upper
// syrup, since every interface moves downward. Equal-height cracks add.
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int n,m;
    double s,v[10],a[10],x[10],d[10],ans[10]={0},q;
    if(scanf("%d%d%lf",&n,&m,&s)!=3) return 0;
    for(int i=0;i<n;++i) scanf("%lf%lf",v+i,a+i);
    for(int j=0;j<m;++j) { scanf("%lf%lf",x+j,d+j); x[j]*=s; }
    scanf("%lf",&q);
    const double eps=1e-9;
    for(;;) {
        int bottom=0;
        while(bottom<n && v[bottom]<=eps) ++bottom;
        if(bottom==n) break;
        double rate[10]={0},h[10],sum=0;
        for(int i=0;i<n;++i) { sum+=v[i]; h[i]=sum; }
        rate[bottom]=a[bottom]*q;
        for(int j=0;j<m;++j) {
            int k=0;
            while(k<n && (v[k]<=eps || h[k]<=x[j]+eps)) ++k;
            if(k<n) rate[k]+=a[k]*d[j];
        }
        double dt=1e100,flow=0;
        for(int i=0;i<n;++i) {
            if(rate[i]>0) dt=min(dt,v[i]/rate[i]);
            flow+=rate[i];
            if(flow>0) for(int j=0;j<m;++j)
                if(h[i]>x[j]+eps) dt=min(dt,(h[i]-x[j])/flow);
        }
        ans[bottom]+=dt*a[bottom]*q;
        for(int i=0;i<n;++i) v[i]=max(0.0,v[i]-dt*rate[i]);
    }
    for(int i=0;i<n;++i) printf("%.10f\n",ans[i]);
    return 0;
}
