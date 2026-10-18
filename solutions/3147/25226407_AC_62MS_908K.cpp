// POJ 3147 - John's Canonical Difference Bound Matrices
// Model: gpt-6-astra
// Integer clocks turn each strict bound x-y<d into x-y<=d-1.
// Floyd-Warshall gives all tight bounds; a negative diagonal is inconsistent.
// Output the equivalent non-strict representation for every finite bound.
#include <cstdio>
#include <cstdlib>
const int INF=100000000;
int d[101][101];
bool strict[101][101];
int main() {
    int T; if(scanf("%d", &T)!=1) return 0;
    for(int tc=1;tc<=T;++tc) {
        for(int i=0;i<101;++i) for(int j=0;j<101;++j) {
            d[i][j]=i==j?0:INF; strict[i][j]=false;
        }
        char token[40]; int m=0;
        while(scanf("%39s",token)==1 && token[0]!='#') {
            int i=atoi(token),j,w,r; scanf("%d%d%d",&j,&w,&r);
            if(r==0) --w; r=1;
            if(i>m) m=i; if(j>m) m=j;
            if(w<d[i][j] || (w==d[i][j] && r==0)) {
                d[i][j]=w; strict[i][j]=(r==0);
            }
        }
        bool bad=false;
        for(int i=0;i<=m;++i)
            if(d[i][i]<0 || (d[i][i]==0 && strict[i][i])) bad=true;
        for(int k=0;k<=m && !bad;++k) {
            for(int i=0;i<=m && !bad;++i) if(d[i][k]!=INF)
                for(int j=0;j<=m && !bad;++j) if(d[k][j]!=INF) {
                    int w=d[i][k]+d[k][j];
                    bool s=strict[i][k] || strict[k][j];
                    if(w<d[i][j] || (w==d[i][j] && s)) {
                        d[i][j]=w; strict[i][j]=s;
                        if(i==j && (w<0 || (w==0 && s))) bad=true;
                    }
                }
            for(int i=0;i<=m;++i)
                if(d[i][i]<0 || (d[i][i]==0 && strict[i][i])) bad=true;
        }
        printf("Case %d:\n",tc);
        if(bad) puts("Canonical DBM does not exist.");
        else for(int i=0;i<=m;++i) {
            for(int j=0;j<=m;++j) {
                if(j) putchar(' ');
                if(d[i][j]==INF) printf("($,<)");
                else printf("(%d,%s)",d[i][j],strict[i][j]?"<":"<=");
            }
            putchar('\n');
        }
        putchar('\n');
    }
    return 0;
}
