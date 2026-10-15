// POJ 2865 - ICPC Strikes Again
// Model: gpt-6.1-sol
// Topological propagation records employees working on prerequisite tasks.
// Reverse topological sums compute significance, counting each direct edge.
// Only tasks with no prerequisite performed by the same employee pay salary.
// Arbitrary precision avoids an unstated bound on DAG path multiplicities.
#include <cstdio>
#include <vector>
#include <queue>
#include <bitset>
using namespace std;
struct Big {
    vector<int> a;
    Big(int x=0) { a.push_back(x); }
    void add(const Big &b) {
        int carry=0;
        if(a.size()<b.a.size()) a.resize(b.a.size(),0);
        for(size_t i=0;i<a.size();++i) {
            int s=a[i]+carry+(i<b.a.size()?b.a[i]:0);
            carry=s>=1000000000;
            a[i]=s-(carry?1000000000:0);
        }
        if(carry) a.push_back(carry);
    }
    void print() const {
        printf("%d",a.back());
        for(int i=(int)a.size()-2;i>=0;--i) printf("%09d",a[i]);
    }
};
int main() {
    int n,e;
    while(scanf("%d%d",&n,&e)==2 && (n||e)) {
        vector<vector<int> > adj(n);
        vector<int> indeg(n,0), order;
        vector<Big> value(n), salary(e);
        vector<bitset<1000> > workers(n), above(n);
        for(int i=0;i<n;++i) {
            int b,d,k,x; scanf("%d%d%d",&b,&d,&k); value[i]=Big(b);
            while(d--) { scanf("%d",&x); --x; adj[i].push_back(x); ++indeg[x]; }
            while(k--) { scanf("%d",&x); workers[i].set(x-1); }
        }
        queue<int> q;
        for(int i=0;i<n;++i) if(!indeg[i]) q.push(i);
        while(!q.empty()) {
            int u=q.front(); q.pop(); order.push_back(u);
            bitset<1000> all=above[u]|workers[u];
            for(size_t j=0;j<adj[u].size();++j) {
                int v=adj[u][j]; above[v]|=all;
                if(--indeg[v]==0) q.push(v);
            }
        }
        for(int i=n-1;i>=0;--i) {
            int u=order[i];
            for(size_t j=0;j<adj[u].size();++j) value[u].add(value[adj[u][j]]);
            for(int w=0;w<e;++w)
                if(workers[u][w] && !above[u][w]) salary[w].add(value[u]);
        }
        puts("*****");
        for(int w=0;w<e;++w) { printf("%d ",w+1); salary[w].print(); puts(""); }
    }
    return 0;
}
