// POJ 3828 - Zombies VS Plants
// Model: gpt-6.1-sol
// Mixed-radix dynamic programming over cleared rows, removed magnets,
// and counts of harvested sunflowers. Accessible sunflowers are interchangeable.
// Sunflower rewards are extra; attacks must strictly exceed defense.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
int main(){
 int cost[2][6];
 for(int w=0;w<2;++w){int d[702]; d[0]=0;for(int a=1;a<=701;++a)d[a]=100000;
  int v[3]={40,70,w?50:200},p[3]={50,75,100};
  for(int a=0;a<=501;++a)for(int z=0;z<3;++z)if(a+v[z]<=701)d[a+v[z]]=min(d[a+v[z]],d[a]+p[z]);
  for(int b=0;b<=5;++b){cost[w][b]=100000;for(int a=b*100+1;a<=701;++a)cost[w][b]=min(cost[w][b],d[a]);}
 }
 int T;scanf("%d",&T);while(T--){
 int P,Q,R,g[5][5],s[5]={0},m[5]={0},b[5]={0},l[5]={0},base[5],stride[5],dead[5];
 scanf("%d%d%d",&P,&Q,&R);
 for(int i=0;i<5;++i)for(int j=0;j<5;++j){scanf("%d",&g[i][j]);int t=g[i][j];s[i]+=t==1;m[i]+=t==3;b[i]+=t==2;l[i]+=t==4;}
 int n=1;for(int i=0;i<5;++i){stride[i]=n;dead[i]=(s[i]+1)*(m[i]?2:1);base[i]=dead[i]+1;n*=base[i];}
 int access[32][5],magok[32][5];
 for(int mask=0;mask<32;++mask)for(int i=0;i<5;++i){access[mask][i]=0;magok[mask][i]=1;
  for(int j=0;j<5;++j)if(g[i][j]==1||g[i][j]==3){bool ok=true;for(int x=max(0,i-1);x<=min(4,i+1);++x)if(!(mask&(1<<x)))for(int y=max(0,j-1);y<=min(4,j+1);++y)if(g[x][y]==4)ok=false;
   if(g[i][j]==1)access[mask][i]+=ok;else if(!ok)magok[mask][i]=0;
  }
 }
 vector<int> dp(n,-1);dp[0]=R;
 for(int st=0;st<n;++st)if(dp[st]>=0){int a[5],h[5],cl[5],mask=0;for(int i=0;i<5;++i){a[i]=st/stride[i]%base[i];if(a[i]==dead[i])mask|=1<<i;h[i]=a[i]%(s[i]+1);cl[i]=!m[i]||a[i]>=s[i]+1;}
 for(int i=0;i<5;++i)if(!(mask&(1<<i))){
  bool weak=false;for(int x=max(0,i-1);x<=min(4,i+1);++x)if(!(mask&(1<<x))&&!cl[x])weak=true;
  int c=cost[weak][b[i]],to=st+(dead[i]-a[i])*stride[i];
  if(dp[st]>=c)dp[to]=max(dp[to],dp[st]-c+P+(s[i]-h[i])*Q);
  if(h[i]<access[mask][i]&&dp[st]>=125){int add=1,bonus=Q;
   if(h[i]+1==s[i]&&cl[i]&&!b[i]&&!l[i]){add=dead[i]-a[i];bonus+=P;}
   int t=st+add*stride[i];dp[t]=max(dp[t],dp[st]-125+bonus);
  }
  if(!cl[i]&&magok[mask][i]&&dp[st]>=125*m[i]){int add=s[i]+1,bonus=0;
   if(h[i]==s[i]&&!b[i]&&!l[i]){add=dead[i]-a[i];bonus=P;}
   int t=st+add*stride[i];dp[t]=max(dp[t],dp[st]-125*m[i]+bonus);
  }
 }
 }
 printf("%d\n",dp[n-1]);
 }
 return 0;
}
