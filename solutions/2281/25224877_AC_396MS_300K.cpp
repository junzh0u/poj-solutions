// POJ 2281 - Link and Pop -- the Block Game
// Model: gpt-6.1-sol
// Try link lengths in increasing order, then endpoints in row-major order.
// Empty border cells allow links outside the board, as in the figure.
// Movement scans a snapshot of block identities, once per checking turn.
#include <cstdio>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;
int n,m,b[32][32],rp[32][33],cp[33][32];
char sym[901],mov[901];
int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};
bool clearline(int x,int y,int u,int v){
 if(x==u){int l=min(y,v)+1,r=max(y,v);return rp[x][r]-rp[x][l]==0;}
 if(y==v){int l=min(x,u)+1,r=max(x,u);return cp[r][y]-cp[l][y]==0;}
 return false;
}
struct P{int x,y;P(int a=0,int c=0):x(a),y(c){}};
bool poppair(){
 P p[900];int k=0;
 for(int x=0;x<=n+1;x++){rp[x][0]=0;for(int y=0;y<=m+1;y++)rp[x][y+1]=rp[x][y]+(b[x][y]!=0);}
 for(int y=0;y<=m+1;y++){cp[0][y]=0;for(int x=0;x<=n+1;x++)cp[x+1][y]=cp[x][y]+(b[x][y]!=0);}
 for(int x=1;x<=n;x++)for(int y=1;y<=m;y++)if(b[x][y])p[k++]=P(x,y);
 for(int seg=1;seg<=3;seg++)for(int i=0;i<k;i++){
  bool reach[32][32];memset(reach,0,sizeof(reach));
  if(seg==3){
   int dist[32][32][2];memset(dist,0x3f,sizeof(dist));
   queue<P> q[4];q[0].push(p[i]);
   for(int s=1;s<=3;s++)while(!q[s-1].empty()){
    P a=q[s-1].front();q[s-1].pop();
    for(int d=0;d<4;d++){
     int x=a.x+dx[d],y=a.y+dy[d],o=d/2;
     while(x>=0&&x<=n+1&&y>=0&&y<=m+1){
      if(b[x][y]){reach[x][y]=true;break;}
      if(dist[x][y][o]<s)break;
      if(dist[x][y][o]>s){dist[x][y][o]=s;q[s].push(P(x,y));}
      x+=dx[d];y+=dy[d];
     }
    }
   }
  }
  for(int j=i+1;j<k;j++)if(sym[b[p[i].x][p[i].y]]==sym[b[p[j].x][p[j].y]]){
   int x=p[i].x,y=p[i].y,u=p[j].x,v=p[j].y;
   bool ok=seg==1?clearline(x,y,u,v):seg==3?reach[u][v]:
    ((!b[x][v]&&clearline(x,y,x,v)&&clearline(x,v,u,v))||(!b[u][y]&&clearline(x,y,u,y)&&clearline(u,y,u,v)));
   if(ok){b[x][y]=b[u][v]=0;return true;}
  }
 }
 return false;
}
void settle(){
 bool changed;
 do{changed=false;bool seen[901]={false};
  for(int x=1;x<=n;x++)for(int y=1;y<=m;y++){
   int id=b[x][y];if(!id||seen[id])continue;seen[id]=true;
   int d=mov[id]=='U'?0:mov[id]=='D'?1:mov[id]=='L'?2:mov[id]=='R'?3:4;
   if(d==4)continue;int u=x+dx[d],v=y+dy[d];
   if(u>=1&&u<=n&&v>=1&&v<=m&&!b[u][v]){b[u][v]=id;b[x][y]=0;changed=true;}
  }
 }while(changed);
}
int main(){int tc=0;while(scanf("%d%d",&n,&m)==2&&(n||m)){
 memset(b,0,sizeof(b));int id=0;char s[8];
 for(int x=1;x<=n;x++)for(int y=1;y<=m;y++){scanf("%s",s);b[x][y]=++id;sym[id]=s[0];mov[id]=s[1];}
 while(poppair())settle();
 printf("Case %d\n",++tc);for(int x=1;x<=n;x++){for(int y=1;y<=m;y++)putchar(b[x][y]?sym[b[x][y]]:'.');puts("");}
}return 0;}
