// POJ 3031 - Jezzball
// Model: gpt-6.1-sol
// Unfold reflections to enumerate line crossings. Each crossing forbids an
// open interval of start times; sort and merge these intervals in each axis.
// Endpoint contacts and contacts at completion are excluded. The promised
// nonzero feasible interval lets us merge numerically coincident boundaries.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
struct Atom { int p[2], v[2]; } a[10];
const int dim[2]={1024,768};
int origin[2],n;
double solve(int axis) {
    int other=1-axis;
    double duration=max(origin[other],dim[other]-origin[other])/200.0;
    double horizon=10000+duration;
    vector<pair<double,double> > bad;
    for(int i=0;i<n;i++) {
        double period=2.0*dim[axis]/abs(a[i].v[axis]);
        for(int side=0;side<2;side++) {
            double target=side?2*dim[axis]-origin[axis]:origin[axis];
            double first=fmod((target-a[i].p[axis])/a[i].v[axis],period);
            if(first<0) first+=period;
            for(int k=0;;k++) {
                double t=first+k*period;
                if(t>horizon) break;
                double pos=fmod(a[i].p[other]+a[i].v[other]*t,2.0*dim[other]);
                if(pos<0) pos+=2*dim[other];
                if(pos>dim[other]) pos=2*dim[other]-pos;
                double end=t-fabs(pos-origin[other])/200.0;
                double start=t-duration;
                if(end>0 && end>start+1e-10) bad.push_back(make_pair(start,end));
            }
        }
    }
    sort(bad.begin(),bad.end());
    double ans=0;
    for(size_t i=0;i<bad.size();i++) {
        if(bad[i].first>ans+1e-8) break;
        ans=max(ans,bad[i].second);
        if(ans>10000) break;
    }
    return ans;
}
int main() {
    while(scanf("%d",&n)==1 && n) {
        scanf("%d%d",&origin[0],&origin[1]);
        for(int i=0;i<n;i++) scanf("%d%d%d%d",&a[i].p[0],&a[i].p[1],&a[i].v[0],&a[i].v[1]);
        double ans=min(solve(0),solve(1));
        if(ans>10000+1e-8) puts("Never");
        else printf("%.5f\n",ans);
    }
    return 0;
}
