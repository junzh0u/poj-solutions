// POJ 3848 - Room Assignments
// Model: gpt-6.1-sol
// Treat guests as edges between rooms. Every component must be a tree or
// unicyclic, leaving exactly one tree component with one vacant room.
// Each vertex of that tree is vacant in equally many guest assignments.
// Two tree choices therefore average their ratings; an outside choice is
// impossible and forces the other, tree choice. Apply lexicographic ties.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
struct DSU {
    vector<int> p, sz, e;
    DSU(int n):p(n+1),sz(n+1,1),e(n+1,0) {for(int i=1;i<=n;++i)p[i]=i;}
    int root(int x) {if(p[x]!=x)p[x]=root(p[x]);return p[x];}
    void add(int a,int b) {
        a=root(a);b=root(b);
        if(a==b){++e[a];return;}
        if(sz[a]<sz[b])swap(a,b);
        p[b]=a;sz[a]+=sz[b];e[a]+=e[b]+1;
    }
};
int main() {
    int cases; if(scanf("%d",&cases)!=1)return 0;
    while(cases--) {
        int n;scanf("%d",&n);DSU d(n);
        for(int i=1;i<n;++i){int a,b;scanf("%d%d",&a,&b);d.add(a,b);}
        vector<int> v(n+1);for(int i=1;i<=n;++i)scanf("%d",&v[i]);
        bool ok=true;int tree=0;
        for(int i=1;i<=n;++i)if(d.root(i)==i){
            if(d.e[i]>d.sz[i])ok=false;
            if(d.e[i]==d.sz[i]-1)tree=i;
        }
        if(!ok){puts("impossible");continue;}
        vector<pair<int,int> > rooms;
        int outside=0;
        for(int i=1;i<=n;++i)if(d.root(i)==tree)rooms.push_back(make_pair(-v[i],i));
        else if(!outside)outside=i;
        sort(rooms.begin(),rooms.end());
        int a,b;
        if(!outside){a=rooms[0].second;b=rooms[1].second;}
        else {
            a=rooms[0].second;b=outside;
            if(rooms.size()>1 && rooms[1].first==rooms[0].first)b=min(b,rooms[1].second);
        }
        if(a>b)swap(a,b);
        printf("%d %d\n",a,b);
    }
    return 0;
}
