// POJ 3703 - Intuition of Escape
// Model: gpt-6.1-sol
// Project each edge expanded by the vehicle radius onto the direction circle.
// Its capsule is convex and excludes the origin, so its angular shadow is
// the interval between the extreme tangents to its two endpoint disks.
// Merge all closed shadows; any uncovered direction permits escape.
// Touching an alert area is forbidden, including tangency.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
const double PI=acos(-1.0), TAU=2*PI, EPS=1e-10;
struct Point { double x,y; };
int main() {
    int n;
    if(scanf("%d",&n)!=1) return 0;
    double r; scanf("%lf",&r);
    vector<pair<double,double> > ranges;
    for(int p=0;p<n;++p) {
        int k; scanf("%d",&k);
        vector<Point> a(k);
        for(int i=0;i<k;++i) scanf("%lf%lf",&a[i].x,&a[i].y);
        for(int i=0;i<k;++i) {
            Point u=a[i],v=a[(i+1)%k];
            double t=atan2(u.y,u.x),s=atan2(v.y,v.x);
            while(s-t>PI) s-=TAU;
            while(s-t<-PI) s+=TAU;
            double x=asin(min(1.0,r/sqrt(u.x*u.x+u.y*u.y)));
            double y=asin(min(1.0,r/sqrt(v.x*v.x+v.y*v.y)));
            double lo=min(t-x,s-y),hi=max(t+x,s+y);
            while(lo<0) { lo+=TAU; hi+=TAU; }
            while(lo>=TAU) { lo-=TAU; hi-=TAU; }
            if(hi>=TAU) {
                ranges.push_back(make_pair(lo,TAU));
                ranges.push_back(make_pair(0.0,hi-TAU));
            } else ranges.push_back(make_pair(lo,hi));
        }
    }
    sort(ranges.begin(),ranges.end());
    double end=0;
    for(size_t i=0;i<ranges.size();++i) {
        if(ranges[i].first>end+EPS) { puts("Survive!"); return 0; }
        end=max(end,ranges[i].second);
    }
    puts(end<TAU-EPS ? "Survive!" : "Baffling!");
    return 0;
}
