// POJ 3774 - Elune's Arrow
// Model: gpt-6.1-sol
// Clip the forward ray against every CCW polygon's closed half-planes.
// The polygon with the smallest entry parameter is hit; boundary contact counts.
// Integer cross products and continued-fraction comparisons avoid roundoff
// and overflow in products of rational parameter numerators/denominators.
#include <iostream>
#include <vector>
using namespace std;
typedef long long Int;
struct Point { Int x,y; };
struct Rat { Int n,d; Rat(Int a=0,Int b=1):n(a),d(b){} };
int compare(Rat a,Rat b) {
    int sign=1;
    for (;;) {
        Int q=a.n/a.d, r=b.n/b.d;
        if(q!=r) return sign*(q<r?-1:1);
        a.n%=a.d; b.n%=b.d;
        if(!a.n || !b.n) return sign*(a.n==b.n?0:(!a.n?-1:1));
        Int t=a.n; a.n=a.d; a.d=t;
        t=b.n; b.n=b.d; b.d=t;
        sign=-sign;
    }
}
Int cross(Point a,Point b) { return a.x*b.y-a.y*b.x; }
Point sub(Point a,Point b) { Point c={a.x-b.x,a.y-b.y}; return c; }
bool entry(const vector<Point>& p,Point o,Point d,Rat& lo) {
    lo=Rat(); Rat hi; bool bounded=false;
    for(unsigned i=0;i<p.size();++i) {
        Point e=sub(p[(i+1)%p.size()],p[i]);
        Int c=cross(e,sub(o,p[i])), b=cross(e,d);
        if(b==0) { if(c<0) return false; }
        else if(b>0) {
            if(c<0) { Rat v(-c,b); if(compare(v,lo)>0) lo=v; }
        } else {
            if(c<0) return false;
            Rat v(c,-b);
            if(!bounded || compare(v,hi)<0) hi=v;
            bounded=true;
        }
    }
    return (d.x || d.y) && (!bounded || compare(lo,hi)<=0);
}
int main() {
    int n;
    while(cin>>n && n) {
        Point o,d; cin>>o.x>>o.y>>d.x>>d.y;
        int best=-1; Rat nearest;
        for(int i=0;i<n;++i) {
            int m; cin>>m; vector<Point> p(m);
            for(int j=0;j<m;++j) cin>>p[j].x>>p[j].y;
            Rat t;
            if(entry(p,o,d,t) && (best<0 || compare(t,nearest)<0)) {
                nearest=t; best=i;
            }
        }
        cout<<(best==0?"HIT":"MISS")<<'\n';
    }
}
