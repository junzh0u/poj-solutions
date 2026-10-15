// POJ 2016 - Ink Blots
// Model: gpt-6.1-sol
// Split circle boundaries at intersections and retain only exposed arcs.
// Their connected components are boundary loops. White regions equal
// boundary loops minus connected ink components plus one. Containment
// joins ink components but does not itself introduce boundary vertices.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double PI=acos(-1.0);
struct DSU {
    vector<int> p;
    DSU(int n):p(n) { for(int i=0;i<n;++i) p[i]=i; }
    int root(int x) { return p[x]==x?x:p[x]=root(p[x]); }
    void join(int a,int b) { p[root(a)]=root(b); }
};
struct Circle { double x,y,r; } c[100];
struct Point { double a; int id; Point(double a_,int i):a(a_),id(i) {} bool operator<(const Point& q)const{return a<q.a;} };
vector<Point> v[100];
double norm(double a) { if(a<0)a+=2*PI; if(a>=2*PI)a-=2*PI; return a; }
bool exposed(int i,double a,int n) {
    double x=c[i].x+c[i].r*cos(a), y=c[i].y+c[i].r*sin(a);
    for(int j=0;j<n;++j) if(i!=j) {
        double dx=x-c[j].x,dy=y-c[j].y;
        if(dx*dx+dy*dy<c[j].r*c[j].r) return false;
    }
    return true;
}
int main() {
    int n;
    while(scanf("%d",&n)==1 && n) {
        for(int i=0;i<n;++i) { scanf("%lf%lf%lf",&c[i].x,&c[i].y,&c[i].r); v[i].clear(); }
        DSU ink(n), boundary(10000); int vertices=0;
        for(int i=0;i<n;++i) for(int j=i+1;j<n;++j) {
            double dx=c[j].x-c[i].x,dy=c[j].y-c[i].y,d=sqrt(dx*dx+dy*dy);
            if(d<c[i].r+c[j].r) ink.join(i,j);
            if(d<=fabs(c[i].r-c[j].r) || d>=c[i].r+c[j].r) continue;
            double a=atan2(dy,dx), b=acos((c[i].r*c[i].r+d*d-c[j].r*c[j].r)/(2*c[i].r*d));
            for(int s=-1;s<=1;s+=2) {
                double t=a+s*b;
                double x=c[i].x+c[i].r*cos(t),y=c[i].y+c[i].r*sin(t);
                v[i].push_back(Point(norm(t),vertices));
                v[j].push_back(Point(norm(atan2(y-c[j].y,x-c[j].x)),vertices++));
            }
        }
        vector<int> used(vertices,0); int loops=0;
        for(int i=0;i<n;++i) {
            sort(v[i].begin(),v[i].end());
            int m=(int)v[i].size();
            if(!m) { if(exposed(i,0,n)) ++loops; continue; }
            for(int k=0;k<m;++k) {
                int l=(k+1)%m;
                double a=v[i][k].a,b=v[i][l].a;
                if(l==0) b+=2*PI;
                if(exposed(i,(a+b)/2,n)) {
                    int u=v[i][k].id,w=v[i][l].id;
                    used[u]=used[w]=1; boundary.join(u,w);
                }
            }
        }
        for(int i=0;i<vertices;++i) if(used[i] && boundary.root(i)==i) ++loops;
        int components=0;
        for(int i=0;i<n;++i) if(ink.root(i)==i) ++components;
        printf("%d\n",loops-components+1);
    }
    return 0;
}
