// POJ 3791 - Text Messaging Improvement?
// Model: gpt-6-astra
// Partition alphabetical letters into exactly K nonempty blocks of length <= 8.
// DP over prefixes and key counts; backtrack the chosen block boundaries.
// Frequencies are percentages: divide by 100 even when their sum is not 100.
#include <cstdio>
#include <string>
using namespace std;
int main() {
    int tests; if (scanf("%d", &tests)!=1) return 0;
    for (int tc=1; tc<=tests; ++tc) {
        int k; scanf("%d", &k);
        double f[26], cost[27][27], dp[27][27]; int prev[27][27];
        for (int i=0;i<26;++i) scanf("%lf", &f[i]);
        for (int i=0;i<26;++i) {
            double sum=0;
            for (int j=i;j<26 && j<i+8;++j) {
                sum+=(j-i+1)*f[j]; cost[i][j+1]=sum;
            }
        }
        for (int a=0;a<=k;++a) for (int n=0;n<=26;++n) dp[a][n]=1e100;
        dp[0][0]=0;
        for (int a=1;a<=k;++a) for (int n=a;n<=26;++n)
            for (int len=1;len<=8 && len<=n;++len) {
                double val=dp[a-1][n-len]+cost[n-len][n];
                if (val<dp[a][n]) { dp[a][n]=val; prev[a][n]=n-len; }
            }
        bool cut[27]={false}; int n=26;
        for (int a=k;a>0;--a) { n=prev[a][n]; cut[n]=true; }
        printf("%d %.3f ",tc,dp[k][26]/100.0);
        for (int i=0;i<26;++i) { if(i && cut[i]) putchar(' '); putchar('A'+i); }
        putchar('\n');
    }
    return 0;
}
