// POJ 1368 - Puzzle
// Model: gpt-6.1-sol
// Count identical oriented pieces and search the most constrained empty cell.
// Flat sides must coincide exactly with the rectangle boundary; rotations are forbidden.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
int n,m,N,side[81][4],cnt[81],board[36],nb[36][4];
vector<int> base[36];
bool legal(int p,int t){
 for(int d=0;d<4;++d) if(nb[p][d]>=0 && board[nb[p][d]]>=0 && side[t][d]+side[board[nb[p][d]]][(d+2)%4]!=3)return false;
 return true;
}
bool dfs(int left){
 if(!left)return true;
 int best=-1,small=82,capacity[81]={0};
 vector<int> choices[36];
 for(int p=0;p<N;++p)if(board[p]<0){
  for(unsigned j=0;j<base[p].size();++j){int t=base[p][j];if(cnt[t]&&legal(p,t)){choices[p].push_back(t);++capacity[t];}}
  int sz=choices[p].size(); if(!sz)return false;
  if(sz<small){small=sz;best=p;}
 }
 for(int t=0;t<81;++t)if(capacity[t]<cnt[t])return false;
 for(unsigned j=0;j<choices[best].size();++j){int t=choices[best][j];board[best]=t;--cnt[t];if(dfs(left-1))return true;++cnt[t];board[best]=-1;}
 return false;
}
int main(){
 for(int t=0;t<81;++t){int x=t;for(int d=3;d>=0;--d){side[t][d]=x%3;x/=3;}}
 while(scanf("%d%d",&n,&m)==2 && n && m){
  N=n*m;memset(cnt,0,sizeof(cnt));
  for(int i=0;i<N;++i){char s[8];scanf("%s",s);int t=0;for(int d=0;d<4;++d)t=t*3+(s[d]=='F'?0:s[d]=='I'?1:2);++cnt[t];}
  for(int p=0;p<N;++p){int r=p/m,c=p%m;board[p]=-1;base[p].clear();nb[p][0]=r?p-m:-1;nb[p][1]=c+1<m?p+1:-1;nb[p][2]=r+1<n?p+m:-1;nb[p][3]=c?p-1:-1;
   for(int t=0;t<81;++t)if(cnt[t]){bool ok=true;for(int d=0;d<4;++d)if((side[t][d]==0)!=(nb[p][d]<0))ok=false;if(ok)base[p].push_back(t);}
  }
  puts(dfs(N)?"YES":"NO");
 }
 return 0;
}
