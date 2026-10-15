// POJ 1689 - 3002 Rubbery
// Model: gpt-6.1-sol
// Compress all vertex coordinates. Even-odd scanlines mark obstacle cells,
// independently of polygon orientation (the judge uses both orientations).
// A free cell is reachable from its upper or right neighbor. Point contacts
// are not passages, since a beam cannot touch two obstacle sides at once.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Point { int x,y; };
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int t; cin>>t;
    while(t--) {
        int w,h,n; cin>>w>>h>>n;
        vector<vector<Point> > p(n);
        vector<int> xs,ys; xs.push_back(0); xs.push_back(w);
        ys.push_back(0); ys.push_back(h);
        for(int k=0;k<n;++k) {
            int m; cin>>m; p[k].resize(m);
            for(int j=0;j<m;++j) {
                cin>>p[k][j].x>>p[k][j].y;
                xs.push_back(p[k][j].x); ys.push_back(p[k][j].y);
            }
        }
        sort(xs.begin(),xs.end()); xs.erase(unique(xs.begin(),xs.end()),xs.end());
        sort(ys.begin(),ys.end()); ys.erase(unique(ys.begin(),ys.end()),ys.end());
        int nx=xs.size(),ny=ys.size();
        vector<unsigned char> a(nx*ny,0);
        for(int k=0;k<n;++k) for(int j=0;j<(int)p[k].size();++j) {
            Point u=p[k][j],v=p[k][(j+1)%p[k].size()];
            if(u.x!=v.x) continue;
            int x=lower_bound(xs.begin(),xs.end(),u.x)-xs.begin();
            int lo=lower_bound(ys.begin(),ys.end(),min(u.y,v.y))-ys.begin();
            int hi=lower_bound(ys.begin(),ys.end(),max(u.y,v.y))-ys.begin();
            for(int y=lo;y<hi;++y) a[y*nx+x]^=1;
        }
        for(int y=0;y<ny-1;++y) {
            int inside=0;
            for(int x=0;x<nx-1;++x) {
                inside^=a[y*nx+x]; a[y*nx+x]=inside;
            }
        }
        int answer=0;
        for(int y=0;y<ny-1;++y) for(int x=nx-2;x>=0;--x) {
            int at=y*nx+x;
            if(a[at]) continue;
            if((y==0 && x==nx-2) || (y>0 && a[at-nx]==2) ||
               (x<nx-2 && a[at+1]==2)) a[at]=2;
            else answer+=(xs[x+1]-xs[x])*(ys[y+1]-ys[y]);
        }
        cout<<answer<<'\n';
    }
}
