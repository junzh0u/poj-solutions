// POJ 4017 - Galaxy Interconnection
// Model: gpt-6.1-sol
// Color the initial cycle distinctly, then extend by depth-first search.
// Choose an unused color immediately preceding a used neighbor color.
// Every vertex has a cyclic successor, hence a rainbow walk of k vertices.
// POJ's statement is empty; recovered from NEERC Northern 2011 problem G.
// Match jury galaxy_aa's input-order traversal and sorted-color tie break.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
vector<vector<int> > g;
vector<int> color;
int k;
void dfs(int x) {
    vector<int> used;
    for (size_t i=0;i<g[x].size();++i)
        if(color[g[x][i]]>=0) used.push_back(color[g[x][i]]);
    sort(used.begin(),used.end());
    for(size_t i=0;i<used.size();++i) {
        if(i==0) {
            if(used[i]>0) {color[x]=used[i]-1;break;}
            if(used.back()<k-1) {color[x]=k-1;break;}
        } else if(used[i]-used[i-1]>1) {color[x]=used[i]-1;break;}
    }
    for(size_t i=0;i<g[x].size();++i)
        if(color[g[x][i]]<0) dfs(g[x][i]);
}
int main() {
    int n,m;
    if(scanf("%d%d%d",&n,&k,&m)!=3) return 0;
    g.assign(n,vector<int>()); color.assign(n,-1);
    for(int i=0;i<m;++i) {int a,b;scanf("%d%d",&a,&b);--a;--b;g[a].push_back(b);g[b].push_back(a);}
    for(int i=0;i<k;++i) color[i]=i;
    for(int i=0;i<k;++i)
        for(size_t j=0;j<g[i].size();++j)
            if(color[g[i][j]]<0) dfs(g[i][j]);
    for(int i=0;i<n;++i) printf("%s%d",i?" ":"",color[i]+1);
    puts("");return 0;
}
