// POJ 2483 - Greatest Common Subtree
// Model: gpt-6.1-sol
// A top-bottom subtree retains the original root and is closed under parents.
// For each pair of roots, match their children with maximum total common size.
// Zero-weight dummy children allow unmatched branches; Hungarian solves the match.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
vector<int> a[51], b[51];
int memo[51][51];
int solve(int x,int y) {
    int &ans=memo[x][y];
    if(ans!=-1) return ans;
    int n=(int)a[x].size(),m=(int)b[y].size();
    if(!n || !m) return ans=1;
    int k=max(n,m),w[51][51];
    memset(w,0,sizeof(w));
    for(int i=1;i<=n;++i) for(int j=1;j<=m;++j)
        w[i][j]=solve(a[x][i-1],b[y][j-1]);
    int u[51]={0},v[51]={0},p[51]={0},way[51]={0};
    for(int i=1;i<=k;++i) {
        p[0]=i;
        int j0=0,dist[51]; bool used[51]={false};
        for(int j=1;j<=k;++j) dist[j]=1000000;
        do {
            used[j0]=true;
            int i0=p[j0],delta=1000000,j1=0;
            for(int j=1;j<=k;++j) if(!used[j]) {
                int cur=-w[i0][j]-u[i0]-v[j];
                if(cur<dist[j]) {dist[j]=cur;way[j]=j0;}
                if(dist[j]<delta) {delta=dist[j];j1=j;}
            }
            for(int j=0;j<=k;++j) {
                if(used[j]) {u[p[j]]+=delta;v[j]-=delta;}
                else if(j) dist[j]-=delta;
            }
            j0=j1;
        } while(p[j0]);
        do {int j1=way[j0];p[j0]=p[j1];j0=j1;} while(j0);
    }
    ans=1;
    for(int j=1;j<=k;++j) ans+=w[p[j]][j];
    return ans;
}
int main() {
    int n,m;
    while(scanf("%d%d",&n,&m)==2) {
        for(int i=0;i<=50;++i) {a[i].clear();b[i].clear();}
        for(int i=1,x,y;i<n;++i) {scanf("%d%d",&x,&y);a[y].push_back(x);}
        for(int i=1,x,y;i<m;++i) {scanf("%d%d",&x,&y);b[y].push_back(x);}
        memset(memo,-1,sizeof(memo));
        printf("%d\n",solve(1,1));
    }
    return 0;
}
