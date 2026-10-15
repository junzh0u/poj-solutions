// POJ 2310 - Cubic Tick-Tack-Toe
// Model: gpt-6.1-sol
// Negamax with alpha-beta pruning and immediate win/forced-block detection.
// Generate all 49 straight lines; legal moves are the lowest empty cells.
#include <cstdio>
#include <vector>
using namespace std;
typedef unsigned int U;
vector<U> lines[27];
int height[9], order[9]={4,0,2,6,8,1,3,5,7};
bool wins(U p,int cell){
 for(unsigned i=0;i<lines[cell].size();++i) if((p&lines[cell][i])==lines[cell][i]) return true;
 return false;
}
int solve(U me,U him,int left,int alpha,int beta){
 if(!left) return 0;
 int block=-1,nblock=0,cell[9];
 for(int i=0;i<9;++i){
  int c=order[i]; cell[i]=height[c]<3?c+9*height[c]:-1;
  if(cell[i]<0) continue;
  U b=1U<<cell[i];
  if(wins(me|b,cell[i])) return 1;
  if(wins(him|b,cell[i])){block=i;++nblock;}
 }
 if(nblock>1) return -1;
 int best=-1;
 for(int i=0;i<9;++i){
  if(cell[i]<0 || (nblock && i!=block)) continue;
  int c=order[i]; ++height[c];
  int v=-solve(him,me|(1U<<cell[i]),left-1,-beta,-alpha);
  --height[c];
  if(v>best) best=v;
  if(v>alpha) alpha=v;
  if(alpha>=beta) break;
 }
 return best;
}
int main(){
 for(int z=0;z<3;++z)for(int y=0;y<3;++y)for(int x=0;x<3;++x)
 for(int dz=-1;dz<=1;++dz)for(int dy=-1;dy<=1;++dy)for(int dx=-1;dx<=1;++dx){
  if(dz<0 || (dz==0 && dy<0) || (dz==0 && dy==0 && dx<=0)) continue;
  int xx=x+2*dx,yy=y+2*dy,zz=z+2*dz;
  if(xx<0||xx>2||yy<0||yy>2||zz<0||zz>2) continue;
  U m=0;int p[3];for(int k=0;k<3;++k){p[k]=(z+k*dz)*9+(y+k*dy)*3+x+k*dx;m|=1U<<p[k];}
  for(int k=0;k<3;++k) lines[p[k]].push_back(m);
 }
 char s[32];
 while(scanf("%s",s)==1){
  U o=0,x=0;int no=0,nx=0;
  for(int c=0;c<9;++c) height[c]=0;
  for(int r=0;r<9;++r){
   if(r && scanf("%s",s)!=1) return 0;
   for(int j=0;j<3;++j){int p=r*3+j;
    if(s[j]=='O'){o|=1U<<p;++no;++height[p%9];}
    if(s[j]=='X'){x|=1U<<p;++nx;++height[p%9];}
   }
  }
  U me=no==nx?o:x,him=no==nx?x:o;
  int v=0;bool done=false;
  for(int p=0;p<27;++p) if((him&(1U<<p))&&wins(him,p)){v=-1;done=true;break;}
  if(!done) for(int p=0;p<27;++p) if((me&(1U<<p))&&wins(me,p)){v=1;done=true;break;}
  if(!done) v=solve(me,him,27-no-nx,-1,1);
  puts(v==1?"WIN":v==-1?"LOSE":"DRAW");
 }
}
