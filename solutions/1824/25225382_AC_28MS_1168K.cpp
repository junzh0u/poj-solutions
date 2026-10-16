// POJ 1824 - TwoFour
// Model: gpt-6.1-sol
// Minimax over counts of pile sizes and ownership of the two-bead piles.
// Conservation determines counts of sizes two and three from the stored counts.
// A two-bead pile can lose its owner; both ownership choices are considered.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int n;
signed char memo[31][31][31][31];
int solve(int z,int o,int f,int own) {
    signed char &ans=memo[z][o][f][own];
    if(ans!=127) return ans;
    int c[5]; c[0]=z;c[1]=o;c[4]=f;c[3]=2*z+o-2*f;
    c[2]=n-c[0]-c[1]-c[3]-c[4];
    int best=-31;
    for(int a=2;a<=4;++a) for(int b=0;b+2<=a;++b) {
        if(!c[a] || !c[b]) continue;
        int removes=(a==2 || b==2);
        for(int takeOwn=0;takeOwn<=removes;++takeOwn) {
            if(takeOwn>own || removes-takeOwn>c[2]-own) continue;
            int d[5]; for(int i=0;i<5;++i)d[i]=c[i];
            --d[a];--d[b];++d[a-1];++d[b+1];
            int other=c[2]-own-(removes-takeOwn);
            best=max(best,-solve(d[0],d[1],d[4],other));
        }
    }
    if(best==-31)best=2*own-n;
    return ans=(signed char)best;
}
int main(){
    int s; if(scanf("%d%d",&n,&s)!=2)return 0;
    memset(memo,127,sizeof(memo));
    while(s--){int c[5]={0};for(int i=0;i<n;++i){int x;scanf("%d",&x);++c[x];}
        int v=solve(c[0],c[1],c[4],c[2]/2);
        printf("%d\n",v>0?1:v<0?2:0);
    }
    return 0;
}
