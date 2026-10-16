// POJ 3810 - Hobby on Rails
// Model: gpt-6.1-sol
// Grow all switch-connected components, assigning rotations only as needed.
// Unused ordinary rails need not be connected. Compress paths between switches
// and find weighted cycles in the finite switch-state transition graph.
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
int w,h,n,k,ans,typ[36],sid[36],pos[6],rot[36],nb[36][4];
int maskv[36][4],opts[36];
int dst[6][4],entry[6][4],len[6][4],nxt[1536],weight[1536],seen[1536],depthv[1536];
int port(int r,int t){return (r+t)%4;}
void evaluate(){
 for(int s=0;s<k;s++)for(int d=0;d<4;d++)if(maskv[pos[s]][rot[pos[s]]]&(1<<d)){
  int p=nb[pos[s]][d],in=(d+2)%4,l=1;
  while(sid[p]<0){int m=maskv[p][rot[p]]^(1<<in),out=0;while(!(m&(1<<out)))out++;p=nb[p][out];in=(out+2)%4;l++;}
  dst[s][d]=sid[p];entry[s][d]=in;len[s][d]=l;
 }
 int total=k*4*(1<<k);memset(seen,0,sizeof(seen));
 for(int bits=0;bits<(1<<k);bits++)for(int s=0;s<k;s++)for(int in=0;in<4;in++){
  int id=(bits*k+s)*4+in;nxt[id]=-1;
  int p=pos[s],r=rot[p];if(!(maskv[p][r]&(1<<in)))continue;
  int b=port(r,3),straight=port(r,1),curve=port(r,typ[p]==2?0:2),out=b,bs=bits;
  if(in==b){out=(bits&(1<<s))?curve:straight;bs^=1<<s;}
  nxt[id]=(bs*k+dst[s][out])*4+entry[s][out];weight[id]=len[s][out];
 }
 for(int start=0;start<total;start++)if(!seen[start]&&nxt[start]>=0){
  int u=start,t=0;while(!seen[u]){seen[u]=start+1;depthv[u]=t;t+=weight[u];u=nxt[u];}
  if(seen[u]==start+1)ans=max(ans,t-depthv[u]);
 }
}
bool fits(int p,int r){int m=maskv[p][r];for(int d=0;d<4;d++){int q=nb[p][d];if(q<0){if(m&(1<<d))return false;}else if(rot[q]>=0){if(bool(m&(1<<d))!=bool(maskv[q][rot[q]]&(1<<((d+2)%4))))return false;}}return true;}
void search(){
 int target=-1;
 for(int p=0;p<n&&target<0;p++)if(rot[p]>=0)for(int d=0;d<4;d++)if(maskv[p][rot[p]]&(1<<d)){int q=nb[p][d];if(rot[q]<0){target=q;break;}}
 if(target<0)for(int s=0;s<k;s++)if(rot[pos[s]]<0){target=pos[s];break;}
 if(target<0){evaluate();return;}
 for(int r=0;r<opts[target];r++)if(fits(target,r)){rot[target]=r;search();rot[target]=-1;}
}
int main(){while(cin>>w>>h,w||h){n=w*h;k=0;ans=0;for(int p=0;p<n;p++){char c;cin>>c;typ[p]=c=='S'?0:c=='C'?1:c=='L'?2:3;sid[p]=-1;if(typ[p]>=2){sid[p]=k;pos[k++]=p;}opts[p]=typ[p]==0?2:4;rot[p]=-1;for(int r=0;r<opts[p];r++){int m=typ[p]==0?5:typ[p]==1?9:typ[p]==2?11:14;maskv[p][r]=((m<<r)|(m>>(4-r)))&15;}int y=p/w,x=p%w;nb[p][0]=y? p-w:-1;nb[p][1]=x+1<w?p+1:-1;nb[p][2]=y+1<h?p+w:-1;nb[p][3]=x?p-1:-1;}search();cout<<ans<<'\n';}}
