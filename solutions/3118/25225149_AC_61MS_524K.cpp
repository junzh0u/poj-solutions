// POJ 3118 - Critical Route
// Model: gpt-6.1-sol
// Reverse the roads and build a reachability tree from the capital.
// Only tree edges can be critical; remove each in turn and search again.
// Parallel one-way segments retain distinct identities and input order.
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;
struct Edge { int from, to; };
vector<Edge> edges;
vector<int> adj[100];
int search(int banned, vector<int>* tree) {
    bool seen[100] = {false};
    int q[100], head=0, tail=1;
    q[0]=0; seen[0]=true;
    while(head<tail) {
        int u=q[head++];
        for(int j=0;j<(int)adj[u].size();++j) {
            int id=adj[u][j], v=edges[id].from;
            if(id==banned || seen[v]) continue;
            seen[v]=true; q[tail++]=v;
            if(tree) tree->push_back(id);
        }
    }
    return tail;
}
int main() {
    int n,m;
    while(cin>>n>>m && (n || m)) {
        map<string,int> index;
        vector<string> names(n);
        for(int i=0;i<n;++i) { cin>>names[i]; index[names[i]]=i; adj[i].clear(); }
        edges.clear();
        for(int i=0;i<m;++i) {
            string a,b; cin>>a>>b;
            Edge e; e.from=index[a]; e.to=index[b];
            edges.push_back(e); adj[e.to].push_back(i);
        }
        vector<int> tree;
        search(-1,&tree);
        vector<bool> critical(m,false);
        for(int i=0;i<(int)tree.size();++i)
            if(search(tree[i],0)<n) critical[tree[i]]=true;
        bool any=false;
        for(int i=0;i<m;++i) if(critical[i]) {
            cout<<names[edges[i].from]<<' '<<names[edges[i].to]<<'\n'; any=true;
        }
        if(!any) cout<<"Nenhuma\n";
        cout<<'\n';
    }
    return 0;
}
