// POJ 1933 - A Brief Gerrymander
// Model: gpt-6-astra
// Count occupied street strips in each avenue interval, then partition
// avenues by dynamic programming. Both outer boundaries are mandatory;
// strictly increasing transitions ensure distinct boundaries even for N=0.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int grid[101][101], cost[101][101], dp[101][101], pred[101][101];
int main() {
    int n;
    while (scanf("%d", &n)==1 && n!=-1) {
        memset(grid,0,sizeof(grid));
        for(int i=0,x,y;i<n;++i) { scanf("%d%d",&x,&y); grid[x][y]=1; }
        int s, streets[101], a;
        scanf("%d",&s);
        for(int i=0;i<s;++i) scanf("%d",&streets[i]);
        scanf("%d",&a);
        memset(cost,0,sizeof(cost));
        for(int k=0;k+1<s;++k) {
            int row[101]={0};
            for(int x=streets[k];x<streets[k+1];++x)
                for(int y=1;y<100;++y) row[y]|=grid[x][y];
            for(int l=1;l<100;++l) {
                int occupied=0;
                for(int r=l+1;r<=100;++r) {
                    occupied|=row[r-1]; cost[l][r]+=occupied;
                }
            }
        }
        for(int k=0;k<=100;++k) for(int r=0;r<=100;++r) dp[k][r]=-1000000;
        dp[1][1]=0;
        for(int k=2;k<=a;++k) for(int r=k;r<=100;++r)
            for(int l=k-1;l<r;++l) if(dp[k-1][l]+cost[l][r]>dp[k][r]) {
                dp[k][r]=dp[k-1][l]+cost[l][r]; pred[k][r]=l;
            }
        int ans[101]; ans[a-1]=100;
        for(int k=a;k>=2;--k) ans[k-2]=pred[k][ans[k-1]];
        printf("%d",a);
        for(int k=0;k<a;++k) printf(" %d",ans[k]);
        puts("");
    }
    return 0;
}
