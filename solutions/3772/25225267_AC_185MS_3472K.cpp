// POJ 3772 - Baseball of Planet Pandora
// Model: gpt-6.1-sol
// Subset DP stores the best score for each used-player set, out count,
// and occupied-base mask. A third out or sacrifice ends play immediately;
// in particular, a third sacrifice must not advance or score any runner.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
static int dp[1<<15][3][8];
static int a[15][15], bits[1<<15];
int main() {
    int n;
    for (int m=1;m<(1<<15);++m) bits[m]=bits[m>>1]+(m&1);
    while (scanf("%d",&n)==1 && n) {
        for(int i=0;i<n;++i) for(int j=0;j<n;++j) scanf("%d",&a[i][j]);
        memset(dp,-1,sizeof(dp));
        dp[0][0][0]=0;
        int ans=0;
        for(int m=0;m<(1<<n);++m) {
            int turn=bits[m];
            for(int o=0;o<3;++o) for(int b=0;b<8;++b) {
                int score=dp[m][o][b];
                if(score<0) continue;
                ans=max(ans,score);
                if(turn==n) continue;
                for(int p=0;p<n;++p) if(!(m&(1<<p))) {
                    int r=a[p][turn], no=o, nb=b, ns=score;
                    if(r<=1) {
                        if(o==2) continue;
                        ++no;
                    }
                    if(r==1 || r==2) {
                        ns+=(b>>2)&1;
                        nb=((b<<1)&7)+(r==2);
                    } else if(r==3) {
                        ns+=bits[b]+1;
                        nb=0;
                    }
                    int &v=dp[m|(1<<p)][no][nb];
                    v=max(v,ns);
                }
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
