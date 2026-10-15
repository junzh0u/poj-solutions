// POJ 2423 - The Parallel Challenge Ballgame
// Model: gpt-6.1-sol
// Reflect each goal in each wall, then unite their angular projections.
// A ball initially in or on a goal scores in every direction.
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
using namespace std;
const double PI=acos(-1.0), TAU=2*PI;
struct Rect { double l,b,r,t; };
vector<pair<double,double> > seg;
double px,py;
void add(Rect a) {
    double v[4]; int k=0;
    for(int i=0;i<2;i++) for(int j=0;j<2;j++) {
        double z=atan2((j?a.t:a.b)-py,(i?a.r:a.l)-px);
        if(z<0) z+=TAU;
        v[k++]=z;
    }
    sort(v,v+4);
    double gap=-1; int at=0;
    for(int i=0;i<4;i++) {
        double d=(i==3?v[0]+TAU:v[i+1])-v[i];
        if(d>gap) gap=d,at=i;
    }
    double s=v[(at+1)%4],e=v[at];
    if(e<s) {seg.push_back(make_pair(s,TAU));seg.push_back(make_pair(0.0,e));}
    else seg.push_back(make_pair(s,e));
}
int main() {
    int tests; scanf("%d",&tests);
    while(tests--) {
        double x1,y1,x2,y2; scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
        if(x1>x2) swap(x1,x2); if(y1>y2) swap(y1,y2);
        scanf("%lf%lf",&px,&py); int n; scanf("%d",&n);
        seg.clear(); bool all=false;
        for(int i=0;i<n;i++) {
            Rect a; scanf("%lf%lf%lf%lf",&a.l,&a.b,&a.r,&a.t);
            if(a.l>a.r) swap(a.l,a.r); if(a.b>a.t) swap(a.b,a.t);
            if(a.l<=px&&px<=a.r&&a.b<=py&&py<=a.t) all=true;
            else {
                add(a); Rect z=a;
                z.l=2*x1-a.r; z.r=2*x1-a.l; add(z);
                z.l=2*x2-a.r; z.r=2*x2-a.l; add(z);
                z=a; z.b=2*y1-a.t; z.t=2*y1-a.b; add(z);
                z.b=2*y2-a.t; z.t=2*y2-a.b; add(z);
            }
        }
        sort(seg.begin(),seg.end()); double sum=0,s=0,e=0;
        for(size_t i=0;i<seg.size();i++) {
            if(seg[i].first>e) {sum+=e-s;s=seg[i].first;e=seg[i].second;}
            else if(seg[i].second>e) e=seg[i].second;
        }
        sum+=e-s;
        printf("%.2f%%\n",all?100.0:sum/TAU*100);
    }
    return 0;
}
