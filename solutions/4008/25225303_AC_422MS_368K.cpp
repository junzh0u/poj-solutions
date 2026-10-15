// POJ 4008 - Running relay
// Model: gpt-6.1-sol
// Give every runner d meters, then take convex combinations of (s,t).
// The lower convex hull gives the least good-mood time for each bad-mood
// budget. Distances are continuous, as the statement's fractional sample shows.
// Double arithmetic keeps all integer sums and cross products exact here.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Point { double s,t; };
bool lessPoint(const Point &a,const Point &b) {
    return a.s<b.s || (a.s==b.s && a.t<b.t);
}
double cross(const Point &a,const Point &b,const Point &c) {
    return (b.s-a.s)*(c.t-a.t)-(b.t-a.t)*(c.s-a.s);
}
int main() {
    int cases; scanf("%d",&cases);
    while(cases--) {
        int n,d,L,W; scanf("%d%d%d%d",&n,&d,&L,&W);
        vector<Point> p(n),h;
        double baseS=0,baseT=0;
        for(int i=0;i<n;++i) {
            scanf("%lf%lf",&p[i].s,&p[i].t);
            baseS+=d*p[i].s; baseT+=d*p[i].t;
        }
        int rem=L-n*d;
        if(rem<0 || baseS>W) { puts("No solution"); continue; }
        if(rem==0) { printf("%.2f\n",baseT); continue; }
        sort(p.begin(),p.end(),lessPoint);
        if(baseS+rem*p[0].s>W) { puts("No solution"); continue; }
        for(int i=0;i<n;++i) {
            if(i && p[i].s==p[i-1].s) continue;
            while(h.size()>=2 && cross(h[h.size()-2],h.back(),p[i])<=0)
                h.pop_back();
            h.push_back(p[i]);
        }
        double budget=(W-baseS)/rem, best=1e100;
        for(unsigned i=0;i<h.size();++i) {
            if(h[i].s<=budget) best=min(best,h[i].t);
            if(i && h[i-1].s<=budget && budget<h[i].s) {
                double f=(budget-h[i-1].s)/(h[i].s-h[i-1].s);
                best=min(best,h[i-1].t+f*(h[i].t-h[i-1].t));
            }
        }
        printf("%.2f\n",baseT+rem*best);
    }
    return 0;
}
