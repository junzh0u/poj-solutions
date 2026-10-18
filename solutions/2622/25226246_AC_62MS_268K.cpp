// POJ 2622 - Convex hull
// Model: gpt-6-astra
// The minimal convex region is the intersection of the eight supporting
// half-planes with axis-parallel or diagonal boundaries. Enumerate feasible
// intersections, then take their counterclockwise convex hull to discard
// duplicate and collinear vertices. Degenerate hulls have one or two vertices.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
const double EPS=1e-8;
struct Point {
    double x,y;
    Point(double X=0,double Y=0):x(X),y(Y) {}
    bool operator<(const Point& p) const {return x<p.x || (x==p.x && y<p.y);}
};
double cross(Point a,Point b,Point c) {
    return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
int main() {
    int n;
    if(scanf("%d",&n)!=1) return 0;
    const int a[8]={1,1,0,-1,-1,-1,0,1};
    const int b[8]={0,1,1,1,0,-1,-1,-1};
    double c[8];
    for(int i=0;i<8;++i) c[i]=-1e100;
    for(int k=0;k<n;++k) {
        double x,y; scanf("%lf%lf",&x,&y);
        for(int i=0;i<8;++i) c[i]=max(c[i],a[i]*x+b[i]*y);
    }
    vector<Point> p;
    for(int i=0;i<8;++i) for(int j=i+1;j<8;++j) {
        int det=a[i]*b[j]-a[j]*b[i];
        if(!det) continue;
        Point q((c[i]*b[j]-c[j]*b[i])/det,
                (a[i]*c[j]-a[j]*c[i])/det);
        bool ok=true;
        for(int k=0;k<8;++k)
            if(a[k]*q.x+b[k]*q.y>c[k]+EPS) ok=false;
        if(ok) {
            // Give numerically equal coordinates one exact representation so
            // lexicographic sorting cannot split a vertical hull edge.
            for(size_t k=0;k<p.size();++k) {
                if(fabs(q.x-p[k].x)<EPS) q.x=p[k].x;
                if(fabs(q.y-p[k].y)<EPS) q.y=p[k].y;
            }
            p.push_back(q);
        }
    }
    sort(p.begin(),p.end());
    vector<Point> u;
    for(size_t i=0;i<p.size();++i)
        if(u.empty() || fabs(p[i].x-u.back().x)>EPS ||
                        fabs(p[i].y-u.back().y)>EPS) u.push_back(p[i]);
    vector<Point> h;
    for(size_t i=0;i<u.size();++i) {
        while(h.size()>1 && cross(h[h.size()-2],h.back(),u[i])<=EPS) h.pop_back();
        h.push_back(u[i]);
    }
    size_t lower=h.size();
    for(int i=(int)u.size()-2;i>=0;--i) {
        while(h.size()>lower && cross(h[h.size()-2],h.back(),u[i])<=EPS) h.pop_back();
        h.push_back(u[i]);
    }
    if(h.size()>1) h.pop_back();
    size_t start=0;
    for(size_t i=1;i<h.size();++i)
        if(h[i].y<h[start].y || (h[i].y==h[start].y && h[i].x<h[start].x)) start=i;
    for(size_t i=0;i<h.size();++i) {
        Point q=h[(start+i)%h.size()];
        printf("%.10g %.10g\n",fabs(q.x)<EPS?0:q.x,fabs(q.y)<EPS?0:q.y);
    }
    return 0;
}
