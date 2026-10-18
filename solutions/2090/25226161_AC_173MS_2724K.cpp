// POJ 2090 - Two-Stacks Solitaire
// Model: gpt-6-astra
// Read cards in reverse dealing order. Two cards must use different stacks
// when an earlier smaller card cannot leave before a later larger card is
// pushed: some still later card is smaller than both. Bipartite coloring
// of these conflicts assigns stacks; greedily pop the next sorted value.
// Comparisons are strict, allowing equal cards and arbitrary multiplicities.
// Time O(N^2), space O(N^2).
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int n, tc=0;
    while (scanf("%d", &n)==1 && n) {
        vector<int> a(n), target, low(n+1, 1000000), color(n,-1);
        for(int i=n-1;i>=0;--i) scanf("%d", &a[i]);
        target=a; sort(target.begin(),target.end());
        for(int i=n-1;i>=0;--i) low[i]=min(a[i],low[i+1]);
        vector<vector<int> > edge(n);
        for(int i=0;i<n;++i) for(int j=i+1;j<n;++j)
            if(a[i]<a[j] && low[j+1]<a[i]) {
                edge[i].push_back(j); edge[j].push_back(i);
            }
        bool ok=true;
        for(int r=0;r<n;++r) if(color[r]<0) {
            vector<int> q(1,r); color[r]=0;
            for(unsigned h=0;h<q.size();++h) {
                int u=q[h];
                for(unsigned k=0;k<edge[u].size();++k) {
                    int v=edge[u][k];
                    if(color[v]<0) {color[v]=1-color[u]; q.push_back(v);}
                    else if(color[v]==color[u]) ok=false;
                }
            }
        }
        printf("#%d\n",++tc);
        if(!ok) {puts("impossible"); continue;}
        vector<int> st[2]; int out=0;
        for(int i=0;i<=n;++i) {
            bool moved=true;
            while(moved && out<n) {
                moved=false;
                for(int s=0;s<2 && out<n;++s)
                    if(!st[s].empty() && st[s].back()==target[out]) {
                        st[s].pop_back(); ++out; moved=true;
                        printf("pop %d\n",s+1);
                    }
            }
            if(i<n) {
                st[color[i]].push_back(a[i]);
                printf("push %d\n",color[i]+1);
            }
        }
    }
    return 0;
}
