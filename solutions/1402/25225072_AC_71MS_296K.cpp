// POJ 1402 - The Game of Master-Mind
// Model: gpt-6.1-sol
// Lexicographic depth-first search with exact black and multiset-match bounds.
// Repeated colours contribute only up to their frequency in each old guess.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int P,C,M,g[100][10],freq[100][101],B[100],T[100];
int cnt[101],black[100],total[100],ans[10];
bool domain[10][101];
int maxb[100][11],minb[100][11];
bool dfs(int pos) {
    int rem=P-pos;
    for(int i=0;i<M;++i) {
        if(black[i]+minb[i][pos]>B[i] || black[i]+maxb[i][pos]<B[i]) return false;
        if(total[i]>T[i] || total[i]+rem<T[i]) return false;
    }
    if(pos==P) return true;
    for(int c=1;c<=C;++c) if(domain[pos][c]) {
        ans[pos]=c;
        for(int i=0;i<M;++i) {
            black[i]+=(g[i][pos]==c);
            total[i]+=(cnt[c]<freq[i][c]);
        }
        ++cnt[c];
        if(dfs(pos+1)) return true;
        --cnt[c];
        for(int i=0;i<M;++i) {
            black[i]-=(g[i][pos]==c);
            total[i]-=(cnt[c]<freq[i][c]);
        }
    }
    return false;
}
int main() {
    int cases; scanf("%d",&cases);
    while(cases--) {
        scanf("%d%d%d",&P,&C,&M);
        memset(freq,0,sizeof(freq)); memset(cnt,0,sizeof(cnt));
        memset(black,0,sizeof(black)); memset(total,0,sizeof(total));
        memset(domain,1,sizeof(domain));
        for(int i=0;i<M;++i) {
            for(int j=0;j<P;++j) {scanf("%d",&g[i][j]); ++freq[i][g[i][j]];}
            int w; scanf("%d%d",&B[i],&w); T[i]=B[i]+w;
            if(B[i]==0) for(int j=0;j<P;++j) domain[j][g[i][j]]=false;
            if(T[i]==0) for(int j=0;j<P;++j) for(int k=0;k<P;++k) domain[j][g[i][k]]=false;
        }
        for(int i=0;i<M;++i) {
            maxb[i][P]=minb[i][P]=0;
            for(int j=P-1;j>=0;--j) {
                int n=0; for(int c=1;c<=C;++c) n+=domain[j][c];
                maxb[i][j]=maxb[i][j+1]+domain[j][g[i][j]];
                minb[i][j]=minb[i][j+1]+(n==1 && domain[j][g[i][j]]);
            }
        }
        if(dfs(0)) {for(int j=0;j<P;++j) printf("%s%d",j?" ":"",ans[j]); puts("");}
        else puts("You are cheating!");
    }
    return 0;
}
