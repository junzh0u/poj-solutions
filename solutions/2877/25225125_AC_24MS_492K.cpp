// POJ 2877 - Chambers Ceramic Conundrum
// Model: gpt-6.1-sol
// Search tiles A-I and clockwise rotations in the prescribed order.
// Anchor each shape's first cell at the first empty floor cell. Prune
// components whose areas are not multiples of four and memoize failures.
// Equal-shaped remaining tiles are interchangeable for feasibility, so
// only the earliest is tried at each node. Reflections are never generated.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
using namespace std;
typedef unsigned long long Mask;
typedef pair<int,int> Cell;
vector<Cell> shapes[7][4];
Mask places[7][4][36];
Mask adjacent[36];
int tile[9];
char board[36];
set<pair<Mask,int> > failed;
const Mask FULL=(Mask(1)<<36)-1;
bool components(Mask occupied) {
    Mask left=FULL^occupied;
    while(left) {
        int p=0; while(!(left&(Mask(1)<<p))) ++p;
        Mask frontier=Mask(1)<<p; left^=frontier;
        int count=0;
        while(frontier) {
            int q=0; while(!(frontier&(Mask(1)<<q))) ++q;
            frontier^=Mask(1)<<q; ++count;
            Mask next=adjacent[q]&left;
            left^=next; frontier|=next;
        }
        if(count%4) return false;
    }
    return true;
}
bool dfs(Mask occupied,int used) {
    if(used==511) return true;
    pair<Mask,int> key(occupied,used);
    if(failed.count(key)) return false;
    if(!components(occupied)) { failed.insert(key); return false; }
    int p=0; while(occupied&(Mask(1)<<p)) ++p;
    int tried=0;
    for(int i=0;i<9;++i) if(!(used&(1<<i))) {
        int s=tile[i];
        if(tried&(1<<s)) continue;
        tried|=1<<s;
        for(int r=0;r<4;++r) {
            Mask m=places[s][r][p];
            if(!m || (m&occupied)) continue;
            for(int j=0;j<36;++j) if(m&(Mask(1)<<j)) board[j]=char('A'+i);
            if(dfs(occupied|m,used|(1<<i))) return true;
        }
    }
    failed.insert(key); return false;
}
int main() {
    const char *top[7]={"XXXX","XX","XX "," XX","X  ","XXX","  X"};
    const char *bottom[7]={"    ","XX"," XX","XX ","XXX","X  ","XXX"};
    for(int s=0;s<7;++s) {
        for(int c=0;top[s][c];++c) if(top[s][c]=='X') shapes[s][0].push_back(Cell(0,c));
        for(int c=0;bottom[s][c];++c) if(bottom[s][c]=='X') shapes[s][0].push_back(Cell(1,c));
        for(int r=0;r<4;++r) {
            if(r) for(int j=0;j<4;++j) {
                Cell a=shapes[s][r-1][j]; shapes[s][r].push_back(Cell(a.second,-a.first));
            }
            sort(shapes[s][r].begin(),shapes[s][r].end());
            Cell first=shapes[s][r][0];
            for(int j=0;j<4;++j) { shapes[s][r][j].first-=first.first; shapes[s][r][j].second-=first.second; }
            for(int p=0;p<36;++p) {
                Mask m=0; bool valid=true;
                for(int j=0;j<4;++j) {
                    int y=p/6+shapes[s][r][j].first,x=p%6+shapes[s][r][j].second;
                    if(y<0||y>=6||x<0||x>=6) {valid=false;break;}
                    m|=Mask(1)<<(y*6+x);
                }
                places[s][r][p]=valid?m:0;
            }
        }
    }
    for(int p=0;p<36;++p) for(int q=0;q<36;++q)
        if(abs(p/6-q/6)+abs(p%6-q%6)==1) adjacent[p]|=Mask(1)<<q;
    int n; if(scanf("%d",&n)!=1) return 0;
    for(int cs=1;cs<=n;++cs) {
        for(int i=0;i<9;++i) {scanf("%d",&tile[i]);--tile[i];}
        failed.clear(); bool ok=dfs(0,0);
        printf("Data Set %d\nThe floor may %sbe tiled.\n",cs,ok?"":"not ");
        if(ok) for(int y=0;y<6;++y) {for(int x=0;x<6;++x) putchar(board[y*6+x]);putchar('\n');}
        putchar('\n');
    }
    puts("End of Output"); return 0;
}
