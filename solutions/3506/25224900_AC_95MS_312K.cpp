// POJ 3506 - Walk
// Model: gpt-6.1-sol
// Only contours separating the endpoints must be crossed. Exit Alice's
// nested contours in increasing area, then enter Bob's in decreasing area.
// Endpoint altitudes are unknown, so only differences between crossings count.
// Double arithmetic is exact for the bounded integral shoelace sums here.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
struct Point { double x,y; };
struct Curve { double area; int h; };
bool lessArea(const Curve &a,const Curve &b) { return a.area<b.area; }
bool inside(const vector<Point>& p,double x) {
    bool in=false;
    for(int i=0,j=(int)p.size()-1;i<(int)p.size();j=i++) {
        if((p[i].y>0)!=(p[j].y>0)) {
            double cross=(p[j].x-p[i].x)*(-p[i].y)-(x-p[i].x)*(p[j].y-p[i].y);
            if((cross>0)==(p[j].y>p[i].y)) in=!in;
        }
    }
    return in;
}
int main() {
    int t; scanf("%d",&t);
    while(t--) {
        int n; scanf("%d",&n); vector<Curve>a,b;
        for(int k=0;k<n;k++) {
            int h,m; scanf("%d%d",&h,&m); vector<Point>p(m);
            for(int i=0;i<m;i++) scanf("%lf%lf",&p[i].x,&p[i].y);
            bool ia=inside(p,0),ib=inside(p,100000);
            if(ia==ib) continue;
            double ar=0;
            for(int i=0,j=m-1;i<m;j=i++) ar+=p[j].x*p[i].y-p[j].y*p[i].x;
            Curve c; c.area=fabs(ar); c.h=h;
            if(ia) a.push_back(c); else b.push_back(c);
        }
        sort(a.begin(),a.end(),lessArea); sort(b.begin(),b.end(),lessArea);
        for(int i=(int)b.size()-1;i>=0;i--) a.push_back(b[i]);
        int up=0,down=0;
        for(int i=1;i<(int)a.size();i++) {
            int d=a[i].h-a[i-1].h;
            if(d>0) up+=d; else down-=d;
        }
        printf("%d %d\n",up,down);
    }
    return 0;
}
