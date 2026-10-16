// POJ 1713 - Divide et unita
// Model: gpt-6-astra
// Enumerate rigid placements of the two pieces in the square. A has
// identity orientation without loss of generality, and contains cell 0.
// Each doubly covered target cell equates two source-cell labels; singly
// covered cells force labels. Enumerate the remaining components and
// explicitly require both nonempty pieces to be connected. No reflections.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;
int n,m,x[25],y[25],p[25],forcev[25],amap[25],bmap[25];
unsigned adj[25],full,answer;
int root(int a){return p[a]==a?a:p[a]=root(p[a]);}
bool connected(unsigned s){
 if(!s)return false;
 unsigned seen=s&(~s+1),old=0;
 while(old!=seen){old=seen;for(int i=0;i<m;i++)if(seen&(1u<<i))seen|=adj[i]&s;}
 return seen==s;
}
bool check(){
 for(int i=0;i<m;i++){p[i]=i;forcev[i]=-1;}
 for(int j=0;j<m;j++){
  if(amap[j]<0 && bmap[j]<0)return false;
  if(amap[j]>=0 && bmap[j]>=0)p[root(amap[j])]=root(bmap[j]);
 }
 unsigned allowedA=0,allowedB=0;
 for(int j=0;j<m;j++){
  if(amap[j]>=0)allowedA|=1u<<amap[j];
  if(bmap[j]>=0)allowedB|=1u<<bmap[j];
 }
 for(int i=0;i<m;i++){
  int f=-1;
  if(!(allowedA&(1u<<i)))f=1;
  if(!(allowedB&(1u<<i))){if(f==1)return false;f=0;}
  if(i==0){if(f==1)return false;f=0;}
  if(f>=0){int r=root(i);if(forcev[r]>=0&&forcev[r]!=f)return false;forcev[r]=f;}
 }
 unsigned masks[25]={0},freeMasks[25],base=0;int k=0;
 for(int i=0;i<m;i++)masks[root(i)]|=1u<<i;
 for(int i=0;i<m;i++)if(masks[i]){
  if(forcev[i]==1)base|=masks[i];
  else if(forcev[i]<0)freeMasks[k++]=masks[i];
 }
 unsigned b=base,prev=0;
 for(unsigned z=0;z<(1u<<k);z++){
  unsigned gray=z^(z>>1),change=gray^prev;
  if(change){int j=0;while(!(change&(1u<<j)))j++;b^=freeMasks[j];}
  prev=gray;
  if(connected(b)&&connected(full^b)){answer=b;return true;}
 }
 return false;
}
bool solve(){
 for(int ax=0;ax<n;ax++)for(int ay=0;ay<n;ay++){
  fill(amap,amap+m,-1);
  for(int i=0;i<m;i++){int a=x[i]-x[0]+ax,b=y[i]-y[0]+ay;if(a>=0&&a<n&&b>=0&&b<n)amap[a*n+b]=i;}
  for(int rot=0;rot<4;rot++){
   int u[25],v[25],loX=1000,hiX=-1000,loY=1000,hiY=-1000;
   for(int i=0;i<m;i++){
    u[i]=x[i]-x[0];v[i]=y[i]-y[0];
    for(int r=0;r<rot;r++){int t=u[i];u[i]=-v[i];v[i]=t;}
    loX=min(loX,u[i]);hiX=max(hiX,u[i]);loY=min(loY,v[i]);hiY=max(hiY,v[i]);
   }
   for(int dx=-hiX;dx<n-loX;dx++)for(int dy=-hiY;dy<n-loY;dy++){
    fill(bmap,bmap+m,-1);
    for(int i=0;i<m;i++){int a=u[i]+dx,b=v[i]+dy;if(a>=0&&a<n&&b>=0&&b<n)bmap[a*n+b]=i;}
    if(check())return true;
   }
  }
 }
 return false;
}
int main(){
 vector<string> lines;string s;
 while(getline(cin,s)){if(!s.empty()&&s[s.size()-1]=='\r')s.erase(s.size()-1);lines.push_back(s);}
 for(int i=0;i<(int)lines.size();i++)for(int j=0;j<(int)lines[i].size();j++)if(lines[i][j]=='*'){x[m]=i;y[m++]=j;}
 n=2;while(n*n<m)n++;full=(1u<<m)-1;
 for(int i=0;i<m;i++)for(int j=0;j<m;j++)if(abs(x[i]-x[j])+abs(y[i]-y[j])==1)adj[i]|=1u<<j;
 if(!solve())return 1;
 for(int i=0;i<m;i++)lines[x[i]][y[i]]=(answer&(1u<<i))?'B':'A';
 for(int i=0;i<(int)lines.size();i++)cout<<lines[i]<<'\n';
 return 0;
}
