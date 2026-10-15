// POJ 2134 - Traffic Lights
// Model: gpt-6.1-sol
// Time-layer dynamic programming over position and speed. Each second moves
// at the old speed, then changes speed by at most one. Red lights forbid
// departure and strict intermediate crossings, but permit stopping there.
// Reaching L requires speed zero. The initial zero-speed second is counted;
// light colors apply to the departure and strict intermediate positions.
#include <cstdio>
#include <cstring>
struct Light { int g,r,c; char color; } a[101];
bool red(int p,int t) {
    if (!a[p].g) return false;
    int k=(t+a[p].c)%(a[p].g+a[p].r);
    return a[p].color=='G' ? k>=a[p].g : k<a[p].r;
}
bool dp[2][101][102];
int main() {
    int L,N;
    if (scanf("%d%d",&L,&N)!=2) return 0;
    for(int i=0;i<N;++i) {
        int p,g,r,c; char col;
        scanf("%d%d%d %c%d",&p,&g,&r,&col,&c);
        a[p].g=g; a[p].r=r; a[p].c=c; a[p].color=col;
    }
    dp[0][0][0]=true;
    for(int t=0;;++t) {
        int cur=t%2,nxt=1-cur;
        if(dp[cur][L][0]) { printf("%d\n",t); break; }
        memset(dp[nxt],0,sizeof(dp[nxt]));
        for(int p=0;p<=L;++p) for(int v=0;p+v<=L;++v) {
            if(!dp[cur][p][v]) continue;
            bool ok=true;
            for(int q=p;q<p+v;++q) if(red(q,t)) { ok=false; break; }
            if(!ok) continue;
            for(int w=v-1;w<=v+1;++w) if(w>=0 && w<=L)
                dp[nxt][p+v][w]=true;
        }
    }
    return 0;
}
