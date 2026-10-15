// POJ 2933 - Explaining the Stock Market
// Model: gpt-6.1-sol
// Iterative deepening partitions days into monotone subsequences of size >= 2.
// Equal-only groups keep both directions available. Among established groups
// of one direction, an exchange of future suffixes permits best-fit placement.
#include <cstdio>
int n,a[30],tail[30],dir[30],sz[30],limit;
bool search(int p,int used,int single) {
    if(n-p<single) return false;
    if(p==n) return single==0;
    int x=a[p],up=-1,down=-1;
    for(int i=0;i<used;++i) {
        if(dir[i]==1 && tail[i]<=x && (up<0 || tail[i]>tail[up])) up=i;
        if(dir[i]==-1 && tail[i]>=x && (down<0 || tail[i]<tail[down])) down=i;
    }
    for(int t=0;t<2;++t) {
        int i=t==0?up:down;
        if(i<0) continue;
        int old=tail[i]; tail[i]=x;
        if(search(p+1,used,single)) return true;
        tail[i]=old;
    }
    bool seen[101][2]={ {false} };
    for(int i=0;i<used;++i) if(dir[i]==0) {
        int old=tail[i],s=sz[i];
        if(seen[old][s-1]) continue;
        seen[old][s-1]=true;
        tail[i]=x; sz[i]=2; dir[i]=(x>old)-(x<old);
        if(search(p+1,used,single-(s==1))) return true;
        tail[i]=old; sz[i]=s; dir[i]=0;
    }
    if(used<limit && n-p>=single+2) {
        tail[used]=x; dir[used]=0; sz[used]=1;
        if(search(p+1,used+1,single+1)) return true;
    }
    return false;
}
int main() {
    if(scanf("%d",&n)!=1) return 0;
    for(int i=0;i<n;++i) scanf("%d",&a[i]);
    for(limit=1;limit<=n/2;++limit) if(search(0,0,0)) {
        printf("%d\n",limit); return 0;
    }
    puts("0"); return 0;
}
