// POJ 1982 - Water Tank
// Model: gpt-6.1-sol
// Jump between overflow events, routing flow over the lower boundary of
// a full basin. Merge adjacent basins when both reach their shared board.
// Flow is volume per second; every basin has depth 30 cm.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Basin { int l,r; double h,flow; Basin(int a,int b,double c,double d):l(a),r(b),h(c),flow(d){} };
int x[12],wall[12],n;
double initial[12];
double solve(int p,double remain) {
    vector<Basin> v;
    for(int i=0;i<=n;++i) v.push_back(Basin(i,i,0,initial[i]));
    for(int iter=0;iter<100;++iter) {
        for(int i=0;i+1<(int)v.size();) {
            double w=wall[v[i].r+1];
            if(v[i].h>=w-1e-8 && v[i+1].h>=w-1e-8) {
                v[i].r=v[i+1].r; v[i].h=max(v[i].h,v[i+1].h);
                v[i].flow+=v[i+1].flow; v.erase(v.begin()+i+1);
                if(i) --i;
            } else ++i;
        }
        int s=v.size(); double rate[12]={0},ceiling[12];
        for(int i=0;i<s;++i) ceiling[i]=min(wall[v[i].l],wall[v[i].r+1]);
        for(int i=0;i<s;++i) {
            int j=i;
            while(v[j].h>=ceiling[j]-1e-8) {
                if(s==1) { j=-1; break; }
                if(wall[v[j].l]<wall[v[j].r+1]) --j; else ++j;
            }
            if(j>=0) rate[j]+=v[i].flow;
        }
        double dt=remain;
        for(int i=0;i<s;++i) if(rate[i]>0)
            dt=min(dt,(ceiling[i]-v[i].h)*(x[v[i].r+1]-x[v[i].l])*30.0/rate[i]);
        for(int i=0;i<s;++i) v[i].h+=dt*rate[i]/((x[v[i].r+1]-x[v[i].l])*30.0);
        remain-=dt;
        if(remain<1e-8 || dt==0) {
            for(int i=0;i<s;++i) if(x[v[i].l]<p && p<x[v[i].r+1]) return v[i].h;
        }
    }
    return 50;
}
int main() {
    int d; scanf("%d",&d);
    while(d--) {
        scanf("%d",&n); x[0]=0; wall[0]=50; x[n+1]=100; wall[n+1]=50;
        for(int i=1;i<=n;++i) scanf("%d%d",x+i,wall+i);
        fill(initial,initial+12,0.0);
        int m; scanf("%d",&m);
        while(m--) { int f,a; scanf("%d%d",&f,&a); int i=0; while(x[i+1]<f) ++i; initial[i]+=a; }
        int l; scanf("%d",&l);
        while(l--) { int p,t; scanf("%d%d",&p,&t); printf("%.3f\n",solve(p,t)); }
    }
    return 0;
}
