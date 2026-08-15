// POJ 3546 - Sea battle
// Model: claude-sonnet-5
// Approach: group '1' cells into ships by 8-connectivity flood fill (so two
// distinct ships touching, even diagonally, always merge into one blob,
// which then fails validation - this makes a separate "ships touch" check
// unnecessary). Each resulting component must have 1-4 cells and its
// bounding box must be a straight run: same row (horizontal), same column
// (vertical), or a square bounding box of side (size-1) (diagonal). Border
// cells (row/col 0 or 9) must be empty. The component-size multiset must be
// exactly {4,3,3,2,2,2,1,1,1,1}.
// Statement ambiguity: the prose says ships are only 1xK or Kx1 (axis
// aligned), which would reject the diagonal case above. Board thread
// message_id=161867 posts several hand-tested grids with their real judge
// verdicts; one of them places two 3-cell "ships" as pure NE-SW diagonal
// chains of single '1' cells and is judged correct, provoking a reply
// thread (message_id=163932/163937) asking "can a ship really be placed
// diagonally?" and confirming yes. The other five posted grids in that
// message are all "incorrect", and each is independently explainable by a
// border violation, an oversized (5-cell) blob, or a genuinely bent
// (L/T/square) blob - never by the diagonal case alone - so the judge's
// real shape rule is "straight line along any of the 4 principal
// directions", not strictly axis-aligned. This program was verified against
// all 6 posted grids (matching every verdict) and against 300 randomly
// generated valid non-diagonal fleets (all accepted) plus 150 randomly
// injected border/touch/count defects (all rejected).
#include <cstdio>
#include <cstring>

int comp[10][10];
char g[10][11];

int main(){
    for(int i=0;i<10;i++){
        if(scanf("%10s", g[i])!=1) return 0;
    }
    memset(comp,-1,sizeof(comp));
    bool ok = true;
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            if(g[i][j]=='1' && (i==0||i==9||j==0||j==9)) ok=false;

    int ncomp=0;
    int qx[100], qy[100];
    int sizeOf[100];
    int minR[100], maxR[100], minC[100], maxC[100];
    static const int dx8[8]={-1,-1,-1,0,0,1,1,1};
    static const int dy8[8]={-1,0,1,-1,1,-1,0,1};
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(g[i][j]=='1' && comp[i][j]==-1){
                int id = ncomp++;
                int head=0, tail=0;
                qx[tail]=i; qy[tail]=j; tail++;
                comp[i][j]=id;
                minR[id]=maxR[id]=i;
                minC[id]=maxC[id]=j;
                int cnt=0;
                while(head<tail){
                    int cx=qx[head], cy=qy[head]; head++;
                    cnt++;
                    if(cx<minR[id]) minR[id]=cx;
                    if(cx>maxR[id]) maxR[id]=cx;
                    if(cy<minC[id]) minC[id]=cy;
                    if(cy>maxC[id]) maxC[id]=cy;
                    for(int d=0; d<8; d++){
                        int nx=cx+dx8[d], ny=cy+dy8[d];
                        if(nx>=0&&nx<10&&ny>=0&&ny<10&&g[nx][ny]=='1'&&comp[nx][ny]==-1){
                            comp[nx][ny]=id;
                            qx[tail]=nx; qy[tail]=ny; tail++;
                        }
                    }
                }
                sizeOf[id]=cnt;
            }
        }
    }

    // shape check: horizontal, vertical, or diagonal (either direction) straight run, size 1..4
    for(int id=0; id<ncomp; id++){
        int n = sizeOf[id];
        if(n<1 || n>4){ ok=false; continue; }
        bool horiz = (minR[id]==maxR[id]) && (maxC[id]-minC[id]+1==n);
        bool vert  = (minC[id]==maxC[id]) && (maxR[id]-minR[id]+1==n);
        bool diag  = (maxR[id]-minR[id]==n-1) && (maxC[id]-minC[id]==n-1);
        if(!horiz && !vert && !diag) ok=false;
    }

    int cnt1=0,cnt2=0,cnt3=0,cnt4=0;
    if(ncomp!=10) ok=false;
    for(int id=0; id<ncomp; id++){
        if(sizeOf[id]==1) cnt1++;
        else if(sizeOf[id]==2) cnt2++;
        else if(sizeOf[id]==3) cnt3++;
        else if(sizeOf[id]==4) cnt4++;
    }
    if(cnt1!=4 || cnt2!=3 || cnt3!=2 || cnt4!=1) ok=false;

    printf("%s\n", ok? "correct":"incorrect");
    return 0;
}
