// POJ 2212 - Cavern
// Model: gpt-6.1-sol
// Flood fill all water connected by faces to any top-layer water cube.
// Travel may go upward as well as downward; each reached cube holds 1000 L.
#include <cstdio>
#include <cstring>
static unsigned char water[102*102*102];
static int q[1000000];
int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        int x,y,z;
        scanf("%d%d%d", &x,&y,&z);
        int row=y+2, plane=(x+2)*row;
        memset(water,0,sizeof(water));
        for (int k=1;k<=z;++k) {
            int p; scanf("%d",&p);
            while (p--) {
                int r,s; scanf("%d%d",&r,&s);
                water[k*plane+r*row+s]=1;
            }
        }
        int head=0,tail=0;
        for (int r=1;r<=x;++r) for (int s=1;s<=y;++s) {
            int v=plane+r*row+s;
            if (water[v]) { water[v]=0; q[tail++]=v; }
        }
        int delta[6]={1,-1,row,-row,plane,-plane};
        while (head<tail) {
            int v=q[head++];
            for (int d=0;d<6;++d) {
                int w=v+delta[d];
                if (water[w]) { water[w]=0; q[tail++]=w; }
            }
        }
        printf("Je nutne vycerpat %d litru vody.\n",tail*1000);
    }
    return 0;
}
