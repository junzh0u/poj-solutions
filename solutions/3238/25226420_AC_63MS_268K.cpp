// POJ 3238 - A Coin Game
// Model: gpt-6-astra
// Rectangle-turning coin values are computed by mex of the three lower
// corners. XOR of all heads is zero exactly for losing positions.
// An XOR basis with coefficient witnesses chooses Sally's heads.
// Input runs to EOF; the first row and column have value zero.
#include <cstdio>
#include <bitset>
using namespace std;
int g[100][100], seen[65536];
int main() {
    int stamp=0;
    for(int x=1;x<100;++x) for(int y=1;y<100;++y) {
        ++stamp;
        for(int a=0;a<x;++a) for(int b=0;b<y;++b)
            seen[g[a][y]^g[x][b]^g[a][b]]=stamp;
        while(seen[g[x][y]]==stamp) ++g[x][y];
    }
    int n,m;
    while(scanf("%d%d",&n,&m)==2) {
        int target=0,x,y,basis[16]={0};
        bitset<200> witness[16],answer;
        for(int i=0;i<n;++i) {scanf("%d%d",&x,&y);target^=g[x-1][y-1];}
        for(int i=0;i<m;++i) {
            scanf("%d%d",&x,&y);
            int v=g[x-1][y-1]; bitset<200> w; w.set(i);
            for(int b=15;b>=0;--b) if(v&(1<<b)) {
                if(basis[b]) {v^=basis[b];w^=witness[b];}
                else {basis[b]=v;witness[b]=w;break;}
            }
        }
        for(int b=15;b>=0;--b) if(target&(1<<b)) {
            target^=basis[b];answer^=witness[b];
        }
        if(target) puts("No");
        else {puts("Yes");for(int i=0;i<m;++i) puts(answer[i]?"H":"T");}
    }
}
