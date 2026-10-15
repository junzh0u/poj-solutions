// POJ 2520 - DNA Sequence Alignment
// Model: gpt-6.1-sol
// Weighted wavefront alignment, with the remaining length difference as a
// consistent potential. Gap transitions towards the target diagonal cost zero;
// those away cost six. Process each score from outer diagonals inward.
// Equal letters extend for free; retain only the furthest point per diagonal.
// The matrix charges G/C substitutions four, all other substitutions five.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int N=50005, W=100011, O=50005;
int f[7][W], seen[W];
char a[N],b[N];
int main(){
 while(scanf("%50000s%50000s",a,b)==2){
  int n=strlen(a),m=strlen(b);
  if(n<m){swap(n,m);for(int i=0;i<=n;i++)swap(a[i],b[i]);}
  int d=n-m;
  memset(f,-1,sizeof(f));memset(seen,-1,sizeof(seen));
  f[3*d%7][O]=0;
  int answer=-1;
  for(int s=3*d;answer<0;s++){
   int lo=-(s-3*d)/6,hi=d+(s-3*d)/6;
   if(lo < -m)lo=-m;if(hi>n)hi=n;
   for(int t=0;t<=hi-lo;t++){
    int k=(t<hi-d)?hi-t:lo+(t-(hi-d));
    int p=O+k,x=f[s%7][p];f[s%7][p]=-1;
    if(x<0||x<=seen[p])continue;
    int y=x-k;
    while(x<n&&y<m&&a[x]==b[y]){++x;++y;}
    seen[p]=x;
    if(x==n&&y==m){answer=s;break;}
    if(x<n&&y<m){
     int w=((a[x]=='G'&&b[y]=='C')||(a[x]=='C'&&b[y]=='G'))?4:5;
     int &v=f[(s+w)%7][p];if(v<x+1)v=x+1;
    }
    if(x<n){int w=k<d?0:6;int &v=f[(s+w)%7][p+1];if(v<x+1)v=x+1;}
    if(y<m){int w=k>d?0:6;int &v=f[(s+w)%7][p-1];if(v<x)v=x;}
   }
  }
  printf("%d\n",answer);
 }
 return 0;
}
