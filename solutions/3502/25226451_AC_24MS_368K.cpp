// POJ 3502 - Flight Safety
// Model: gpt-6-astra
// Rotate each route leg onto the x-axis. Binary-search the radius whose
// continent interiors and edge capsules cover the complete leg.
// Polygons may be concave; even-odd crossings give their interior intervals.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
struct P { double x,y; P(double X=0,double Y=0):x(X),y(Y){} };
struct Edge { double ux,uy,q,h,len; };
vector<P> vertices;
vector<Edge> edges;
vector<pair<double,double> > land, intervals;
double length;
bool clip(double a,double b,double low,double high,double &l,double &r) {
    if(fabs(a)<1e-14) return b>=low-1e-10 && b<=high+1e-10;
    double x=(low-b)/a,y=(high-b)/a;
    if(x>y) swap(x,y);
    l=max(l,x); r=min(r,y); return l<=r;
}
bool covered(double rad) {
    intervals=land;
    for(size_t i=0;i<vertices.size();++i) {
        P p=vertices[i];
        if(fabs(p.y)>rad) continue;
        double dx=sqrt(max(0.0,rad*rad-p.y*p.y));
        double l=max(0.0,p.x-dx),r=min(length,p.x+dx);
        if(l<=r) intervals.push_back(make_pair(l,r));
    }
    for(size_t i=0;i<edges.size();++i) {
        Edge e=edges[i]; double l=0,r=length;
        if(clip(e.ux,e.q,0,e.len,l,r) && clip(-e.uy,e.h,-rad,rad,l,r))
            intervals.push_back(make_pair(l,r));
    }
    sort(intervals.begin(),intervals.end());
    double reach=0;
    for(size_t i=0;i<intervals.size();++i) {
        if(intervals[i].first>reach+1e-9) return false;
        reach=max(reach,intervals[i].second);
        if(reach>=length-1e-9) return true;
    }
    return false;
}
int main() {
    int tc; if(scanf("%d",&tc)!=1) return 0;
    while(tc--) {
        int c,n; scanf("%d%d",&c,&n);
        vector<P> route(n); vector<vector<P> > polys(c);
        for(int i=0;i<n;++i) scanf("%lf%lf",&route[i].x,&route[i].y);
        for(int j=0;j<c;++j) {
            int m; scanf("%d",&m); polys[j].resize(m);
            for(int i=0;i<m;++i) scanf("%lf%lf",&polys[j][i].x,&polys[j][i].y);
        }
        double answer=0;
        for(int k=1;k<n;++k) {
            P origin=route[k-1]; double ux=route[k].x-origin.x,uy=route[k].y-origin.y;
            length=sqrt(ux*ux+uy*uy); if(length<1e-12) continue;
            ux/=length; uy/=length; vertices.clear(); edges.clear(); land.clear();
            for(int j=0;j<c;++j) {
                int m=polys[j].size(); vector<P> v(m); vector<double> crossings;
                for(int i=0;i<m;++i) {
                    double x=polys[j][i].x-origin.x,y=polys[j][i].y-origin.y;
                    v[i]=P(x*ux+y*uy,-x*uy+y*ux);
                    if(fabs(v[i].y)<1e-10) v[i].y=0;
                    vertices.push_back(v[i]);
                }
                for(int i=0;i<m;++i) {
                    P a=v[i],b=v[(i+1)%m];
                    if((a.y>0)!=(b.y>0)) crossings.push_back(a.x+(b.x-a.x)*(-a.y)/(b.y-a.y));
                    double dx=b.x-a.x,dy=b.y-a.y,d=sqrt(dx*dx+dy*dy);
                    if(d<1e-12) continue;
                    Edge e; e.ux=dx/d; e.uy=dy/d; e.len=d;
                    e.q=-a.x*e.ux-a.y*e.uy; e.h=a.x*e.uy-a.y*e.ux; edges.push_back(e);
                }
                sort(crossings.begin(),crossings.end());
                for(size_t i=1;i<crossings.size();i+=2) {
                    double l=max(0.0,crossings[i-1]),r=min(length,crossings[i]);
                    if(l<=r) land.push_back(make_pair(l,r));
                }
            }
            if(covered(answer)) continue;
            double lo=answer,hi=30000;
            while(hi-lo>0.0000001) {
                double mid=(lo+hi)/2;
                if(covered(mid)) hi=mid; else lo=mid;
            }
            answer=hi;
        }
        printf("%.6f\n",answer);
    }
    return 0;
}
