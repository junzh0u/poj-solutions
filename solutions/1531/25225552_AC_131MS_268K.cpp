// POJ 1531 - Trooper of Bam
// Model: gpt-6.1-sol
// Enumerate fixed straight patrol segments consistent with all sightings and
// non-sightings, then cover their possible stopped positions by coordinate ranges.
// Sightings are complete; motion stops at time s, even within the observations.
// The new route starts at the last recorded position of Salman.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int ax[105],ay[105],ox[75][105],oy[75][105];
bool possible[105][105];
int pos(int a,int b,int z,int dt) {
 int len=b-a, v=(z+dt)%(2*len); if(v<0)v+=2*len;
 return a+(v<=len?v:2*len-v);
}
int travel(int l,int r,int start){return r-l+min(start-l,r-start);}
int main(){
 int n,p,s,X,Y,o;
 while(scanf("%d%d%d%d%d%d",&n,&p,&s,&X,&Y,&o)==6 && (n||p||s||X||Y||o)){
  for(int t=1;t<=p;t++)scanf("%d%d",&ax[t],&ay[t]);
  memset(ox,0,sizeof(ox));memset(oy,0,sizeof(oy));
  for(int k=0;k<o;k++){int id,t,x,y;scanf("%d%d%d%d",&id,&t,&x,&y);ox[id][t]=x;oy[id][t]=y;}
  memset(possible,0,sizeof(possible));
  for(int id=1;id<=n;id++){
   int t0=1;while(t0<=p&&!ox[id][t0])t0++;
   if(t0>p)continue;
   for(int axis=0;axis<2;axis++){
    int fixed=axis?ox[id][t0]:oy[id][t0];
    int anchor=axis?oy[id][t0]:ox[id][t0], bound=axis?Y:X;
    for(int a=1;a<=anchor;a++)for(int b=max(a+1,anchor);b<=bound;b++)for(int dir=0;dir<2;dir++){
     int z=dir?2*(b-a)-(anchor-a):anchor-a;
     bool ok=true;
     for(int t=1;t<=p && ok;t++){
      int v=pos(a,b,z,min(t,s)-min(t0,s));
      int x=axis?fixed:v,y=axis?v:fixed;
      if(ox[id][t])ok=(x==ox[id][t]&&y==oy[id][t]);
      else ok=(x!=ax[t]&&y!=ay[t]);
     }
     if(ok){int v=pos(a,b,z,s-min(t0,s));possible[axis?fixed:v][axis?v:fixed]=true;}
    }
   }
  }
  int lo[105],hi[105];
  for(int x=1;x<=X;x++){lo[x]=Y+1;hi[x]=0;for(int y=1;y<=Y;y++)if(possible[x][y]){lo[x]=min(lo[x],y);hi[x]=max(hi[x],y);}}
  int ans=100000,sx=ax[p],sy=ay[p];
  for(int l=1;l<=sx;l++)for(int r=sx;r<=X;r++){
   int bot=sy,top=sy;
   for(int x=1;x<=X;x++)if(x<l||x>r){bot=min(bot,lo[x]);top=max(top,hi[x]);}
   ans=min(ans,travel(l,r,sx)+travel(bot,top,sy));
  }
  printf("%d\n",ans);
 }
 return 0;
}
