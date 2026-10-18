// POJ 2971 - "Switchback"
// Model: gpt-6-astra
// Memoized longest paths on (square, incoming direction, speed).
// Speed + height never increases; at equal energy every move descends,
// so the state graph is acyclic. The initial push permits only the first
// descent from speed zero; a later zero speed ends the trip.
#include <cstdio>
#include <cstring>
int n,m,h[50][50],dp[50][50][4][101];
signed char go[50][50][4][101];
int dr[4]={1,0,-1,0},dc[4]={0,1,0,-1};
const int BAD=-1000000;
int solve(int r,int c,int d,int v) {
    int &ans=dp[r][c][d][v];
    if(ans!=-1) return ans;
    ans=v<=3?0:BAD;
    go[r][c][d][v]=-1;
    if(v==0) return ans;
    for(int nd=0;nd<4;++nd) {
        if(nd==(d+2)%4) continue;
        int nr=r+dr[nd],nc=c+dc[nd];
        if(nr<0||nr>=n||nc<0||nc>=m) continue;
        int dh=h[nr][nc]-h[r][c];
        if(dh < -2 || dh > 1) continue;
        int nv=v+(dh<0?-dh:(dh==0?-1:-2));
        if(nv<0) continue;
        int cand=1+solve(nr,nc,nd,nv);
        if(cand>ans) {ans=cand; go[r][c][d][v]=(signed char)nd;}
    }
    return ans;
}
int main() {
    int a,b;
    while(std::scanf("%d%d%d%d",&n,&m,&a,&b)==4) {
        --a;--b;
        for(int i=0;i<n;++i) for(int j=0;j<m;++j) std::scanf("%d",&h[i][j]);
        std::memset(dp,-1,sizeof(dp));
        int best=BAD,first=-1;
        for(int d=0;d<4;++d) {
            int r=a+dr[d],c=b+dc[d];
            if(r<0||r>=n||c<0||c>=m) continue;
            int v=h[a][b]-h[r][c];
            if(v<1||v>2) continue;
            int cand=1+solve(r,c,d,v);
            if(cand>best) {best=cand;first=d;}
        }
        if(first<0) {std::puts("0");continue;}
        std::printf("%d\n",best);
        int r=a,c=b,d=first,v=0;
        for(int i=0;i<best;++i) {
            int nr=r+dr[d],nc=c+dc[d],dh=h[nr][nc]-h[r][c];
            v+=(dh<0?-dh:(dh==0?-1:-2));r=nr;c=nc;
            std::printf("%d %d\n",r+1,c+1);
            d=go[r][c][d][v];
        }
    }
    return 0;
}
