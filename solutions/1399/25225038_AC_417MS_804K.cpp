// POJ 1399 - Direct Visibility
// Model: gpt-6.1-sol
// Trace each sight line through open grid-cell interiors with exact rational
// boundary times. Touching terrain or a cell corner does not block visibility.
// Breadth-first search uses visible cells and directed height-limited steps.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;
int h[200][200],distances[200][200],p,q;
bool visible(int a,int b,int x,int y) {
 int dx=x-a,dy=y-b,ax=abs(dx),ay=abs(dy);
 int sx=(dx>0?1:-1),sy=(dy>0?1:-1);
 int nx=1,ny=1,px=2*ax,py=2*ay;
 int ln=0,ld=1,z=2*h[a][b]+1,dz=2*(h[x][y]-h[a][b]);
 int r=a,c=b;
 while(true) {
  int rn=1,rd=1;
  if(ax && nx<px && nx*rd<rn*px) {rn=nx;rd=px;}
  if(ay && ny<py && ny*rd<rn*py) {rn=ny;rd=py;}
  int mn=(dz<0?rn:ln),md=(dz<0?rd:ld);
  if(z*md+dz*mn<2*h[r][c]*md) return false;
  if(rn==rd) return true;
  bool cx=ax && nx*rd==rn*px;
  bool cy=ay && ny*rd==rn*py;
  if(cx) {r+=sx;nx+=2;}
  if(cy) {c+=sy;ny+=2;}
  ln=rn;ld=rd;
 }
}
int main() {
 int t;scanf("%d",&t);
 while(t--) {
  scanf("%d%d",&p,&q);
  for(int i=0;i<p;i++) for(int j=0;j<q;j++) scanf("%d",&h[i][j]);
  int a,b,x,y;scanf("%d%d%d%d",&a,&b,&x,&y);--a;--b;--x;--y;
  bool ok[200][200];
  for(int i=0;i<p;i++) for(int j=0;j<q;j++) ok[i][j]=visible(a,b,i,j)||visible(x,y,i,j);
  int qr[40000],qc[40000],head=0,tail=0;
  memset(distances,-1,sizeof(distances));distances[a][b]=0;
  qr[tail]=a;qc[tail++]=b;
  int dr[4]={1,-1,0,0},dc[4]={0,0,1,-1};
  while(head<tail) {
   int r=qr[head],c=qc[head++];
   for(int k=0;k<4;k++) {
    int nr=r+dr[k],nc=c+dc[k];
    if(nr<0||nr>=p||nc<0||nc>=q||!ok[nr][nc]||distances[nr][nc]>=0) continue;
    int diff=h[nr][nc]-h[r][c];if(diff>1||diff< -3) continue;
    distances[nr][nc]=distances[r][c]+1;qr[tail]=nr;qc[tail++]=nc;
   }
  }
  if(distances[x][y]<0) puts("Mission impossible!");
  else printf("The shortest path is %d steps long.\n",distances[x][y]);
 }
 return 0;
}
