// POJ 1643 - Checker's Check
// Model: gpt-6.1-sol
// Simulate every recorded move on the numbered board. Captures are mandatory,
// jump chains must finish, and reaching the promotion row ends the turn.
// Captured pieces are removed immediately, so no piece can be jumped twice.
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std;
int b[8][8], rr[33], cc[33];
bool inside(int r,int c){return r>=0&&r<8&&c>=0&&c<8;}
bool jump(int r,int c){
 int p=b[r][c];
 for(int dr=-1;dr<=1;dr+=2) for(int dc=-1;dc<=1;dc+=2){
  if(abs(p)==1&&dr!=(p>0?1:-1))continue;
  int x=r+2*dr,y=c+2*dc;
  if(inside(x,y)&&b[x][y]==0&&b[r+dr][c+dc]*p<0)return true;
 }
 return false;
}
bool valid(const vector<int>&v,int side){
 if(v.size()<2)return false;
 for(unsigned i=0;i<v.size();++i)if(v[i]<1||v[i]>32)return false;
 int r=rr[v[0]],c=cc[v[0]],p=b[r][c];
 if(p*side<=0)return false;
 bool must=false;
 for(int x=0;x<8;++x)for(int y=0;y<8;++y)if(b[x][y]*side>0&&jump(x,y))must=true;
 bool capturing=abs(rr[v[1]]-r)==2;
 if(!capturing&&(must||v.size()!=2))return false;
 bool promoted=false;
 for(unsigned i=1;i<v.size();++i){
  int x=rr[v[i]],y=cc[v[i]],dr=x-r,dc=y-c;
  int step=capturing?2:1;
  if(promoted||abs(dr)!=step||abs(dc)!=step||b[x][y])return false;
  if(abs(p)==1&&(dr*side<=0))return false;
  if(capturing){if(b[(x+r)/2][(y+c)/2]*p>=0)return false;b[(x+r)/2][(y+c)/2]=0;}
  b[r][c]=0;b[x][y]=p;r=x;c=y;
  if(abs(p)==1&&r==(side>0?7:0)){p=2*side;b[r][c]=p;promoted=true;}
 }
 return !capturing||promoted||!jump(r,c);
}
int main(){
 for(int n=1;n<=32;++n){rr[n]=(n-1)/4;cc[n]=2*((n-1)%4)+(rr[n]%2==0?1:0);}
 int nr,nw;
 while(cin>>nr>>nw&&(nr||nw)){
  for(int r=0;r<8;++r)for(int c=0;c<8;++c)b[r][c]=0;
  for(int side=1;side>=-1;side-=2)for(int i=0;i<(side==1?nr:nw);++i){int n;cin>>n;b[rr[abs(n)]][cc[abs(n)]]=side*(n<0?2:1);}
  int m;char turn;cin>>m>>turn;string line;getline(cin,line);int bad=0;
  for(int k=1;k<=m;++k){getline(cin,line);vector<int>v;int n=0;bool digits=false;
   for(unsigned i=0;i<=line.size();++i){if(i<line.size()&&line[i]>='0'&&line[i]<='9'){n=n*10+line[i]-'0';digits=true;}else if(digits){v.push_back(n);n=0;digits=false;}}
   if(!bad&&!valid(v,turn=='R'?1:-1))bad=k;
   turn=turn=='R'?'W':'R';
  }
  if(bad)cout<<"Move "<<bad<<" is invalid\n";else cout<<"All moves valid\n";
 }
}
