// POJ 3127 - Lineland's Airport
// Model: gpt-6-astra
// Split runway positions at vertex crossings and changes of minimum height.
// On every resulting interval the excavation area is quadratic; check its
// endpoints and stationary point. Only removing rock means the runway height
// is the minimum terrain height anywhere along its entire length.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
int n;
double len, x[505], y[505], slope[505], pref[505];
int segment(double p) {
    int i=int(upper_bound(x,x+n,p)-x)-1;
    return max(0,min(n-2,i));
}
double height(double p) { int i=segment(p); return y[i]+slope[i]*(p-x[i]); }
double integral(double p) {
    int i=segment(p); double d=p-x[i];
    return pref[i]+y[i]*d+slope[i]*d*d/2;
}
double value(double p,double lowest) {
    double h=min(lowest,min(height(p),height(p+len)));
    return integral(p+len)-integral(p)-len*h;
}
int main() {
    int tc; scanf("%d",&tc);
    while(tc--) {
        scanf("%d%lf",&n,&len);
        for(int i=0;i<n;++i) scanf("%lf%lf",x+i,y+i);
        pref[0]=0;
        for(int i=0;i+1<n;++i) {
            slope[i]=(y[i+1]-y[i])/(x[i+1]-x[i]);
            pref[i+1]=pref[i]+(y[i]+y[i+1])*(x[i+1]-x[i])/2;
        }
        double end=x[n-1]-len;
        vector<double> breaks;
        breaks.push_back(x[0]); breaks.push_back(end);
        for(int i=0;i<n;++i) {
            if(x[i]>x[0] && x[i]<end) breaks.push_back(x[i]);
            if(x[i]-len>x[0] && x[i]-len<end) breaks.push_back(x[i]-len);
        }
        sort(breaks.begin(),breaks.end());
        breaks.erase(unique(breaks.begin(),breaks.end()),breaks.end());
        double low=1e30;
        for(int i=0;i<n;++i) if(x[i]>=x[0] && x[i]<=x[0]+len) low=min(low,y[i]);
        double ans=value(x[0],low);
        for(size_t k=1;k<breaks.size();++k) {
            double left=breaks[k-1],right=breaks[k],mid=(left+right)/2;
            low=1e30;
            for(int i=0;i<n;++i) if(x[i]>mid && x[i]<mid+len) low=min(low,y[i]);
            double a[3]={slope[segment(mid)],slope[segment(mid+len)],0};
            double b[3]={height(left),height(left+len),low};
            vector<double> cuts;
            cuts.push_back(0); cuts.push_back(right-left);
            for(int i=0;i<3;++i) for(int j=0;j<i;++j) if(a[i]!=a[j]) {
                double t=(b[j]-b[i])/(a[i]-a[j]);
                if(t>0 && t<right-left) cuts.push_back(t);
            }
            sort(cuts.begin(),cuts.end());
            for(size_t j=0;j<cuts.size();++j) ans=min(ans,value(left+cuts[j],low));
            for(size_t j=1;j<cuts.size();++j) {
                double t=(cuts[j-1]+cuts[j])/2; int m=0;
                for(int i=1;i<3;++i) if(a[i]*t+b[i]<a[m]*t+b[m]) m=i;
                double curvature=a[1]-a[0];
                if(curvature>0) {
                    double root=(len*a[m]-b[1]+b[0])/curvature;
                    if(root>cuts[j-1] && root<cuts[j]) ans=min(ans,value(left+root,low));
                }
            }
        }
        printf("%.4f\n",max(0.0,ans));
    }
    return 0;
}
