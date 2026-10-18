// POJ 2811 - Packing Dominoes
// Model: gpt-6-astra
// Scan cells in order, either leaving one empty or starting an unused domino.
// Check every contact except the two halves of that same domino. Relabeling
// all pips preserves the complete double-K set, so introduce new labels in
// order. Cache each board size and transpose witnesses for rotated boards.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int k,w,h,n,a[10],b[10],board[144],px[10],py[10];
int state[4][13][13],saved[4][13][13][10][4];
bool fits(int s,int v,int mate) {
    int x=s%w,y=s/w;
    if(x && s-1!=mate && board[s-1]>=0 && board[s-1]!=v) return false;
    if(x+1<w && s+1!=mate && board[s+1]>=0 && board[s+1]!=v) return false;
    if(y && s-w!=mate && board[s-w]>=0 && board[s-w]!=v) return false;
    if(y+1<h && s+w!=mate && board[s+w]>=0 && board[s+w]!=v) return false;
    return true;
}
bool search(int s,int mask,int left,int remaining,int colors) {
    if(!mask) return true;
    if(left<2*remaining) return false;
    while(s<w*h && board[s]>=0) ++s;
    if(s==w*h) return false;
    for(int d=0;d<2;++d) {
        int t=s+(d?w:1);
        if(t>=w*h || (!d && s%w==w-1) || board[t]!=-1) continue;
        for(int i=0;i<n;++i) if(mask&(1<<i)) {
            for(int rev=0;rev<1+(a[i]!=b[i]);++rev) {
                int u=rev?b[i]:a[i],v=rev?a[i]:b[i];
                if(u>colors || v>colors+(u==colors)) continue;
                if(!fits(s,u,t) || !fits(t,v,s)) continue;
                board[s]=u; board[t]=v;
                px[i]=rev?t:s; py[i]=rev?s:t;
                int nc=colors;
                if(u==nc) ++nc;
                if(v==nc) ++nc;
                if(search(s+1,mask^(1<<i),left-2,remaining-1,nc)) return true;
                board[s]=board[t]=-1;
            }
        }
    }
    board[s]=-2;
    bool ok=search(s+1,mask,left-1,remaining,colors);
    board[s]=-1;
    return ok;
}
int main() {
    int width,height;
    while(scanf("%d%d%d",&k,&width,&height)==3 && k>0) {
        printf("%d %d %d\n",k,width,height);
        w=min(width,height); h=max(width,height); n=0;
        for(int u=0;u<=k;++u) for(int v=u;v<=k;++v) {
            a[n]=u; b[n++]=v;
        }
        if(!state[k][w][h]) {
            memset(board,-1,sizeof board);
            bool ok=search(0,(1<<n)-1,w*h,n,0);
            state[k][w][h]=ok?1:-1;
            if(ok) for(int i=0;i<n;++i) {
                saved[k][w][h][i][0]=px[i]%w;
                saved[k][w][h][i][1]=px[i]/w;
                saved[k][w][h][i][2]=py[i]%w;
                saved[k][w][h][i][3]=py[i]/w;
            }
        }
        if(state[k][w][h]<0) puts("No packing is possible");
        else for(int i=0;i<n;++i) {
            int *p=saved[k][w][h][i];
            if(width<=height) printf("%d %d %d %d %d %d\n",a[i],b[i],p[0],p[1],p[2],p[3]);
            else printf("%d %d %d %d %d %d\n",a[i],b[i],p[1],p[0],p[3],p[2]);
        }
    }
    return 0;
}
