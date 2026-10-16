// POJ 4034 - Alphabet Soup
// Model: gpt-6.1-sol
// Sort the positions and find the minimal period of their circular gaps.
// Exactly the shifts by that period preserve the position set. Burnside's
// lemma averages S^gcd(P, shift) over this cyclic rotation group.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int MOD=100000007;
int power(int a,int b){long long r=1,x=a;while(b){if(b&1)r=r*x%MOD;x=x*x%MOD;b>>=1;}return (int)r;}
int gcd(int a,int b){while(b){int t=a%b;a=b;b=t;}return a;}
int main(){int s,n;while(scanf("%d%d",&s,&n)==2 && s!=-1){vector<int>a(n),g(n),pi(n,0);for(int i=0;i<n;++i)scanf("%d",&a[i]);sort(a.begin(),a.end());for(int i=0;i<n;++i)g[i]=(i+1<n?a[i+1]:a[0]+360000)-a[i];for(int i=1;i<n;++i){int j=pi[i-1];while(j && g[i]!=g[j])j=pi[j-1];if(g[i]==g[j])++j;pi[i]=j;}int d=n-pi[n-1];if(n%d)d=n;int m=n/d;vector<int> pw(n+1,1);for(int i=1;i<=n;++i)pw[i]=(long long)pw[i-1]*s%MOD;long long ans=0;for(int k=0;k<m;++k)ans=(ans+pw[gcd(n,k*d)])%MOD;printf("%d\n",(int)(ans*power(m,MOD-2)%MOD));}return 0;}
