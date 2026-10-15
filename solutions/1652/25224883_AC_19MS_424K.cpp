// POJ 1652 - Holey Cloth
// Model: gpt-6.1-sol
// Label each cloth piece, then count bounded components of its complement.
// Other pieces are traversable when examining a piece: nested cloth does not
// split its hole. Both cloth connectivity and hole connectivity use four sides.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int w,h,id[102][102],seen[102][102],qx[10404],qy[10404];
char a[102][102];
int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
int main(){
 if(scanf("%d%d",&w,&h)!=2)return 0;
 for(int i=1;i<=h;i++)scanf("%s",a[i]+1);
 int cnt=0,best=0,answer=0,stamp=0;
 for(int i=1;i<=h;i++)for(int j=1;j<=w;j++)if(a[i][j]=='*'&&!id[i][j]){
  ++cnt;int head=0,tail=1;qx[0]=i;qy[0]=j;id[i][j]=cnt;
  int xlo=i,xhi=i,ylo=j,yhi=j;
  while(head<tail){int x=qx[head],y=qy[head++];
   xlo=min(xlo,x);xhi=max(xhi,x);ylo=min(ylo,y);yhi=max(yhi,y);
   for(int d=0;d<4;d++){int nx=x+dx[d],ny=y+dy[d];
    if(nx>=1&&nx<=h&&ny>=1&&ny<=w&&a[nx][ny]=='*'&&!id[nx][ny]){
     id[nx][ny]=cnt;qx[tail]=nx;qy[tail++]=ny;
    }
   }
  }
  int area=tail;if(area<7)continue;
  --xlo;++xhi;--ylo;++yhi;++stamp;int regions=0;
  for(int x=xlo;x<=xhi;x++)for(int y=ylo;y<=yhi;y++)if(id[x][y]!=cnt&&seen[x][y]!=stamp){
   ++regions;head=0;tail=1;qx[0]=x;qy[0]=y;seen[x][y]=stamp;
   while(head<tail){int cx=qx[head],cy=qy[head++];
    for(int d=0;d<4;d++){int nx=cx+dx[d],ny=cy+dy[d];
     if(nx>=xlo&&nx<=xhi&&ny>=ylo&&ny<=yhi&&id[nx][ny]!=cnt&&seen[nx][ny]!=stamp){
      seen[nx][ny]=stamp;qx[tail]=nx;qy[tail++]=ny;
     }
    }
   }
  }
  int holes=regions-1;
  if(holes>best||(holes==best&&holes>0&&area<answer)){best=holes;answer=area;}
 }
 printf("%d\n",answer);return 0;
}
