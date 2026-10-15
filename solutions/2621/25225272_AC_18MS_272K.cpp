// POJ 2621 - Parallelepiped
// Model: gpt-6.1-sol
// Enumerate the two x faces. For each lower y face, sweep the upper
// y face downward, deleting projected obstacles from a linked z order.
// The largest adjacent z gap supplies the best empty rectangle.
// Only strictly interior points obstruct a box; outside points are ignored.
// Coordinates are exact hundredths and volume rounding uses integers.
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
struct Point { int x,y,z; };
int readcoord() {
    string s; cin >> s;
    int a=0,b=0,k=0; bool dot=false;
    for(size_t i=0;i<s.size();++i) {
        if(s[i]=='.') dot=true;
        else if(!dot) a=a*10+s[i]-'0';
        else { b=b*10+s[i]-'0'; ++k; }
    }
    while(k<2) { b*=10; ++k; }
    return a*100+b;
}
bool byz(const Point&a,const Point&b) { return a.z<b.z; }
bool byy(const pair<int,int>&a,const pair<int,int>&b) {return a.first>b.first;}
long long area(vector<Point> p,int v,int w) {
    sort(p.begin(),p.end(),byz);
    vector<int> lows(1,0);
    for(size_t i=0;i<p.size();++i) lows.push_back(p[i].y);
    long long best=0;
    for(size_t t=0;t<lows.size();++t) {
        int low=lows[t],z[54],pr[54],nx[54],m=0;
        vector<pair<int,int> > order;
        z[0]=0;
        for(size_t j=0;j<p.size();++j) if(p[j].y>low) {
            z[++m]=p[j].z; order.push_back(make_pair(p[j].y,m));
        }
        z[m+1]=w;
        int gap=0;
        for(int j=0;j<=m+1;++j) {pr[j]=j-1;nx[j]=j+1; if(j) gap=max(gap,z[j]-z[j-1]);}
        best=max(best,(long long)(v-low)*gap);
        sort(order.begin(),order.end(),byy);
        for(size_t j=0;j<order.size();) {
            int high=order[j].first;
            do {
                int q=order[j].second,a=pr[q],b=nx[q];
                nx[a]=b; pr[b]=a; gap=max(gap,z[b]-z[a]); ++j;
            } while(j<order.size() && order[j].first==high);
            best=max(best,(long long)(high-low)*gap);
        }
    }
    return best;
}
int main() {
    int u=readcoord(),v=readcoord(),w=readcoord(),n; cin>>n;
    vector<Point> p; vector<int> xs; xs.push_back(0);xs.push_back(u);
    for(int i=0;i<n;++i) {
        Point a; a.x=readcoord();a.y=readcoord();a.z=readcoord();
        if(a.x>0 && a.x<u && a.y>0 && a.y<v && a.z>0 && a.z<w) {p.push_back(a);xs.push_back(a.x);}
    }
    sort(xs.begin(),xs.end()); xs.erase(unique(xs.begin(),xs.end()),xs.end());
    long long best=0;
    for(size_t i=0;i<xs.size();++i) for(size_t j=i+1;j<xs.size();++j) {
        vector<Point> q;
        for(size_t k=0;k<p.size();++k) if(p[k].x>xs[i] && p[k].x<xs[j]) q.push_back(p[k]);
        best=max(best,(long long)(xs[j]-xs[i])*area(q,v,w));
    }
    long long cents=(best+5000)/10000;
    cout<<cents/100<<'.'<<(cents%100<10?"0":"")<<cents%100<<'\n';
    return 0;
}
