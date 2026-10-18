// POJ 2690 - Yahtzee
// Model: gpt-6-astra
// Assign rounds to categories in order with subset dynamic programming.
// Add the upper-section bonus immediately after category six; for a fixed
// subset a larger upper subtotal dominates every smaller one.
// Five equal dice also satisfy the stated full-house condition: three equal
// dice and the remaining two equal dice, with no distinctness requirement.
#include <cstdio>
#include <algorithm>
using namespace std;
int score[13][13], dp[8192], last[8192], bits[8192];
int main() {
    for (int m=1;m<8192;++m) bits[m]=bits[m>>1]+(m&1);
    int dice[5];
    while (scanf("%d",&dice[0])==1) {
        for(int r=0;r<13;++r) {
            for(int j=(r==0?1:0);j<5;++j)
                if(scanf("%d",&dice[j])!=1) return 0;
            int c[7]={0}, sum=0, mx=0, mask=0;
            for(int j=0;j<5;++j) { ++c[dice[j]]; sum+=dice[j]; mask|=1<<(dice[j]-1); }
            for(int j=1;j<=6;++j) { score[r][j-1]=j*c[j]; mx=max(mx,c[j]); }
            score[r][6]=sum;
            score[r][7]=mx>=3?sum:0;
            score[r][8]=mx>=4?sum:0;
            score[r][9]=mx==5?50:0;
            score[r][10]=((mask&15)==15||(mask&30)==30||(mask&60)==60)?25:0;
            score[r][11]=(mask==31||mask==62)?35:0;
            bool pair=false, triple=false;
            for(int j=1;j<=6;++j) { if(c[j]==2) pair=true; if(c[j]==3) triple=true; }
            score[r][12]=(mx==5||(pair&&triple))?40:0;
        }
        fill(dp,dp+8192,-1); dp[0]=0;
        for(int m=1;m<8192;++m) {
            int k=bits[m]-1;
            for(int r=0;r<13;++r) if(m&(1<<r)) {
                int val=dp[m^(1<<r)]+score[r][k];
                if(k==5&&val>=63) val+=35;
                if(val>dp[m]) { dp[m]=val; last[m]=r; }
            }
        }
        int ans[13], m=8191, upper=0;
        for(int k=12;k>=0;--k) { int r=last[m]; ans[k]=score[r][k]; m^=1<<r; }
        for(int k=0;k<13;++k) { printf("%d ",ans[k]); if(k<6) upper+=ans[k]; }
        printf("%d %d\n",upper>=63?35:0,dp[8191]);
    }
    return 0;
}
