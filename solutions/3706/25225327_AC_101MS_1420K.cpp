// POJ 3706 - System Test
// Model: gpt-6.1-sol
// Compute each player's attainable scores by subset sum. For each partition,
// place excluded players at their minimum scores, then wildcard players at
// their smallest scores above all excluded players. The unique largest key
// in each of those two groups identifies its canonical boundary pair.
// Count assignments for every boundary pair by a two-dimensional DP.
// Rank keys incorporate SEED, including for negative challenge scores.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
typedef unsigned long long ULL;
int main(){
 int n,m,c1,c2;
 while(cin>>n>>m>>c1>>c2){
  vector<int> keys[40]; int lo[40],hi[40];
  for(int i=0;i<n;i++){
   int seed,a[40],sum=0,c; cin>>seed;
   for(int j=0;j<m;j++){cin>>a[j];sum+=a[j];} cin>>c;
   vector<char> ok(sum+1,0);ok[0]=1;int cur=0;
   for(int j=0;j<m;j++){for(int k=cur;k>=0;k--)if(ok[k])ok[k+a[j]]=1;cur+=a[j];}
   for(int s=0;s<=sum;s++)if(ok[s])keys[i].push_back((s+c)*n+n-seed);
   lo[i]=keys[i].front();hi[i]=keys[i].back();
  }
  ULL ans=0,dp[41][41],nd[41][41];
  for(int l=0;l<n;l++){
   int t=lo[l],next[40];
   for(int i=0;i<n;i++){
    vector<int>::iterator it=upper_bound(keys[i].begin(),keys[i].end(),t);
    next[i]=it==keys[i].end()?2000000000:*it;
   }
   for(int w=0;w<n;w++)if(w!=l && next[w]!=2000000000){
    int u=next[w];memset(dp,0,sizeof(dp));dp[0][1]=1;
    for(int i=0;i<n;i++)if(i!=l && i!=w){
     memset(nd,0,sizeof(nd));
     for(int f=0;f<=c1;f++)for(int b=0;b<=c2;b++)if(dp[f][b]){
      if(lo[i]<=t)nd[f][b]+=dp[f][b];
      if(next[i]<=u && b<c2)nd[f][b+1]+=dp[f][b];
      if(hi[i]>u && f<c1)nd[f+1][b]+=dp[f][b];
     }
     memcpy(dp,nd,sizeof(dp));
    }
    ans+=dp[c1][c2];
   }
  }
  cout<<ans<<'\n';
 }
 return 0;
}
