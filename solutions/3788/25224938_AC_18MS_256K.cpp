// POJ 3788 - Interior Points of Lattice Polygons
// Model: gpt-6.1-sol
// Intersect strict clockwise edge half-planes with each integer scan line.
// Exact signed division excludes all boundary points, including vertices.
// The statement gives no coordinate bound; use 64-bit intermediate arithmetic.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long Int;
struct Point { Int x,y; };
struct Row { Int y,l,r; Row(Int a,Int b,Int c):y(a),l(b),r(c){} };
Int floorDiv(Int a,Int b) { Int q=a/b,r=a%b; return q-(r<0); }
Int ceilDiv(Int a,Int b) { Int q=a/b,r=a%b; return q+(r>0); }
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int cases; cin>>cases;
    while(cases--) {
        int id,n; cin>>id>>n; vector<Point> p(n);
        Int ymin=0,ymax=0,xmin=0,xmax=0;
        for(int i=0;i<n;++i) {
            cin>>p[i].x>>p[i].y;
            if(i==0) ymin=ymax=p[i].y,xmin=xmax=p[i].x;
            ymin=min(ymin,p[i].y); ymax=max(ymax,p[i].y);
            xmin=min(xmin,p[i].x); xmax=max(xmax,p[i].x);
        }
        vector<Row> rows;
        for(Int y=ymax-1;y>ymin;--y) {
            Int l=xmin,r=xmax;
            for(int i=0;i<n;++i) {
                Point a=p[i],b=p[(i+1)%n];
                Int dx=b.x-a.x,dy=b.y-a.y;
                Int num=a.x*dy+dx*(y-a.y);
                if(dy>0) l=max(l,floorDiv(num,dy)+1);
                else if(dy<0) r=min(r,ceilDiv(-num,-dy)-1);
                else if(dx*(y-a.y)>=0) { l=1; r=0; break; }
            }
            if(l<=r) rows.push_back(Row(y,l,r));
        }
        cout<<id<<' '<<rows.size()<<'\n';
        for(size_t i=0;i<rows.size();++i)
            cout<<rows[i].y<<' '<<rows[i].l<<' '<<rows[i].r<<'\n';
    }
}
