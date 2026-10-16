// POJ 2120 - Herbalists
// Model: gpt-6.1-sol
// Recognize outerplanarity by eliminating vertices of degree at most two.
// A replacement edge records one forced outer path, or a closed outer cycle.
// Closing another cycle through a saturated edge would trap a vertex inside.
// All graph and reduction state is rebuilt for each input instance.
#include <cstdio>
#include <map>
#include <vector>
#include <queue>
using namespace std;
bool possible(vector< map<int,int> > &g) {
    int n = (int)g.size(), remaining = n;
    queue<int> q;
    vector<bool> gone(n,false);
    for(int i=0;i<n;++i) if(g[i].size()<=2) q.push(i);
    while(!q.empty()) {
        int v=q.front(); q.pop();
        if(gone[v] || g[v].size()>2) continue;
        if(g[v].size()==2) {
            map<int,int>::iterator it=g[v].begin();
            int a=it->first, x=it->second; ++it;
            int b=it->first, y=it->second;
            map<int,int>::iterator f=g[a].find(b);
            int z=(f==g[a].end() ? -1 : f->second);
            if(z>=0 && (x==2 || y==2 || z==2)) return false;
            int mark=(x==2 || y==2 || z==1) ? 2 : 1;
            g[a][b]=g[b][a]=mark;
        }
        for(map<int,int>::iterator it=g[v].begin();it!=g[v].end();++it) {
            int u=it->first; g[u].erase(v);
            if(g[u].size()<=2) q.push(u);
        }
        g[v].clear(); gone[v]=true; --remaining;
    }
    return remaining==0;
}
int main() {
    int n,m;
    while(scanf("%d%d",&n,&m)==2 && n) {
        vector< map<int,int> > g(n);
        for(int i=0;i<m;++i) {
            int a,b; scanf("%d%d",&a,&b);
            if(a!=b) g[a][b]=g[b][a]=0;
        }
        puts(possible(g) ? "Yes, village can be built" : "No, village cannot be built");
    }
    return 0;
}
