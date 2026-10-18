// POJ 3380 - Bridges
// Model: gpt-6-astra
// An edge separating s towns from n-s towns is used by s*(n-s) pairs.
// Rank length*s*(n-s) descending if carriages are faster, ascending otherwise.
// Bridges require carriages even when horses are faster; equal speeds allow any.
// Integer weights are at most 2.5e13, exactly representable in double.
// Iterative tree traversal avoids stack overflow. O(n log n) time, O(n) space.
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
struct Edge { int to, id; Edge(int t, int i):to(t),id(i){} };
int main() {
    ios::sync_with_stdio(false);
    int n,k,sh,sc;
    if (!(cin>>n>>k>>sh>>sc)) return 0;
    vector<vector<Edge> > adj(n);
    vector<int> len(n-1);
    for(int i=0;i<n-1;++i) {
        int a,b; cin>>a>>b>>len[i]; --a; --b;
        adj[a].push_back(Edge(b,i)); adj[b].push_back(Edge(a,i));
    }
    vector<int> parent(n,-1), pe(n,-1), order(1,0), sz(n,1);
    parent[0]=0;
    for(int i=0;i<n;++i) {
        int v=order[i];
        for(size_t j=0;j<adj[v].size();++j) {
            Edge e=adj[v][j];
            if(parent[e.to]!=-1) continue;
            parent[e.to]=v; pe[e.to]=e.id; order.push_back(e.to);
        }
    }
    vector<pair<double,int> > ranked;
    for(int i=n-1;i>0;--i) {
        int v=order[i];
        double weight=double(sz[v])*(n-sz[v])*len[pe[v]];
        if(sc>sh) weight=-weight;
        if(sc==sh) weight=0;
        ranked.push_back(make_pair(weight,pe[v]+1));
        sz[parent[v]]+=sz[v];
    }
    sort(ranked.begin(),ranked.end());
    vector<int> answer;
    for(int i=0;i<k;++i) answer.push_back(ranked[i].second);
    sort(answer.begin(),answer.end());
    for(int i=0;i<k;++i) cout<<(i?" ":"")<<answer[i];
    cout<<'\n';
    return 0;
}
