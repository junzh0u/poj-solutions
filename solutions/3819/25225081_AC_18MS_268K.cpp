// POJ 3819 - Coverage
// Model: gpt-6.1-sol
// Project each tower onto the path, clip its chord to the segment,
// then merge the covered intervals. Tangency contributes zero length.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;
int main() {
    int n;
    while (scanf("%d", &n)==1 && n) {
        double x0,y0,x1,y1;
        scanf("%lf%lf%lf%lf", &x0,&y0,&x1,&y1);
        double dx=x1-x0, dy=y1-y0, len=sqrt(dx*dx+dy*dy);
        vector<pair<double,double> > a;
        for(int i=0;i<n;++i) {
            double x,y,r; scanf("%lf%lf%lf", &x,&y,&r);
            x-=x0; y-=y0;
            double center=(x*dx+y*dy)/len;
            double perp=(x*dy-y*dx)/len;
            double h=r*r-perp*perp;
            if(h<=0) continue;
            double half=sqrt(h);
            double l=max(0.0,center-half), u=min(len,center+half);
            if(l<u) a.push_back(make_pair(l,u));
        }
        sort(a.begin(),a.end());
        double total=0, end=0;
        for(size_t i=0;i<a.size();++i) {
            if(a[i].second>end) {
                total+=a[i].second-max(end,a[i].first);
                end=a[i].second;
            }
        }
        printf("%.2f\n",100*total/len);
    }
    return 0;
}
