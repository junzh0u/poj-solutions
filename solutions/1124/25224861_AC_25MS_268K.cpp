// POJ 1124 - Oh, Those Achin' Feet
// Model: gpt-6.1-sol
// BFS counts shortest prefixes from each endpoint. Their product counts
// shortest routes through a street square, so its share is product/total.
// Only the route's endpoints and streets are traversable; other entrances
// are buildings. Output uses adjacent width-seven fields, as the board notes.
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;
int w,h, pos[15],ds[400],dt[400];
char grid[20][21];
double cs[400],ct[400],load[400];
bool valid(int v,int s,int t) {
    return grid[v/w][v%w]=='.' || v==s || v==t;
}
void bfs(int s,int t,int start,int *d,double *c) {
    for(int i=0;i<w*h;i++) {d[i]=-1;c[i]=0;}
    queue<int> q; q.push(start); d[start]=0;c[start]=1;
    while(!q.empty()) {
        int v=q.front();q.pop();
        int x=v%w,y=v/w;
        int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
        for(int k=0;k<4;k++) {
            int a=x+dx[k],b=y+dy[k];
            if(a<0||a>=w||b<0||b>=h) continue;
            int u=b*w+a;
            if(!valid(u,s,t)) continue;
            if(d[u]<0) {d[u]=d[v]+1;q.push(u);}
            if(d[u]==d[v]+1) c[u]+=c[v];
        }
    }
}
int main() {
    if(scanf("%d%d",&w,&h)!=2) return 0;
    for(int y=0;y<h;y++) {
        scanf("%s",grid[y]);
        for(int x=0;x<w;x++) if(grid[y][x]>='A'&&grid[y][x]<='O')
            pos[grid[y][x]-'A']=y*w+x;
    }
    char route[8];double amount;
    while(scanf("%s%lf",route,&amount)==2 && strcmp(route,"XX")!=0) {
        int s=pos[route[0]-'A'],t=pos[route[1]-'A'];
        bfs(s,t,s,ds,cs); bfs(s,t,t,dt,ct);
        for(int v=0;v<w*h;v++)
            if(grid[v/w][v%w]=='.'&&ds[v]>=0&&dt[v]>=0&&ds[v]+dt[v]==ds[t])
                load[v]+=amount*(cs[v]*ct[v]/cs[t]);
    }
    for(int y=0;y<h;y++) {
        for(int x=0;x<w;x++) printf("%7.2f",load[y*w+x]);
        printf("\n");
    }
    return 0;
}
