// POJ 2884 - ACM-Telecom
// Model: gpt-6.1-sol
// Trie dynamic programming minimizes retained prefix rules for each inherited
// rate. Input order is irrelevant: longer prefixes override shorter ones.
// Missing branches retain their original inherited rate; the discussion's
// separating cases require preserving these gaps rather than inventing rules.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int INF=1000000;
int ch[4010][10],rate[4010],dp[4010][101],nodes;
void solve(int v,int old){
    if(rate[v]) old=rate[v];
    int sum[101]={0};
    for(int d=0;d<10;++d){
        int u=ch[v][d];
        if(u){solve(u,old);for(int c=0;c<=100;++c)sum[c]+=dp[u][c];}
        else if(old){for(int c=0;c<=100;++c)if(c!=old)sum[c]+=INF;}
    }
    int best=INF;
    for(int c=1;c<=100;++c)best=min(best,1+sum[c]);
    for(int c=0;c<=100;++c)dp[v][c]=min(sum[c],best);
}
int main(){
    int t;scanf("%d",&t);
    while(t--){
        memset(ch,0,sizeof(ch));memset(rate,0,sizeof(rate));nodes=1;
        int n;scanf("%d",&n);
        while(n--){char s[20];int c;scanf("%s%d",s,&c);int v=0;
            for(int i=0;s[i];++i){int d=s[i]-'0';if(!ch[v][d])ch[v][d]=nodes++;v=ch[v][d];}rate[v]=c;
        }
        int ans=0;for(int d=1;d<=9;++d)if(ch[0][d]){solve(ch[0][d],0);ans+=dp[ch[0][d]][0];}
        printf("%d\n",ans);
    }
    return 0;
}
