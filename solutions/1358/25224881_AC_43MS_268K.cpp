// POJ 1358 - Housing Complexes
// Model: gpt-6.1-sol
// Find every owner whose removal permits a fixed h-row, w-column rectangle
// using prefix sums of forbidden cells. Lands already having an empty rectangle
// need no owner; match the remaining lands to distinct owners by augmenting paths.
// Rotation is not allowed, as confirmed by the problem's discussion board.
#include <cstdio>
#include <cstring>
char land[51][51];
int p[51][51], mt[26];
bool edge[30][26], seen[26], freeLand[30];
int m,n,h,w;
bool fits(char owner) {
    int i,j;
    memset(p,0,sizeof(p));
    for(i=1;i<=m;++i) for(j=1;j<=n;++j)
        p[i][j]=p[i-1][j]+p[i][j-1]-p[i-1][j-1]+
            (land[i-1][j-1]!='0' && land[i-1][j-1]!=owner);
    for(i=h;i<=m;++i) for(j=w;j<=n;++j)
        if(p[i][j]-p[i-h][j]-p[i][j-w]+p[i-h][j-w]==0) return true;
    return false;
}
bool augment(int x) {
    for(int c=0;c<26;++c) if(edge[x][c] && !seen[c]) {
        seen[c]=true;
        if(mt[c]<0 || augment(mt[c])) {mt[c]=x;return true;}
    }
    return false;
}
int main() {
    int t,k;
    scanf("%d",&t);
    while(t--) {
        scanf("%d%d%d%d%d",&k,&m,&n,&h,&w);
        memset(edge,0,sizeof(edge));
        int ans=0;
        for(int x=0;x<k;++x) {
            for(int i=0;i<m;++i) scanf("%s",land[i]);
            freeLand[x]=fits('0');
            if(freeLand[x]) ++ans;
            else for(int c=0;c<26;++c) edge[x][c]=fits(char('A'+c));
        }
        memset(mt,-1,sizeof(mt));
        for(int x=0;x<k;++x) if(!freeLand[x]) {
            memset(seen,0,sizeof(seen));
            if(augment(x)) ++ans;
        }
        printf("%d\n",ans);
    }
    return 0;
}
