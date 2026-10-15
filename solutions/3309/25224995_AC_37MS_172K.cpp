// POJ 3309 - Unlucky Luke!
// Model: gpt-6.1-sol
// Sort by value density. Before the first bag is filled, all earlier stones
// can be taken whole. Subset sums enumerate their distribution between bags;
// cut the current stone to close one bag, then greedily fill the other.
// A selected portion belongs to one bag; discarded remainders are unavailable.
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;
struct Stone { int w; double v; };
bool cmp(const Stone &a,const Stone &b) { return a.v*b.w>b.v*a.w; }
int main() {
 int T; scanf("%d",&T);
 while(T--) {
  int n; double V; scanf("%d%lf",&n,&V);
  vector<Stone> a(n); int i,j;
  for(i=0;i<n;i++) scanf("%d",&a[i].w);
  for(i=0;i<n;i++) scanf("%lf",&a[i].v);
  sort(a.begin(),a.end(),cmp);
  vector<int> w(n+1,0); vector<double> val(n+1,0);
  for(i=0;i<n;i++) {w[i+1]=w[i]+a[i].w;val[i+1]=val[i]+a[i].v;}
  int cap=(int)floor(V+1e-9); vector<char> dp(cap+1,0); dp[0]=1;
  double ans=0;
  for(i=0;i<n;i++) {
   for(j=0;j<=cap;j++) if(dp[j] && w[i]-j<=V+1e-9) {
    double cut=V-j;
    if(cut>a[i].w+1e-9) continue;
    double remain=V-(w[i]-j);
    int lo=i+1,hi=n;
    while(lo<hi) {int mid=(lo+hi+1)/2; if(w[mid]-w[i+1]<=remain+1e-9) lo=mid; else hi=mid-1;}
    double cur=val[i]+cut*a[i].v/a[i].w+val[lo]-val[i+1];
    remain-=w[lo]-w[i+1];
    if(lo<n) cur+=remain*a[lo].v/a[lo].w;
    ans=max(ans,cur);
   }
   for(j=cap;j>=a[i].w;j--) if(dp[j-a[i].w]) dp[j]=1;
   if(w[i+1]<=2*V+1e-9) for(j=0;j<=cap;j++) if(dp[j] && w[i+1]-j<=V+1e-9) ans=max(ans,val[i+1]);
  }
  printf("%.4f\n",ans);
 }
 return 0;
}
