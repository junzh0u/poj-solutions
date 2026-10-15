// POJ 3985 - Knight's Problem
// Model: gpt-6.1-sol
// BFS in a finite corridor around the start-to-target segment.
// For any n-move solution, subtract target/n from each move. These
// zero-sum vectors have infinity norm at most 20. The 2D Steinitz
// rearrangement lemma bounds every partial sum by 40. Thus some ordering
// of the same moves stays within infinity distance 40 of the segment,
// preserving the shortest length. After swapping axes, a vertical margin
// of 81 around the line and horizontal endpoint margins of 40 contain it.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
    int tests; scanf("%d", &tests);
    while(tests--) {
        int fx,fy,tx,ty,m;
        scanf("%d%d%d%d%d",&fx,&fy,&tx,&ty,&m);
        tx-=fx; ty-=fy;
        int vx[10],vy[10];
        for(int i=0;i<m;++i) scanf("%d%d",vx+i,vy+i);
        if(abs(tx)<abs(ty)) {
            swap(tx,ty);
            for(int i=0;i<m;++i) swap(vx[i],vy[i]);
        }
        if(tx<0) {
            tx=-tx;
            for(int i=0;i<m;++i) vx[i]=-vx[i];
        }
        if(tx==0 && ty==0) { puts("0"); continue; }
        const int W=163;
        int rows=tx+81;
        vector<int> low(rows);
        for(int r=0;r<rows;++r) {
            int a=(r-40)*ty;
            int q=a/tx;
            if(a<0 && a%tx) --q;
            low[r]=q-81;
        }
        int count=rows*W;
        vector<int> dist(count,-1), que(count);
        int start=40*W-low[40];
        int goal=(tx+40)*W+ty-low[tx+40];
        int head=0,tail=0;
        dist[start]=0; que[tail++]=start;
        while(head<tail && dist[goal]<0) {
            int id=que[head++],r=id/W,y=id%W+low[r];
            for(int i=0;i<m;++i) {
                int nr=r+vx[i];
                if(nr<0 || nr>=rows) continue;
                int col=y+vy[i]-low[nr];
                if(col<0 || col>=W) continue;
                int next=nr*W+col;
                if(dist[next]<0) {
                    dist[next]=dist[id]+1;
                    que[tail++]=next;
                }
            }
        }
        if(dist[goal]<0) puts("IMPOSSIBLE");
        else printf("%d\n",dist[goal]);
    }
    return 0;
}
