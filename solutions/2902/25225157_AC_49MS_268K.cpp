// POJ 2902 - Intercepting Missiles
// Model: gpt-6.1-sol
// Sweep aircraft by height for each missile, maintaining the union of launch
// times blocked by lower aircraft. Match missiles to reachable bombers.
// Aircraft extend one unit behind their heads; touching either endpoint hits.
// Scale times by vm*vn*vk so all closed-interval comparisons are exact.
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
typedef long long Int;
struct Object { int x,y,id,v; };
bool byHeight(const Object &a,const Object &b) { return a.y<b.y; }
vector<int> edges[300];
int mate[300]; bool seen[300];
bool augment(int u) {
    for (int i=0;i<(int)edges[u].size();++i) {
        int v=edges[u][i];
        if (seen[v]) continue;
        seen[v]=true;
        if (mate[v]<0 || augment(mate[v])) { mate[v]=u; return true; }
    }
    return false;
}
int main() {
    int tests; cin>>tests;
    for (int tc=1;tc<=tests;++tc) {
        int m,n,k,vm,vn,vk; cin>>m>>n>>k>>vm>>vn>>vk;
        vector<Object> a(m+n);
        for (int i=0;i<m+n;++i) {
            cin>>a[i].x>>a[i].y;
            a[i].id=i<m?i:-1; a[i].v=i<m?vm:vn;
        }
        sort(a.begin(),a.end(),byHeight);
        for (int j=0;j<k;++j) {
            int x; cin>>x; edges[j].clear();
            map<Int,Int> blocked;
            for (int i=0;i<m+n;++i) {
                Int width=Int(vm)*vn/a[i].v*vk;
                Int l=Int(x-a[i].x)*width-Int(a[i].y)*vm*vn;
                Int r=l+width;
                if (r<0) continue;
                l=max(l,Int(0));
                map<Int,Int>::iterator it=blocked.upper_bound(l);
                bool covered=false;
                if (it!=blocked.begin()) {
                    --it; covered=it->second>=r;
                }
                if (a[i].id>=0 && !covered) edges[j].push_back(a[i].id);
                it=blocked.lower_bound(l);
                if (it!=blocked.begin()) {
                    map<Int,Int>::iterator prev=it; --prev;
                    if (prev->second>=l) it=prev;
                }
                while (it!=blocked.end() && it->first<=r) {
                    l=min(l,it->first); r=max(r,it->second);
                    map<Int,Int>::iterator old=it++; blocked.erase(old);
                }
                blocked[l]=r;
            }
        }
        fill(mate,mate+m,-1);
        int answer=0;
        for (int j=0;j<k;++j) {
            fill(seen,seen+m,false);
            if (augment(j)) ++answer;
        }
        cout<<"Mission #"<<tc<<": "<<answer<<" bomber(s) exploded\n";
    }
}
