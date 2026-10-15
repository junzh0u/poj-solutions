// POJ 3846 - Mountain Road
// Model: gpt-6.1-sol
// Dynamic programming over the two queue prefixes and the last direction.
// Extend a maximal-direction batch with earliest entry and exit times; the
// ten-second gap applies at both ends, but resets after opposite traffic.
#include <cstdio>
#include <algorithm>
using namespace std;
int t[2][205], d[2][205], dp[205][205][2];
int main() {
    int cases; scanf("%d", &cases);
    while (cases--) {
        int n, cnt[2]={0,0}; scanf("%d", &n);
        for (int z=0;z<n;++z) {
            char s[4]; int a,b; scanf("%s%d%d",s,&a,&b);
            int q=s[0]-'A', k=++cnt[q]; t[q][k]=a; d[q][k]=b;
        }
        const int INF=1000000000;
        for(int i=0;i<=cnt[0];++i) for(int j=0;j<=cnt[1];++j)
            dp[i][j][0]=dp[i][j][1]=INF;
        dp[0][0][0]=dp[0][0][1]=0;
        for(int i=0;i<=cnt[0];++i) for(int j=0;j<=cnt[1];++j) {
            if(dp[i][j][1]<INF) {
                int entry=dp[i][j][1], leave=entry;
                for(int k=i+1;k<=cnt[0];++k) {
                    entry=max(entry,t[0][k]);
                    leave=max(leave,entry+d[0][k]);
                    dp[k][j][0]=min(dp[k][j][0],leave);
                    entry+=10; leave+=10;
                }
            }
            if(dp[i][j][0]<INF) {
                int entry=dp[i][j][0], leave=entry;
                for(int k=j+1;k<=cnt[1];++k) {
                    entry=max(entry,t[1][k]);
                    leave=max(leave,entry+d[1][k]);
                    dp[i][k][1]=min(dp[i][k][1],leave);
                    entry+=10; leave+=10;
                }
            }
        }
        printf("%d\n",min(dp[cnt[0]][cnt[1]][0],dp[cnt[0]][cnt[1]][1]));
    }
}
