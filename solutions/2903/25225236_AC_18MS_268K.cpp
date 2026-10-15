// POJ 2903 - Joy of Mobile Routing
// Model: gpt-6.1-sol
// Trace the ray from each intersection to each antenna top through grid cells.
// Integer fractions distinguish crossing a building from touching its boundary.
// BFS uses covered intersections; the destination needs no further phone call.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
using namespace std;
int R,C,H[51][51];
bool ok[51][51];
bool visible(int r,int c,int ar,int ac,int h) {
    int dr=ar-r, dc=ac-c;
    if (!dr || !dc) return true;
    int sr=dr>0?1:-1, sc=dc>0?1:-1;
    int nr=abs(dr), nc=abs(dc);
    int x=r+(sr<0?-1:0), y=c+(sc<0?-1:0);
    int ir=1,ic=1, num=0,den=1;
    while (true) {
        if (H[x][y]*den>h*num) return false;
        int cmp=ir*nc-ic*nr;
        if (cmp<=0) {num=ir;den=nr;} else {num=ic;den=nc;}
        if (num==den) return true;
        if (cmp<=0) {x+=sr;++ir;}
        if (cmp>=0) {y+=sc;++ic;}
    }
}
int main() {
    int T; scanf("%d",&T);
    while(T--) {
        scanf("%d%d",&R,&C);
        for(int i=0;i<R;++i) for(int j=0;j<C;++j) scanf("%d",&H[i][j]);
        int sr,sc,tr,tc,A; scanf("%d%d%d%d%d",&sr,&sc,&tr,&tc,&A);
        memset(ok,0,sizeof(ok));
        while(A--) {
            int r,c,h;scanf("%d%d%d",&r,&c,&h);
            for(int i=0;i<=R;++i) for(int j=0;j<=C;++j)
                if(!ok[i][j] && visible(i,j,r,c,h)) ok[i][j]=true;
        }
        ok[tr][tc]=true;
        int d[51][51];memset(d,-1,sizeof(d));queue<int> q;
        if(ok[sr][sc]) {q.push(sr*51+sc);d[sr][sc]=0;}
        const int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
        while(!q.empty()) {
            int r=q.front()/51,c=q.front()%51;q.pop();
            for(int k=0;k<4;++k) {
                int x=r+dx[k],y=c+dy[k];
                if(x>=0&&x<=R&&y>=0&&y<=C&&ok[x][y]&&d[x][y]<0) {
                    d[x][y]=d[r][c]+10;q.push(x*51+y);
                }
            }
        }
        printf("%d\n",d[tr][tc]);
    }
    return 0;
}
