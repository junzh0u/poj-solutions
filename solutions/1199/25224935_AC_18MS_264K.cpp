// POJ 1199 - Left labyrinths
// Model: gpt-6.1-sol
// Find the entrance from the exterior, then follow the left-hand wall.
// A courtyard contains a 2 by 2 floor square; revisits with a different
// heading are legal, so detect cycles using both position and direction.
#include <cstdio>
#include <queue>
#include <utility>
using namespace std;
char a[105][105];
bool outside[105][105], seen[105][105][4];
int n,m,dr[4]={-1,0,1,0},dc[4]={0,1,0,-1};
bool floorAt(int r,int c) { return r>=0 && r<=n+1 && c>=0 && c<=m+1 && a[r][c]!='#'; }
bool courtyard(int r,int c) {
    for(int x=-1;x<=0;++x) for(int y=-1;y<=0;++y)
        if(floorAt(r+x,c+y) && floorAt(r+x+1,c+y) &&
           floorAt(r+x,c+y+1) && floorAt(r+x+1,c+y+1)) return true;
    return false;
}
int main() {
    if(scanf("%d%d",&n,&m)!=2) return 0;
    for(int r=1;r<=n;++r) scanf("%s",a[r]+1);
    queue< pair<int,int> > q; q.push(make_pair(0,0)); outside[0][0]=true;
    int r=-1,c=-1,d=0;
    while(!q.empty() && r<0) {
        int x=q.front().first,y=q.front().second;q.pop();
        for(int k=0;k<4;++k) {
            int u=x+dr[k],v=y+dc[k];
            if(!floorAt(u,v) || outside[u][v]) continue;
            if((!floorAt(u-1,v) && !floorAt(u+1,v)) ||
               (!floorAt(u,v-1) && !floorAt(u,v+1))) {
                r=u;c=v;d=k;break;
            }
            outside[u][v]=true;q.push(make_pair(u,v));
        }
    }
    if(r<0) { puts("NO");return 0; }
    int sr=r,sc=c;
    while(!seen[r][c][d]) {
        seen[r][c][d]=true;
        int k;
        for(k=0;k<4;++k) {
            int nd=(d+3+k)%4;
            if(floorAt(r+dr[nd],c+dc[nd])) { d=nd;break; }
        }
        if(k==4) break;
        r+=dr[d];c+=dc[d];
        if(r==sr && c==sc) break;
        if(courtyard(r,c)) { puts("YES");return 0; }
    }
    puts("NO");return 0;
}
