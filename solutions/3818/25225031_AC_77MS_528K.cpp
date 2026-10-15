// POJ 3818 - Mosaic
// Model: gpt-6.1-sol
// Precompute column-profile transitions for 2x2 squares and all four
// L-tromino rotations, then cache every length up to 500 for each height.
// The figure's second tile occupies three unit cells, not a full square.
#include <cstdio>
#include <vector>
#include <utility>
#include <cstring>
using namespace std;
const int MOD=1000000;
int h, full, counts[1024], answer[11][501];
bool ready[11];
void fillcol(int cur,int next){
 if(cur==full){++counts[next];return;}
 int x=0;while(cur&(1<<x))++x;
 for(int r=x-1;r<=x;++r){
  if(r<0||r+1>=h)continue;
  for(int missing=-1;missing<4;++missing){
   int a=0,b=0;
   for(int k=0;k<4;++k)if(k!=missing){
    if(k<2)a|=1<<(r+k);else b|=1<<(r+k-2);
   }
   if(!(a&(1<<x))||(a&cur)||(b&next))continue;
   fillcol(cur|a,next|b);
  }
 }
}
void prepare(int n){
 h=n;full=(1<<h)-1;
 vector< vector< pair<int,int> > > tr(1<<h);
 for(int s=0;s<=full;++s){
  memset(counts,0,sizeof(counts));fillcol(s,0);
  for(int t=0;t<=full;++t)if(counts[t])tr[s].push_back(make_pair(t,counts[t]));
 }
 int dp[1024]={0},nd[1024];dp[0]=1;answer[n][0]=1;
 for(int len=1;len<=500;++len){
  memset(nd,0,sizeof(nd));
  for(int s=0;s<=full;++s)if(dp[s])
   for(size_t j=0;j<tr[s].size();++j){
    int t=tr[s][j].first;
    nd[t]=(nd[t]+(long long)dp[s]*tr[s][j].second)%MOD;
   }
  memcpy(dp,nd,sizeof(dp));answer[n][len]=dp[0];
 }
 ready[n]=true;
}
int main(){int n,m;while(scanf("%d%d",&n,&m)==2&&(n||m)){
 if(!ready[n])prepare(n);printf("%d\n",answer[n][m]);
}return 0;}
