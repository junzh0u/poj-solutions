// POJ 2824 - Bohemian Rhapsody
// Model: gpt-6.1-sol
// Maintain the boundary arcs of the intersection of unit disks centered at
// negative prefix displacements. Green's theorem gives each survival area;
// linearity of expectation weights it by that step's prize. Boundary points
// do not survive, so a tangent-only intersection earns no further prizes.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0), TAU=2*PI;
struct Point { double x,y; Point(double a=0,double b=0):x(a),y(b){} };
struct Arc { Point c; double a,b; Arc(Point p,double l,double r):c(p),a(l),b(r){} };
void clip(const Arc &s, Point c, vector<Arc> &out) {
    double dx=c.x-s.c.x,dy=c.y-s.c.y;
    double d=sqrt(dx*dx+dy*dy);
    if(d<1e-13) { out.push_back(s); return; }
    if(d>=2.0) return;
    double mid=atan2(dy,dx), h=acos(d/2);
    for(int k=-1;k<=2;k++) {
        double l=max(s.a,mid-h+k*TAU),r=min(s.b,mid+h+k*TAU);
        if(r>l+1e-14) out.push_back(Arc(s.c,l,r));
    }
}
double area(const vector<Arc> &v) {
    double z=0;
    for(size_t i=0;i<v.size();i++) {
        const Arc &s=v[i];
        z+=s.b-s.a+s.c.x*(sin(s.b)-sin(s.a))
            +s.c.y*(cos(s.a)-cos(s.b));
    }
    return max(0.0,min(PI,z/2));
}
int main() {
    int n,cas=0;
    while(scanf("%d",&n)==1 && n) {
        vector<Point> centers(1,Point());
        vector<Arc> arcs(1,Arc(Point(),0,TAU));
        double x=0,y=0,expected=0;
        double sum=0,best=0;
        for(int i=0;i<n;i++) {
            double dx,dy; int prize;
            scanf("%lf%lf%d",&dx,&dy,&prize);
            x-=dx;y-=dy;Point c(x,y);
            bool duplicate=false;
            for(size_t j=0;j<centers.size();j++)
                if(fabs(c.x-centers[j].x)<1e-13 && fabs(c.y-centers[j].y)<1e-13) duplicate=true;
            if(!duplicate && !arcs.empty()) {
                vector<Arc> next;
                for(size_t j=0;j<arcs.size();j++) clip(arcs[j],c,next);
                vector<Arc> fresh(1,Arc(c,0,TAU));
                for(size_t j=0;j<centers.size() && !fresh.empty();j++) {
                    vector<Arc> tmp;
                    for(size_t k=0;k<fresh.size();k++) clip(fresh[k],centers[j],tmp);
                    fresh.swap(tmp);
                }
                next.insert(next.end(),fresh.begin(),fresh.end());
                arcs.swap(next);centers.push_back(c);
            }
            expected+=prize*area(arcs)/PI;
            if(!arcs.empty()) { sum+=prize;best=max(best,sum); }
        }
        printf("Case %d:\nThe expected amount of money: %.3f\nThe maximum amount of money: %.0f\n",++cas,expected,best);
    }
    return 0;
}
