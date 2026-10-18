// POJ 2927 - Constructing Roads
// Model: gpt-6-astra
// Compute an MST containing the nearest-neighbour construction forest.
// Its unbuilt length is the fixed connecting cost plus each planned road's
// remaining length. Invert this continuous piecewise-linear function.
// Interpretation: additional construction completes the city-road network;
// geometrically rerouting links through off-axis moving sites would be a
// different optimization, not represented by this city-edge model.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;
struct City { string name; double x,y,v; };
struct Road { double len, rate; };
int main() {
    int n,kingdom=0;
    while(cin>>n && n) {
        vector<City> a(n);
        for(int i=0;i<n;++i) cin>>a[i].name>>a[i].x>>a[i].y>>a[i].v;
        vector<double> d(n*n,0), best(n,1e100);
        vector<int> to(n,-1),used(n,0);
        for(int i=0;i<n;++i) for(int j=0;j<i;++j) {
            double x=a[i].x-a[j].x,y=a[i].y-a[j].y;
            d[i*n+j]=d[j*n+i]=sqrt(x*x+y*y);
        }
        for(int i=0;i<n;++i) for(int j=0;j<n;++j) if(i!=j) {
            if(to[i]<0 || d[i*n+j]<d[i*n+to[i]] ||
               (d[i*n+j]==d[i*n+to[i]] && a[j].name<a[to[i]].name)) to[i]=j;
        }
        double fixed=0;
        best[0]=0;
        for(int k=0;k<n;++k) {
            int u=-1;
            for(int i=0;i<n;++i) if(!used[i] && (u<0 || best[i]<best[u])) u=i;
            used[u]=1; fixed+=best[u];
            for(int i=0;i<n;++i) if(!used[i]) best[i]=min(best[i],d[u*n+i]);
        }
        vector<Road> roads;
        double endtime=0;
        for(int i=0;i<n;++i) if(to[i]>=0) {
            int j=to[i];
            if(to[j]==i && j<i) continue;
            Road r; r.len=d[i*n+j]; r.rate=a[i].v+(to[j]==i?a[j].v:0);
            if(r.rate>0) { roads.push_back(r); fixed-=r.len; endtime=max(endtime,r.len/r.rate); }
        }
        fixed=max(0.0,fixed);
        printf("Kingdom %d\n",++kingdom);
        int type; double value;
        while(cin>>type && type) {
            cin>>value;
            if(type==1) {
                double ans=fixed;
                for(size_t i=0;i<roads.size();++i) ans+=max(0.0,roads[i].len-roads[i].rate*value);
                printf("%.3f feet left at time %.3f\n",ans,value);
            } else if(value+1e-8<fixed) puts("NEVER");
            else {
                double lo=0,hi=endtime;
                for(int k=0;k<80;++k) {
                    double mid=(lo+hi)/2,rem=fixed;
                    for(size_t i=0;i<roads.size();++i) rem+=max(0.0,roads[i].len-roads[i].rate*mid);
                    if(rem>value+1e-9) lo=mid; else hi=mid;
                }
                printf("%.3f hours before %.3f feet left\n",hi,value);
            }
        }
        puts("");
    }
    puts("End");
    return 0;
}
