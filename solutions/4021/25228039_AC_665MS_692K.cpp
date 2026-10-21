// POJ 4021 - Kids Like Cakes
// Model: gpt-6-astra
// A smallest piece may be chosen as an ear of the original polygon.
// Find a maximum triangle with rotating calipers. All ears except those
// centered at its three vertices can use it; for those three ears, find
// a maximum triangle after deleting the ear's middle vertex. O(n^2) time.
// The original contest requires both triangle witnesses, omitted by the
// POJ discussion's abridged statement. Exact 64-bit doubled areas avoid
// rounding at coordinates up to 10^8; the actual judge supports this type.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long Int;
struct Point { Int x,y; int id; };
struct Tri { Int s; int a,b,c; Tri():s(-1),a(0),b(0),c(0){} };
Int area(const Point&a,const Point&b,const Point&c) {
    Int s=(b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
    return s<0?-s:s;
}
Tri largest(const vector<Point>&p) {
    Tri t; int n=p.size();
    for(int i=0;i<n-2;++i) {
        int k=i+2;
        for(int j=i+1;j<n-1;++j) {
            if(k<=j) k=j+1;
            while(k+1<n && area(p[i],p[j],p[k+1])>=area(p[i],p[j],p[k])) ++k;
            Int s=area(p[i],p[j],p[k]);
            if(s>t.s) {t.s=s;t.a=p[i].id;t.b=p[j].id;t.c=p[k].id;}
        }
    }
    return t;
}
int main() {
    int n; if(!(cin>>n)) return 0;
    vector<Point> p(n);
    for(int i=0;i<n;++i) {cin>>p[i].x>>p[i].y;p[i].id=i;}
    Tri all=largest(p),big; Int best=-1; int ear=0;
    for(int i=0;i<n;++i) {
        Tri t=all;
        if(i==all.a || i==all.b || i==all.c) {
            vector<Point> q;
            for(int j=0;j<n;++j) if(j!=i) q.push_back(p[j]);
            t=largest(q);
        }
        Int d=t.s-area(p[(i+n-1)%n],p[i],p[(i+1)%n]);
        if(d>best) {best=d;big=t;ear=i;}
    }
    cout<<best/2<<'.'<<(best%2?5:0)<<'\n';
    cout<<big.a+1<<' '<<big.b+1<<' '<<big.c+1<<'\n';
    cout<<(ear+n-1)%n+1<<' '<<ear+1<<' '<<(ear+1)%n+1<<'\n';
}
