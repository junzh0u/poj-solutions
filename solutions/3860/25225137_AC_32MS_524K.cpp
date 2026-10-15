// POJ 3860 - Fruit Weights
// Model: gpt-6.1-sol
// Take logarithms of positive weights and close the resulting difference
// constraints with Floyd-Warshall. A negative cycle is inconsistent; the
// two directed bounds determine which query comparisons are forced.
// Logarithms avoid underflow on long chains; EPS absorbs roundoff at equality.
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
using namespace std;
const double INF=1e50, EPS=1e-10;
double d[205][205];
int getid(map<string,int>& ids, const string& s) {
    if(ids.find(s)==ids.end()) { int z=ids.size(); ids[s]=z; }
    return ids[s];
}
int main() {
    int n;
    while(cin>>n && n) {
        map<string,int> ids;
        for(int i=0;i<205;++i) for(int j=0;j<205;++j) d[i][j]=i==j?0:INF;
        for(int i=0;i<n;++i) {
            int a,b; string x,y; cin>>a>>x>>b>>y;
            int u=getid(ids,x),v=getid(ids,y);
            d[v][u]=min(d[v][u],log(double(b)/a));
        }
        int a,b; string x,y; cin>>a>>x>>b>>y;
        int u=getid(ids,x),v=getid(ids,y),m=ids.size();
        for(int k=0;k<m;++k) for(int i=0;i<m;++i) if(d[i][k]<INF/2)
            for(int j=0;j<m;++j) if(d[k][j]<INF/2 && d[i][k]+d[k][j]<d[i][j]-EPS)
                d[i][j]=d[i][k]+d[k][j];
        bool bad=false;
        for(int i=0;i<m;++i) if(d[i][i]<-EPS) bad=true;
        if(bad) { cout<<"INCONSISTENT\n"; continue; }
        double q=log(double(b)/a);
        bool le=d[v][u]<=q+EPS,ge=d[u][v]<=-q+EPS;
        cout<<(le?(ge?"==":"<="):(ge?">=":"UNAVAILABLE"))<<'\n';
    }
}
