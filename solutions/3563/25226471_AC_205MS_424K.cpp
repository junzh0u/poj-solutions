// POJ 3563 - Parabolic teleports
// Model: gpt-6-astra
// Contract each teleport into one graph vertex. Edge weights are minimum
// Euclidean distances, attained at endpoints, intersections, or common normals.
// Cubic roots are isolated by derivative subdivision, retaining critical points
// as extra feasible candidates so repeated roots are not lost. Dijkstra then
// finds the minimum travel time; movement along a teleport costs zero.
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long double Real;
struct Curve {
    Real a,b,c,l,r;
    Real y(Real x) const { return (a*x+b)*x+c; }
};
Real eval(const vector<Real>& p, Real x) {
    Real v=0;
    for(int i=(int)p.size()-1;i>=0;--i) v=v*x+p[i];
    return v;
}
// Includes every real root in [l,r], plus harmless feasible candidates.
vector<Real> candidates(vector<Real> p, Real l, Real r) {
    while(p.size()>1 && p.back()==0) p.pop_back();
    vector<Real> cuts;
    cuts.push_back(l); cuts.push_back(r);
    if(p.size()==1) return cuts;
    vector<Real> deriv;
    for(int i=1;i<(int)p.size();++i) deriv.push_back(i*p[i]);
    vector<Real> extra=candidates(deriv,l,r);
    cuts.insert(cuts.end(),extra.begin(),extra.end());
    sort(cuts.begin(),cuts.end());
    cuts.erase(unique(cuts.begin(),cuts.end()),cuts.end());
    vector<Real> ans=cuts;
    for(int i=1;i<(int)cuts.size();++i) {
        Real lo=cuts[i-1],hi=cuts[i],v=eval(p,lo),w=eval(p,hi);
        if(!((v<0 && w>0)||(v>0 && w<0))) continue;
        for(int k=0;k<100;++k) {
            Real mid=(lo+hi)/2,z=eval(p,mid);
            if(mid==lo || mid==hi) break;
            if(z==0) {lo=hi=mid;break;}
            if((z<0)==(v<0)) {lo=mid;v=z;} else hi=mid;
        }
        ans.push_back((lo+hi)/2);
    }
    return ans;
}
Real sq(Real x) {return x*x;}
Real pointDistance(Real x, Real y, const Curve& q) {
    vector<Real> p(4);
    p[0]=q.b*(q.c-y)-x;
    p[1]=1+q.b*q.b+2*q.a*(q.c-y);
    p[2]=3*q.a*q.b;
    p[3]=2*q.a*q.a;
    vector<Real> roots=candidates(p,q.l,q.r);
    Real best=1e40L;
    for(size_t i=0;i<roots.size();++i)
        best=min(best,sq(roots[i]-x)+sq(q.y(roots[i])-y));
    return sqrt(best);
}
Real curveDistance(const Curve& p,const Curve& q) {
    Real best=min(pointDistance(p.l,p.y(p.l),q),pointDistance(p.r,p.y(p.r),q));
    best=min(best,pointDistance(q.l,q.y(q.l),p));
    best=min(best,pointDistance(q.r,q.y(q.r),p));
    Real lo=max(p.l,q.l),hi=min(p.r,q.r);
    if(lo<=hi) {
        vector<Real> poly(3);
        poly[0]=p.c-q.c;poly[1]=p.b-q.b;poly[2]=p.a-q.a;
        vector<Real> roots=candidates(poly,lo,hi);
        for(size_t i=0;i<roots.size();++i)
            best=min(best,fabsl(p.y(roots[i])-q.y(roots[i])));
    }
    // Equal tangent slopes imply u=alpha*x+beta on the second curve.
    Real alpha=p.a/q.a,beta=(p.b-q.b)/(2*q.a);
    Real x1=(q.l-beta)/alpha,x2=(q.r-beta)/alpha;
    lo=max(p.l,min(x1,x2));hi=min(p.r,max(x1,x2));
    if(lo<=hi) {
        Real c0=p.c-((q.a*beta+q.b)*beta+q.c);
        Real c1=p.b-(2*q.a*beta+q.b)*alpha;
        Real c2=p.a-q.a*alpha*alpha;
        vector<Real> poly(4);
        poly[0]=-beta+p.b*c0;
        poly[1]=1-alpha+2*p.a*c0+p.b*c1;
        poly[2]=2*p.a*c1+p.b*c2;
        poly[3]=2*p.a*c2;
        vector<Real> roots=candidates(poly,lo,hi);
        for(size_t i=0;i<roots.size();++i) {
            Real x=roots[i],u=max(q.l,min(q.r,alpha*x+beta));
            best=min(best,sqrt(sq(x-u)+sq(p.y(x)-q.y(u))));
        }
    }
    return best;
}
int main() {
    int n;if(!(cin>>n))return 0;
    Real sx,sy,tx,ty;cin>>sx>>sy>>tx>>ty;
    vector<Curve> p(n);
    for(int i=0;i<n;++i)cin>>p[i].a>>p[i].b>>p[i].c>>p[i].l>>p[i].r;
    vector<vector<Real> > g(n+2,vector<Real>(n+2,0));
    g[n][n+1]=g[n+1][n]=sqrt(sq(sx-tx)+sq(sy-ty));
    for(int i=0;i<n;++i) {
        g[n][i]=g[i][n]=pointDistance(sx,sy,p[i]);
        g[n+1][i]=g[i][n+1]=pointDistance(tx,ty,p[i]);
        for(int j=0;j<i;++j)g[i][j]=g[j][i]=curveDistance(p[i],p[j]);
    }
    vector<Real>d(n+2,1e40L);vector<bool> used(n+2,false);d[n]=0;
    for(int k=0;k<n+2;++k) {
        int v=-1;
        for(int i=0;i<n+2;++i)if(!used[i]&&(v<0||d[i]<d[v]))v=i;
        used[v]=true;
        for(int i=0;i<n+2;++i)d[i]=min(d[i],d[v]+g[v][i]);
    }
    cout<<fixed<<setprecision(10)<<d[n+1]<<'\n';
}
