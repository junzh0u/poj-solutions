// POJ 3809 - Twenty Questions
// Model: gpt-6.1-sol
// Ternary states record each feature as unknown, zero, or one.
// Minimize one plus the worse answer branch over informative questions.
// Questions are adaptive; a single remaining object needs no more questions.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int cnt[177147], dp[177147], pw[12];
int main() {
    pw[0]=1; for(int i=1;i<=11;++i) pw[i]=pw[i-1]*3;
    int m,n;
    while(scanf("%d%d",&m,&n)==2 && m) {
        int total=pw[m];
        memset(cnt,0,total*sizeof(int));
        for(int j=0;j<n;++j) {
            char s[20]; scanf("%s",s); int state=0;
            for(int i=0;i<m;++i) state+=(s[i]-'0'+1)*pw[i];
            cnt[state]=1;
        }
        for(int state=total-1;state>=0;--state) {
            int t=state, first=-1;
            for(int i=0;i<m;++i,t/=3) if(t%3==0) {first=i;break;}
            if(first>=0) cnt[state]=cnt[state+pw[first]]+cnt[state+2*pw[first]];
            if(cnt[state]<=1) {dp[state]=0;continue;}
            int lower=0; for(int k=1;k<cnt[state];k*=2) ++lower;
            int best=m; t=state;
            for(int i=0;i<m;++i,t/=3) if(t%3==0) {
                int a=state+pw[i], b=state+2*pw[i];
                if(cnt[a] && cnt[b]) {
                    best=min(best,1+max(dp[a],dp[b]));
                    if(best==lower) break;
                }
            }
            dp[state]=best;
        }
        printf("%d\n",dp[0]);
    }
    return 0;
}
