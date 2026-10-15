// POJ 1600 - Centipede Collisions
// Model: gpt-6.1-sol
// Simulate each segment, advancing centipedes in cyclic numeric order.
// All moving segments of one centipede move simultaneously; only segments
// at a collision stop, while the other segments continue independently.
#include <cstdio>
#include <vector>
#include <cstring>
using namespace std;
struct Segment { int x,y,dx,dy,owner; bool moving; };
int main() {
    int n;
    while (scanf("%d", &n)==1 && n>0) {
        vector<Segment> s;
        int cell[30][30]; bool crash[30][30];
        memset(cell,-1,sizeof(cell)); memset(crash,0,sizeof(crash));
        for(int i=0;i<n;++i) {
            char d; int l,x,y; scanf(" %c%d%d%d",&d,&l,&x,&y);
            int dx=(d=='R')-(d=='L'),dy=(d=='U')-(d=='D');
            for(int j=0;j<l;++j) {
                Segment a; a.x=x-j*dx; a.y=y-j*dy;
                a.dx=dx; a.dy=dy; a.owner=i; a.moving=true;
                cell[a.x][a.y]=(int)s.size(); s.push_back(a);
            }
        }
        for(int turn=0;turn<30;++turn) for(int i=0;i<n;++i) {
            for(int j=0;j<(int)s.size();++j) if(s[j].owner==i && s[j].moving)
                cell[s[j].x][s[j].y]=-1;
            for(int j=0;j<(int)s.size();++j) if(s[j].owner==i && s[j].moving) {
                Segment &a=s[j]; a.x+=a.dx; a.y+=a.dy;
                if(a.x<0 || a.x>=30 || a.y<0 || a.y>=30) { a.moving=false; continue; }
                if(crash[a.x][a.y]) a.moving=false;
                else if(cell[a.x][a.y]>=0) {
                    s[cell[a.x][a.y]].moving=false; a.moving=false;
                    crash[a.x][a.y]=true;
                } else cell[a.x][a.y]=j;
            }
        }
        for(int k=0;k<2;++k) {
            printf("  "); for(int x=0;x<30;++x) printf(" %d",k==0?x/10:x%10); puts("");
        }
        for(int y=29;y>=0;--y) {
            printf("%02d",y); for(int x=0;x<30;++x) printf(" %c",crash[x][y]?'X':'.'); puts("");
        }
        puts("");
    }
    return 0;
}
