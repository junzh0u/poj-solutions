// POJ 2592 - Standard Point System
// Model: gpt-6.1-sol
// Count strictly lower marks, then compare their fraction with normal-CDF
// thresholds at every integer standard score. Composite Simpson integration
// avoids repeated inverse searches. The rounding changes at 501, not 500.
#include <cstdio>
#include <cmath>
#include <cstring>
double threshold[901];
double density(double x) { return exp(-x*x/2.0)/sqrt(2.0*acos(-1.0)); }
int main() {
    threshold[500]=0.5;
    double integral=0;
    for(int k=1;k<=400;++k) {
        double a=(k-1)/100.0, h=0.0001;
        double sum=density(a)+density(k/100.0);
        for(int j=1;j<100;++j) sum+=(j%2?4:2)*density(a+j*h);
        integral+=sum*h/3;
        threshold[500+k]=0.5+integral;
        threshold[500-k]=0.5-integral;
    }
    int n, first=1;
    while(scanf("%d",&n)==1 && n) {
        int count[151], lower[151];
        memset(count,0,sizeof(count));
        for(int i=0,v;i<n;++i) { scanf("%d",&v); ++count[v]; }
        int total=0;
        for(int i=0;i<=150;++i) { lower[i]=total; total+=count[i]; }
        if(!first) puts("");
        first=0;
        int m; scanf("%d",&m);
        while(m--) {
            int v; scanf("%d",&v);
            double p=double(lower[v])/n;
            int answer;
            if(p<=threshold[501]) {
                int l=100,r=501;
                while(l<r) { int mid=(l+r+1)/2; if(threshold[mid]<=p) l=mid; else r=mid-1; }
                answer=l;
            } else {
                int l=502,r=900;
                while(l<r) { int mid=(l+r)/2; if(threshold[mid]>=p) r=mid; else l=mid+1; }
                answer=l;
            }
            printf("%d\n",answer);
        }
    }
    return 0;
}
