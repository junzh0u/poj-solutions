// POJ 4004 - My World Cup
// Model: gpt-6.1-sol
// Minimax over remaining card masks and the current goal difference.
// Each meeting combines independent scoring distributions for both cards.
// Equal speeds favor the home team; a tied extra time has win chance 1/2.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;
struct Card { int t,s,p; } a[5],b[5];
struct Outcome { int d; double p; } out[5][5][4];
int cnt[5][5];
double dp[16][16][81],extra;
int seen[16][16][81];
void score(Card x,Card y,int &g,double &p) {
 g=0;p=0;
 if(y.t==0) { p=19.0/81; return; }
 if(x.t==1) { int k=x.p-(y.t==2?y.p:0); p=k>0?1-pow(2.0/3,k):0; }
 if(x.t==2 && y.t==3) { int k=x.p-y.p; p=k>0?1-pow(2.0/3,k):0; }
 if(x.t==3) g=max(0,x.p-(y.t==2?y.p:0));
}
double solve(int m,int n,int d) {
 if(!m) return d>0?1:d<0?0:extra;
 double &ans=dp[m][n][d+40];
 if(seen[m][n][d+40]) return ans;
 seen[m][n][d+40]=1;
 int sa=0,sb=0,i,j,k;
 for(i=0;i<4;i++) {if(m>>i&1) sa=max(sa,a[i].s); if(n>>i&1) sb=max(sb,b[i].s);}
 bool home=sa>=sb; ans=home?0:1;
 for(i=0;i<4;i++) if(m>>i&1) for(j=0;j<4;j++) if(n>>j&1) {
  if(home?a[i].s!=sa:b[j].s!=sb) continue;
  double v=0;
  for(k=0;k<cnt[i][j];k++) v+=out[i][j][k].p*solve(m^(1<<i),n^(1<<j),d+out[i][j][k].d);
  ans=home?max(ans,v):min(ans,v);
 }
 return ans;
}
int main(){int T;scanf("%d",&T);while(T--){int i,j,x,y;
 for(i=0;i<5;i++)scanf("%d%d%d",&a[i].t,&a[i].s,&a[i].p);
 for(i=0;i<5;i++)scanf("%d%d%d",&b[i].t,&b[i].s,&b[i].p);
 for(i=0;i<5;i++)for(j=0;j<5;j++) {int ga,gb;double pa,pb;score(a[i],b[j],ga,pa);score(b[j],a[i],gb,pb);cnt[i][j]=0;
 for(x=0;x<2;x++)for(y=0;y<2;y++){Outcome &o=out[i][j][cnt[i][j]++];o.d=ga+x-gb-y;o.p=(x?pa:1-pa)*(y?pb:1-pb);}}
 extra=0;for(i=0;i<4;i++) extra+=out[4][4][i].p*(out[4][4][i].d>0?1:out[4][4][i].d<0?0:0.5);
 memset(seen,0,sizeof(seen));printf("%.0f\n",floor(solve(15,15,0)*100+0.5));
 }return 0;}
