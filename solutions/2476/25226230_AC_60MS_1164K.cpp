// POJ 2476 - Game schedule required
// Model: gpt-6-astra
// The remaining games form a tree. A loser must be a leaf, so match
// leaves to unused neighbours using degrees frozen at the round's start.
// Leaves sharing a neighbour are interchangeable; the unmatched team,
// which may itself be a leaf, receives the single permitted wildcard.
// Remove all losers simultaneously and repeat. O(n log n) time and O(n) space.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace std;
int main() {
    int n;
    while (scanf("%d", &n) == 1 && n) {
        char name[1005][26], a[26], b[26];
        map<string,int> id;
        vector<int> adj[1005];
        int degree[1005] = {0};
        bool alive[1005];
        for (int i=0;i<n;++i) {
            scanf("%25s", name[i]); id[name[i]]=i; alive[i]=true;
        }
        for (int i=1;i<n;++i) {
            scanf("%25s%25s",a,b);
            int u=id[a], v=id[b];
            adj[u].push_back(v); adj[v].push_back(u);
            ++degree[u]; ++degree[v];
        }
        int left=n, round=0;
        while (left>1) {
            printf("Round #%d\n",++round);
            bool used[1005]={false}, lost[1005]={false};
            for (int u=0;u<n;++u) if (alive[u] && degree[u]==1 && !used[u]) {
                int v=-1;
                for (unsigned j=0;j<adj[u].size();++j)
                    if (alive[adj[u][j]]) { v=adj[u][j]; break; }
                if (v>=0 && !used[v]) {
                    printf("%s defeats %s\n", name[v],name[u]);
                    used[u]=used[v]=true; lost[u]=true;
                }
            }
            for (int u=0;u<n;++u) if (alive[u] && !used[u])
                printf("%s advances with wildcard\n",name[u]);
            for (int u=0;u<n;++u) if (lost[u]) {
                alive[u]=false; --left;
                for (unsigned j=0;j<adj[u].size();++j) --degree[adj[u][j]];
                degree[u]=0;
            }
        }
        for (int u=0;u<n;++u) if (alive[u]) printf("Winner: %s\n\n",name[u]);
    }
    return 0;
}
