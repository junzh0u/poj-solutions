// POJ 3873 - Trick or Treat
// Model: gpt-6-astra
// Squared distances share x*x. Build the upper envelope of their lines,
// then find where its piecewise quadratic derivative changes sign.
// Sort decreasing house x; equal x keeps the largest absolute y.
// Factor differences of squares at intersections to avoid cancellation.
// Negative coordinates are allowed, as the statement's sample confirms.
// O(n log n) time and O(n) memory; cases end at n=0.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
struct House { long double x,y; };
bool order(const House &a,const House &b) {
    return a.x!=b.x ? a.x>b.x : a.y>b.y;
}
long double cross(const House &a,const House &b) {
    return (a.x+b.x)/2+(a.y-b.y)*(a.y+b.y)/(2*(a.x-b.x));
}
int main() {
    int n;
    while(scanf("%d",&n)==1 && n) {
        vector<House> p(n),h;
        vector<long double> start;
        for(int i=0;i<n;++i) {
            double x,y; scanf("%lf%lf",&x,&y);
            p[i].x=x; p[i].y=fabs(y);
        }
        sort(p.begin(),p.end(),order);
        for(int i=0;i<n;++i) {
            if(i && p[i].x==p[i-1].x) continue;
            long double s=-1e100L;
            while(!h.empty()) {
                s=cross(h.back(),p[i]);
                if(s>start.back()) break;
                h.pop_back(); start.pop_back();
            }
            if(h.empty()) s=-1e100L;
            h.push_back(p[i]); start.push_back(s);
        }
        long double x=0;
        for(size_t i=0;i<h.size();++i) {
            long double end=i+1<h.size()?start[i+1]:1e100L;
            if(h[i].x<=end) { x=max(h[i].x,start[i]); break; }
        }
        long double d=0;
        for(int i=0;i<n;++i) {
            long double dx=x-p[i].x;
            d=max(d,dx*dx+p[i].y*p[i].y);
        }
        printf("%.10f %.10f\n",(double)x,(double)sqrt(d));
    }
    return 0;
}
