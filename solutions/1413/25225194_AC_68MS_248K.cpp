// POJ 1413 - GIGA Universe Cup
// Model: gpt-6.1-sol
// Enumerate the 9^4 possible remaining scores with binomial weights.
// Rank tied subsets recursively using only their mutual matches; when no
// split is possible, fair lots give each team the remaining slots equally.
#include <cstdio>
#include <cstring>
int goals[4][4], target;
int countbits(int x) { int n=0; for (;x;x&=x-1) ++n; return n; }
double qualify(int mask,int slots) {
    int n=countbits(mask);
    if(slots<=0) return 0;
    if(slots>=n) return 1;
    int p[4]={0},d[4]={0},g[4]={0};
    for(int i=0;i<4;++i) if(mask&(1<<i))
        for(int j=0;j<4;++j) if(i!=j && (mask&(1<<j))) {
            g[i]+=goals[i][j]; d[i]+=goals[i][j]-goals[j][i];
            p[i]+=goals[i][j]>goals[j][i]?3:goals[i][j]==goals[j][i]?1:0;
        }
    int tied=0,better=0;
    for(int i=0;i<4;++i) if(mask&(1<<i)) {
        if(p[i]==p[target] && d[i]==d[target] && g[i]==g[target]) tied|=1<<i;
        else if(p[i]>p[target] || (p[i]==p[target] && (d[i]>d[target] || (d[i]==d[target] && g[i]>g[target])))) ++better;
    }
    if(tied==mask) return double(slots)/n;
    return qualify(tied,slots-better);
}
int main() {
    double prob[9]; prob[0]=6561.0/65536.0;
    for(int i=1;i<=8;++i) prob[i]=prob[i-1]*(9-i)/(3*i);
    int cases; std::scanf("%d",&cases);
    while(cases--) {
        char s[100]; std::scanf("%s",s); target=0;
        for(int i=0;i<4;++i) if(s[5+5*i]=='*') target=i;
        int a[2],b[2],k=0;
        for(int i=0;i<4;++i) {
            std::scanf("%s",s);
            for(int j=i+1;j<4;++j) {
                int at=5+5*j;
                if(s[at+1]=='_') { a[k]=i;b[k++]=j; }
                else { goals[i][j]=s[at+1]-'0';goals[j][i]=s[at+3]-'0'; }
            }
        }
        double answer=0;
        for(int x=0;x<=8;++x) for(int y=0;y<=8;++y)
            for(int z=0;z<=8;++z) for(int w=0;w<=8;++w) {
                goals[a[0]][b[0]]=x;goals[b[0]][a[0]]=y;
                goals[a[1]][b[1]]=z;goals[b[1]][a[1]]=w;
                answer+=prob[x]*prob[y]*prob[z]*prob[w]*qualify(15,2);
            }
        std::printf("%.7f\n",answer);
    }
}
