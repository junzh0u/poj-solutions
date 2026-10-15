// POJ 3689 - Equations
// Model: gpt-6.1-sol
// Normalize each variable by A+B. Feasible objectives form a convex hull
// of (A/(A+B), C/(A+B)); its upper chain gives every query optimum.
// Integer determinants decide ordering, turns, and feasibility exactly.
// A query at an endpoint also works when only one distinct point exists.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct P { long long a,b,c,d; };
bool lessP(const P& p,const P& q) {
    long long v=p.a*q.d-q.a*p.d;
    if(v) return v<0;
    return p.c*q.d>q.c*p.d;
}
long long turn(const P& p,const P& q,const P& r) {
    return p.a*(q.c*r.d-q.d*r.c)-p.c*(q.a*r.d-q.d*r.a)
          +p.d*(q.a*r.c-q.c*r.a);
}
int main() {
    int n,m;
    if(scanf("%d%d",&n,&m)!=2) return 0;
    vector<P> v(n),h;
    for(int i=0;i<n;++i) {
        int a,b,c; scanf("%d%d%d",&a,&b,&c);
        v[i].a=a; v[i].b=b; v[i].c=c; v[i].d=(long long)a+b;
    }
    sort(v.begin(),v.end(),lessP);
    for(int i=0;i<n;++i) {
        if(i && v[i].a*v[i-1].d==v[i-1].a*v[i].d) continue;
        while(h.size()>1 && turn(h[h.size()-2],h.back(),v[i])>=0) h.pop_back();
        h.push_back(v[i]);
    }
    while(m--) {
        int s,t; scanf("%d%d",&s,&t);
        long long d=(long long)s+t;
        if((long long)s*h.front().d<h.front().a*d ||
           (long long)s*h.back().d>h.back().a*d) { puts("IMPOSSIBLE"); continue; }
        int l=0,r=(int)h.size()-1;
        while(l<r) {
            int mid=(l+r+1)/2;
            if(h[mid].a*d<=(long long)s*h[mid].d) l=mid; else r=mid-1;
        }
        double ans;
        if(h[l].a*d==(long long)s*h[l].d)
            ans=(double)h[l].c*d/h[l].d;
        else {
            const P &p=h[l],&q=h[l+1];
            long long det=p.a*q.b-q.a*p.b;
            long long num=p.c*((long long)s*q.b-q.a*t)+q.c*(p.a*t-(long long)s*p.b);
            ans=(double)num/det;
        }
        printf("%.5f\n",ans);
    }
    return 0;
}
