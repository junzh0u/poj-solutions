// POJ 3642 - Carpool
// Model: gpt-6.1-sol
// Floyd-Warshall gives road distances. Enumerate every ordered route of at
// most five errands, then partition the people into exactly ceil(n/5) cars
// by a minimax subset DP. Passing a landmark does not require a stop there.
#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;
typedef long long Int;
const Int INF = 4000000000000000000LL;
Int d[17][17], route[32768], memo[4][32768];
int n, cnt[32768];
void routes(int mask, int last, int size, Int value) {
    if (size) route[mask] = min(route[mask], value + d[last][n+1]);
    if (size == 5) return;
    for (int i=0; i<n; ++i) if (!(mask & (1<<i)))
        routes(mask | (1<<i), i+1, size+1, value+d[last][i+1]+5);
}
Int solve(int mask, int cars) {
    if (cars == 1) return route[mask];
    Int &ans = memo[cars][mask];
    if (ans != -1) return ans;
    ans = INF;
    int first = mask & -mask;
    for (int sub=mask; sub; sub=(sub-1)&mask) {
        if (!(sub & first) || cnt[sub]>5) continue;
        int remaining = cnt[mask] - cnt[sub];
        if (remaining < cars-1 || remaining > 5*(cars-1)) continue;
        if (route[sub] >= ans) continue;
        ans = min(ans, max(route[sub], solve(mask^sub,cars-1)));
    }
    return ans;
}
int main() {
    int m;
    while (cin >> n >> m) {
        for (int i=0;i<n+2;++i) for (int j=0;j<n+2;++j)
            d[i][j] = i==j ? 0 : INF/4;
        for (int i=0;i<m;++i) {
            int a,b; Int w; cin>>a>>b>>w;
            d[a][b]=d[b][a]=min(d[a][b],w);
        }
        for (int k=0;k<n+2;++k) for (int i=0;i<n+2;++i)
            for (int j=0;j<n+2;++j) d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
        int limit=1<<n;
        fill(route,route+limit,INF);
        cnt[0]=0;
        for (int mask=1;mask<limit;++mask) cnt[mask]=cnt[mask>>1]+(mask&1);
        memset(memo,-1,sizeof(memo));
        routes(0,0,0,0);
        cout<<solve(limit-1,(n+4)/5)<<'\n';
    }
    return 0;
}
