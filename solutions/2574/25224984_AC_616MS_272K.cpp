// POJ 2574 - Saskatchewan
// Model: gpt-6.1-sol
// Sweep unit-height strips, pairing exact rational boundary intersections.
// Both endpoints constrain a whole square; no convexity is assumed.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long Int;
struct Point { int x,y; };
struct Cross {
    Int lo,hi,den;
    bool operator<(const Cross& b) const {
        return (lo+hi)*b.den < (b.lo+b.hi)*den;
    }
};
int main() {
    vector<Point> p; Point t;
    int ymin=100001,ymax=0;
    while(cin>>t.x>>t.y) { p.push_back(t); ymin=min(ymin,t.y); ymax=max(ymax,t.y); }
    Int answer=0;
    for(int y=ymin;y<ymax;++y) {
        vector<Cross> cuts;
        for(unsigned i=0;i<p.size();++i) {
            Point a=p[i],b=p[(i+1)%p.size()];
            if(a.y>b.y) swap(a,b);
            if(a.y>y || b.y<=y) continue;
            Cross c; c.den=b.y-a.y;
            c.lo=Int(a.x)*c.den+Int(b.x-a.x)*(y-a.y);
            c.hi=Int(a.x)*c.den+Int(b.x-a.x)*(y+1-a.y);
            cuts.push_back(c);
        }
        sort(cuts.begin(),cuts.end());
        for(unsigned i=0;i+1<cuts.size();i+=2) {
            Cross a=cuts[i],b=cuts[i+1];
            Int left=(max(a.lo,a.hi)+a.den-1)/a.den;
            Int right=min(b.lo,b.hi)/b.den;
            if(right>left) answer+=right-left;
        }
    }
    cout<<answer<<'\n';
}
