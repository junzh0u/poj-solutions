// POJ 1983 - Name the Crossing
// Model: gpt-6.1-sol
// Color each crossing component to infer street orientations. Add equality
// arcs using only the original crossings, then close the strength relation.
// Directions in different components cannot be inferred relative to each other.
#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <queue>
using namespace std;
bool edge[205][205], reach[205][205];
int color[205], comp[205];
map<string,int> ids;
int id(const string &s) {
    map<string,int>::iterator it=ids.find(s);
    if(it!=ids.end()) return it->second;
    int v=ids.size(); ids[s]=v; return v;
}
void split(const string &s,string &a,string &b) {
    string::size_type p=s.find('-'); a=s.substr(0,p); b=s.substr(p+1);
}
int main() {
    int n;
    while(cin>>n && n) {
        ids.clear(); memset(edge,0,sizeof(edge));
        string s,a,b;
        for(int i=0;i<n;++i) {cin>>s;split(s,a,b);int u=id(a),v=id(b);edge[u][v]=true;}
        int k=ids.size();
        memset(color,-1,sizeof(color));
        for(int root=0;root<k;++root) if(color[root]<0) {
            queue<int> q;q.push(root);color[root]=0;comp[root]=root;
            while(!q.empty()) {int u=q.front();q.pop();
                for(int v=0;v<k;++v) if((edge[u][v]||edge[v][u])&&color[v]<0) {
                    color[v]=1-color[u];comp[v]=root;q.push(v);
                }
            }
        }
        memcpy(reach,edge,sizeof(edge));
        for(int u=0;u<k;++u) for(int v=u+1;v<k;++v) {
            bool common=false,between=false;
            for(int w=0;w<k;++w) {
                if((edge[u][w]||edge[w][u])&&(edge[v][w]||edge[w][v])) common=true;
                if((edge[u][w]&&edge[w][v])||(edge[v][w]&&edge[w][u])) between=true;
            }
            if(common&&!between) reach[u][v]=reach[v][u]=true;
        }
        for(int w=0;w<k;++w) for(int u=0;u<k;++u) if(reach[u][w])
            for(int v=0;v<k;++v) if(reach[w][v]) reach[u][v]=true;
        cout<<k<<'\n'; int m;cin>>m;
        while(m--) {cin>>s;split(s,a,b);
            map<string,int>::iterator x=ids.find(a),y=ids.find(b);
            bool yes=false;
            if(x!=ids.end()&&y!=ids.end()) {
                int u=x->second,v=y->second;
                yes=comp[u]==comp[v]&&color[u]!=color[v]&&reach[u][v];
            }
            cout<<(yes?"YES":"NO")<<'\n';
        }
    }
}
