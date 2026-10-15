// POJ 3351 - Gerrymandering
// Model: gpt-6.1-sol
// Precompute strictly winning intervals. For each district count, maximize
// wins minus losses over prefix partitions. Previous balances have one parity,
// so a nonmaximum balance cannot beat maximum-1, even with a winning interval.
// Bitsets test whether any maximum-balance split ends in a winning district.
// A merger removes one boundary; ties count as losses, not wins.
#include <cstdio>
#include <cstring>
const int N = 1005, W = 32;
int votes[N][10], a[N], b[N];
unsigned int win[N][W];
int main() {
    int n,p;
    if (scanf("%d%d", &n,&p)!=2) return 0;
    for (int i=0;i<n;++i) for(int k=0;k<p;++k) scanf("%d",&votes[i][k]);
    for (int j=0;j<n;++j) {
        int sum[10]={0};
        for (int i=j+1;i<=n;++i) {
            for(int k=0;k<p;++k) sum[k]+=votes[i-1][k];
            bool good=true;
            for(int k=1;k<p;++k) if(sum[0]<=sum[k]) good=false;
            if(good) win[i][j/32] |= 1U<<(j%32);
        }
    }
    for(int i=0;i<=n;++i) a[i]=-10000;
    a[0]=0;
    int answer=-1;
    for(int q=1;q<=n;++q) {
        unsigned int best[W];
        memset(best,0,sizeof(best));
        int maximum=-10000;
        for(int i=0;i<q;++i) b[i]=-10000;
        for(int i=q;i<=n;++i) {
            int j=i-1;
            if(a[j]>maximum) {
                maximum=a[j];
                memset(best,0,sizeof(best));
            }
            if(a[j]==maximum) best[j/32] |= 1U<<(j%32);
            bool good=false;
            for(int k=(q-1)/32;k<=(i-1)/32;++k)
                if(best[k]&win[i][k]) { good=true; break; }
            b[i]=maximum+(good?1:-1);
        }
        if(b[n]>0) answer=n-q;
        for(int i=0;i<=n;++i) a[i]=b[i];
    }
    printf("%d\n",answer);
    return 0;
}
