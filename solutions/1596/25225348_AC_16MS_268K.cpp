// POJ 1596 - Gears on a Board
// Model: gpt-6.1-sol
// Check both tooth planes independently for overlap and tangency.
// Propagate signed angular-speed ratios from the motor; prime exponents
// make all consistency checks exact. Disconnected gears remain idle.
// Carry numeric speeds along the traversal for conventional printf rounding.
// Both simultaneous contacts must agree, including contacts of one pair.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <queue>
using namespace std;
const int P=25;
const int primes[P]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
struct Gear { int x,y,r[2]; } g[21];
int fac[101][P], e[21][P],sgn[21];
int main() {
    for(int r=1;r<=100;r++) {
        int v=r;
        for(int p=0;p<P;p++) while(v%primes[p]==0) {fac[r][p]++;v/=primes[p];}
    }
    int n,av,tc=0;
    while(scanf("%d%d%d%d%d%d",&g[0].x,&g[0].y,&g[0].r[0],&g[0].r[1],&av,&n)==6) {
        for(int i=1;i<=n;i++) scanf("%d%d%d%d",&g[i].x,&g[i].y,&g[i].r[0],&g[i].r[1]);
        bool overlap=false,conflict=false;
        bool edge[21][21][2]; memset(edge,0,sizeof(edge));
        for(int i=0;i<=n;i++) for(int j=0;j<i;j++) {
            int dx=g[i].x-g[j].x,dy=g[i].y-g[j].y,d=dx*dx+dy*dy;
            for(int k=0;k<2;k++) {
                int sum=g[i].r[k]+g[j].r[k];
                if(d<sum*sum) overlap=true;
                if(d==sum*sum) edge[i][j][k]=edge[j][i][k]=true;
            }
        }
        memset(sgn,0,sizeof(sgn)); memset(e,0,sizeof(e));
        double velocity[21]={0}; velocity[0]=av;
        queue<int> q; sgn[0]=av>0?1:-1;q.push(0);
        while(!q.empty()) {
            int i=q.front();q.pop();
            for(int j=0;j<=n;j++) for(int k=0;k<2;k++) if(edge[i][j][k]) {
                int next[P];
                for(int p=0;p<P;p++) next[p]=e[i][p]+fac[g[i].r[k]][p]-fac[g[j].r[k]][p];
                if(!sgn[j]) {
                    velocity[j]=-velocity[i]*g[i].r[k]/g[j].r[k];
                    sgn[j]=-sgn[i]; memcpy(e[j],next,sizeof(next));q.push(j);
                } else {
                    if(sgn[j]!=-sgn[i]) conflict=true;
                    for(int p=0;p<P;p++) if(e[j][p]!=next[p]) conflict=true;
                }
            }
        }
        printf("Simulation #%d\n",++tc);
        if(overlap) puts("Error -- Overlapping Gears");
        else if(conflict) puts("Error -- Conflicting Gear Rotation");
        else for(int i=1;i<=n;i++) {
            printf("%2d: ",i);
            if(!sgn[i]) puts("Warning -- Idle Gear");
            else {
                double speed=fabs(velocity[i]);
                printf("%c %.2f\n",sgn[i]>0?'R':'L',speed);
            }
        }
        puts("");
    }
    return 0;
}
