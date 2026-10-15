// POJ 2855 - Go
// Model: gpt-6.1-sol
// Flood-fill opponent groups after each placement, removing those without
// liberties. Flood-fill final empty regions and count their boundary colours.
// Board edges enclose territory; a region without either colour belongs to
// both, as in the contest reference. Legal input needs no suicide/ko checks.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
int n, a[19][19], seen[19][19];
const int dx[4]={1,-1,0,0}, dy[4]={0,0,1,-1};
bool inside(int x,int y) { return x>=0 && x<n && y>=0 && y<n; }
void fill(int x,int y,int color,vector<int>& cells,bool& liberty,int& mask) {
    seen[x][y]=1; cells.push_back(x*n+y);
    for(int d=0;d<4;++d) {
        int u=x+dx[d],v=y+dy[d];
        if(!inside(u,v)) continue;
        if(a[u][v]==0) liberty=true;
        else mask|=1<<a[u][v];
        if(a[u][v]==color && !seen[u][v]) fill(u,v,color,cells,liberty,mask);
    }
}
int main() {
    int m;
    while(scanf("%d%d",&n,&m)==2 && n) {
        memset(a,0,sizeof(a)); int score[3]={0,0,0};
        for(int i=0;i<m;++i) {
            char c; int x,y;
            scanf(" %c(%d,%d)",&c,&x,&y);
            x+=n/2; y+=n/2; int p=(c=='B'?1:2),q=3-p;
            a[x][y]=p;
            memset(seen,0,sizeof(seen));
            vector<int> dead;
            for(int d=0;d<4;++d) {
                int u=x+dx[d],v=y+dy[d];
                if(!inside(u,v) || a[u][v]!=q || seen[u][v]) continue;
                vector<int> cells; bool liberty=false; int mask=0;
                fill(u,v,q,cells,liberty,mask);
                if(!liberty) dead.insert(dead.end(),cells.begin(),cells.end());
            }
            score[p]+=(int)dead.size();
            for(unsigned j=0;j<dead.size();++j) a[dead[j]/n][dead[j]%n]=0;
        }
        memset(seen,0,sizeof(seen));
        for(int x=0;x<n;++x) for(int y=0;y<n;++y) if(!a[x][y]&&!seen[x][y]) {
            vector<int> cells; bool liberty=false; int mask=0;
            fill(x,y,0,cells,liberty,mask);
            if(!(mask&(1<<2))) score[1]+=(int)cells.size();
            if(!(mask&(1<<1))) score[2]+=(int)cells.size();
        }
        printf("%d %d\n",score[1],score[2]);
    }
    return 0;
}
