// POJ 2990 - Board Game
// Model: gpt-6.1-sol
// Undirected vertex geography on unordered pairs of squares. Positions with
// adjacent pieces are immediate wins, so moving into one is suicidal and can
// be omitted. The remaining graph is bipartite by coordinate-sum parity.
// The first player wins exactly when every maximum matching covers the start:
// match without it, then test whether adding it permits an augmenting path.
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;
vector<int> edges[4096];
int mate[4096], seen[4096], stamp;
bool augment(int u) {
    if (seen[u] == stamp) return false;
    seen[u] = stamp;
    for (unsigned i=0; i<edges[u].size(); ++i) {
        int v=edges[u][i];
        if (mate[v]<0 || augment(mate[v])) {
            mate[v]=u;
            return true;
        }
    }
    return false;
}
int main() {
    int m,n;
    while(cin>>m>>n) {
        string b[8]; int id[8][8], x[64],y[64],p[2],pc=0,k=0;
        memset(id,-1,sizeof(id));
        for(int r=0;r<m;++r) {
            cin>>b[r];
            for(int c=0;c<n;++c) if(b[r][c]!='#') {
                id[r][c]=k; x[k]=r;y[k]=c;
                if(b[r][c]=='P') p[pc++]=k;
                ++k;
            }
        }
        if(abs(x[p[0]]-x[p[1]])+abs(y[p[0]]-y[p[1]])==1) {
            cout<<"Alice wins.\n"; continue;
        }
        int state[64][64],a[2016],d[2016],color[2016],cnt=0;
        memset(state,-1,sizeof(state));
        for(int u=0;u<k;++u) for(int v=u+1;v<k;++v)
            if(abs(x[u]-x[v])+abs(y[u]-y[v])!=1) {
                state[u][v]=state[v][u]=cnt;
                a[cnt]=u;d[cnt]=v;
                color[cnt]=(x[u]+y[u]+x[v]+y[v])%2;
                edges[cnt].clear();++cnt;
            }
        int start=state[p[0]][p[1]], side=color[start];
        const int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
        for(int s=0;s<cnt;++s) if(color[s]==side)
            for(int t=0;t<2;++t) for(int z=0;z<4;++z) {
                int u=t?a[s]:d[s],v=t?d[s]:a[s];
                int r=x[u]+dx[z],c=y[u]+dy[z];
                if(r<0||r>=m||c<0||c>=n||id[r][c]<0) continue;
                int q=state[id[r][c]][v];
                if(q>=0) edges[s].push_back(q);
            }
        memset(mate,-1,sizeof(mate));memset(seen,0,sizeof(seen));stamp=0;
        for(int s=0;s<cnt;++s) if(s!=start&&color[s]==side) {
            ++stamp;augment(s);
        }
        ++stamp;
        cout<<(augment(start)?"Alice wins.\n":"Bob wins.\n");
    }
}
