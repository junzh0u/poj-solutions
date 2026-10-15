// POJ 2788 - Black Box
// Model: gpt-6.1-sol
// Enumerate all five-atom subsets of the 6 by 6 interior and simulate
// each experiment. Absorption is tested before diagonal deflection;
// two diagonal atoms reverse the beam. Output only a unique layout.
// Gate numbering follows the statement figure, clockwise from the left.
#include <cstdio>
#include <cstring>
using namespace std;
bool atom[10][10], answer[10][10];
int dr[4]={0,1,0,-1},dc[4]={1,0,-1,0};
int entry[32],out[32],n,found;
int beam(int gate){
 int r,c,d;
 if(gate<8){r=gate+1;c=0;d=0;}
 else if(gate<16){r=9;c=gate-7;d=3;}
 else if(gate<24){r=24-gate;c=9;d=2;}
 else{r=0;c=32-gate;d=1;}
 for(int steps=0;steps<300;++steps){
  int nr=r+dr[d],nc=c+dc[d];
  if(nr==0)return 32-nc;
  if(nr==9)return nc+7;
  if(nc==0)return nr-1;
  if(nc==9)return 24-nr;
  if(atom[nr][nc])return -1;
  int left=(d+3)%4,right=(d+1)%4;
  bool a=atom[nr+dr[left]][nc+dc[left]];
  bool b=atom[nr+dr[right]][nc+dc[right]];
  if(a&&b)d=(d+2)%4;
  else if(a)d=right;
  else if(b)d=left;
  else{r=nr;c=nc;}
 }
 return -2;
}
void search(int start,int remaining){
 if(found>1)return;
 if(!remaining){
  for(int i=0;i<n;++i)if(beam(entry[i])!=out[i])return;
  ++found;memcpy(answer,atom,sizeof atom);return;
 }
 for(int p=start;p<=36-remaining;++p){
  int r=p/6+2,c=p%6+2;
  atom[r][c]=true;search(p+1,remaining-1);atom[r][c]=false;
  if(found>1)return;
 }
}
int main(){
 if(scanf("%d",&n)!=1)return 0;
 for(int i=0;i<n;++i)scanf("%d%d",&entry[i],&out[i]);
 search(0,5);
 if(found!=1)puts("NO");
 else for(int r=1;r<=8;++r){for(int c=1;c<=8;++c)putchar(answer[r][c]?'+':'-');putchar('\n');}
 return 0;
}
