// POJ 1233 - Street Crossing
// Model: gpt-6.1-sol
// Simulate the cellular automaton and reachable positions in rolling layers.
// Figure 1 puts one-based odd rows to the left of even rows. Reaching the
// last row at time t finishes the crossing at t+1, as in the sample.
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;
int main() {
    int cases; scanf("%d", &cases);
    while (cases--) {
        int n,m; scanf("%d%d", &n,&m);
        vector<int> adj[100];
        for(int r=0;r<n;++r) for(int c=0;c<m;++c) {
            int u=r*m+c;
            for(int s=0;s<n;++s) for(int d=0;d<m;++d) {
                int dx=(2*c+r%2)-(2*d+s%2);
                if((r==s && (dx==2 || dx==-2)) ||
                   ((r-s==1 || s-r==1) && (dx==1 || dx==-1)))
                    adj[u].push_back(s*m+d);
            }
        }
        bool cold[100], reach[100], nc[100], nr[100];
        memset(reach,0,sizeof(reach));
        for(int r=0;r<n;++r) {
            char line[12]; scanf("%s",line);
            for(int c=0;c<m;++c) {
                cold[r*m+c]=line[c]=='C';
                if(r==0) reach[c]=cold[c];
            }
        }
        int answer=-1;
        for(int t=0;t<1000;++t) {
            for(int c=0;c<m;++c) if(reach[(n-1)*m+c]) answer=t+1;
            if(answer!=-1) break;
            for(int u=0;u<n*m;++u) {
                int count=0;
                for(unsigned k=0;k<adj[u].size();++k) count+=cold[adj[u][k]];
                nc[u]=count==3 || (cold[u] && count==2);
                bool possible=reach[u];
                for(unsigned k=0;k<adj[u].size();++k) possible=possible || reach[adj[u][k]];
                nr[u]=nc[u] && possible;
            }
            memcpy(cold,nc,sizeof(cold)); memcpy(reach,nr,sizeof(reach));
        }
        if(answer<0) puts("impossible"); else printf("%d\n",answer);
    }
    return 0;
}
