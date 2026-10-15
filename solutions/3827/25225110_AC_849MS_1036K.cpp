// POJ 3827 - Facer is learning to swim
// Model: gpt-6.1-sol
// Surface visits reset both velocity and oxygen, so enumerate each excursion
// of at most K moves and combine its return with a column dynamic program.
// Bottom clipping preserves velocity; K-1 underwater cells are permitted.
#include <cstdio>
#include <algorithm>
using namespace std;
int n,m,k,changev[100][1000],money[100][1000],best[1000];
void walk(int x,int y,int v,int left,int value) {
    if(y+1>=m || left==0) return;
    int nx=max(0,min(n-1,x+v)),ny=y+1;
    value+=money[nx][ny];
    if(nx==0) { best[ny]=max(best[ny],value); return; }
    if(left==1 || ny==m-1) return;
    v+=changev[nx][ny];
    for(int q=-1;q<=1;++q) walk(nx,ny,v+q,left-1,value);
}
int main() {
    while(scanf("%d%d%d",&n,&m,&k)==3 && n) {
        for(int x=0;x<n;++x) for(int y=0;y<m;++y) {
            char c; int a; scanf(" %c%d",&c,&a);
            changev[x][y]=c=='v'?a:0;
            money[x][y]=c=='$'?a:0;
        }
        fill(best,best+m,-2000000000); best[0]=money[0][0];
        for(int y=0;y<m-1;++y) {
            walk(0,y,0,k,best[y]);
            walk(0,y,1,k,best[y]);
        }
        printf("%d\n",best[m-1]);
    }
    return 0;
}
