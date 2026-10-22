// POJ 3466 - Distance to the Prey
// Model: gpt-6-astra
// Each web has a boundary staircase joining its endpoints through its peak.
// Every component off that staircase attaches at one vertex. The five child
// staircases form the parent staircase and two chains of unit squares.
// Project each query onto its child staircase; recurse if both share a child,
// otherwise join their projections in this skeleton. Distances in a square
// chain are Manhattan distances, so all diagonal contacts are preserved.
// Input consists of cases to EOF. Geometric intersections permit switching
// branches; treating the recursively drawn curve as a simple path is wrong.
#include <iostream>
#include <cstdlib>
using namespace std;
typedef long long Int;
struct Point {
    Int x,y;
    Point(Int a=0,Int b=0):x(a),y(b) {}
};
struct Projection {
    Point p;
    Int d;
    Projection(Point a,Int b):p(a),d(b) {}
};
Int power3[21];
Int ab(Int x) { return x<0 ? -x:x; }
Int manhattan(Point p,Point q) { return ab(p.x-q.x)+ab(p.y-q.y); }
Point local(Point p,int i,Int l) {
    if(i==0) return p;
    if(i==1) return Point(p.y,l-p.x);
    if(i==2) return Point(p.x-l,p.y-l);
    if(i==3) return Point(l-p.y,p.x-2*l);
    return Point(p.x-2*l,p.y);
}
Point global(Point p,int i,Int l) {
    if(i==0) return p;
    if(i==1) return Point(l-p.y,p.x);
    if(i==2) return Point(l+p.x,l+p.y);
    if(i==3) return Point(2*l+p.y,l-p.x);
    return Point(2*l+p.x,p.y);
}
bool inside(Point p,Int l) { return p.y>=0 && p.y<=p.x && p.x+p.y<=l; }
bool boundary(Point p,Int l) { return p.x-p.y<=1 || p.x+p.y>=l-1; }
Int position(Point p,Int l) {
    if(p.x-p.y<=1) return p.x+p.y;
    return 2*l-1-(l-p.x+p.y);
}
Projection skeleton(Point p,Int l) {
    if(boundary(p,l)) return Projection(p,0);
    Int h=(l/3-1)/2;
    Point root(p.x<l/2 ? h+1:l-h-1,h);
    return Projection(root,manhattan(p,root));
}
Projection project(int n,Point p) {
    Int l=power3[n];
    if(boundary(p,l)) return Projection(p,0);
    Int child=l/3;
    int i=0;
    while(!inside(local(p,i,child),child)) ++i;
    Projection a=project(n-1,local(p,i,child));
    Projection b=skeleton(global(a.p,i,child),l);
    return Projection(b.p,a.d+b.d);
}
Int distance(int n,Point p,Point q) {
    if(p.x==q.x && p.y==q.y) return 0;
    if(n==0) return 1;
    Int l=power3[n],child=l/3;
    int ip=-1,iq=-1;
    for(int i=0;i<5;++i) {
        bool a=inside(local(p,i,child),child);
        bool b=inside(local(q,i,child),child);
        if(a && b) return distance(n-1,local(p,i,child),local(q,i,child));
        if(a) ip=i;
        if(b) iq=i;
    }
    Projection a=project(n-1,local(p,ip,child));
    Projection b=project(n-1,local(q,iq,child));
    Point r=global(a.p,ip,child),s=global(b.p,iq,child);
    Projection c=skeleton(r,l),d=skeleton(s,l);
    if(c.d && d.d && (r.x<l/2)==(s.x<l/2))
        return a.d+b.d+manhattan(r,s);
    return a.d+b.d+c.d+d.d+ab(position(c.p,l)-position(d.p,l));
}
int main() {
    power3[0]=1;
    for(int i=1;i<=20;++i) power3[i]=3*power3[i-1];
    Point p,q;
    while(cin>>p.x>>p.y>>q.x>>q.y) cout<<distance(20,p,q)<<'\n';
    return 0;
}
