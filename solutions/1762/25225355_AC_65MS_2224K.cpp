// POJ 1762 - Bundling
// Model: gpt-6.1-sol
// Enumerate all nonempty subsequences of each template, including its stop.
// Prefix DP minimizes bundles, then stops; retain the latest last stop for
// the minimum stop count. One extra boundary stop dominates all other states.
// Empty slots may occur anywhere; a template's stop remains mandatory.
#include <cstdio>
#include <algorithm>
using namespace std;
bool ok[4][1000][5];
int c[100005],d[100005],b[100005],s[100005],last[100005];
int main(){
 int t,n; if(scanf("%d%d",&t,&n)!=2)return 0;
 while(t--){char a[5];int p;scanf("%s%d",a,&p);
  for(int m=1;m<8;m++){int k=0,v=0,q=0;
   for(int j=0;j<3;j++)if(m&(1<<j)){v=v*10+a[j]-'A';k++;if(j<p)q++;}
   ok[k][v][p?q+1:0]=true;
  }
 }
 for(int i=1;i<=n;i++){char a[3];scanf("%s%d",a,&d[i]);c[i]=a[0]-'A';}
 for(int i=1;i<=n;i++){
  b[i]=s[i]=1000000000;last[i]=-1;
  for(int k=1;k<=3&&k<=i;k++){
   int j=i-k,v=0;for(int x=j+1;x<=i;x++)v=v*10+c[x];
   for(int z=0;z<=k+1;z++)if(ok[k][v][z]){
    for(int extra=0;extra<2;extra++){
     int stop=extra?j:last[j],ns=s[j]+extra+(z!=0);bool valid=true;
     for(int x=1;x<=k;x++){
      if(z && x>z-1)stop=j+z-1;
      if(d[j+x]>stop){valid=false;break;}
     }
     if(z && z-1==k)stop=i;
     if(!valid)continue;
     int nb=b[j]+1;
     if(nb<b[i]||(nb==b[i]&&ns<s[i])){b[i]=nb;s[i]=ns;last[i]=stop;}
     else if(nb==b[i]&&ns==s[i])last[i]=max(last[i],stop);
    }
   }
  }
 }
 printf("%d %d\n",b[n],s[n]);return 0;
}
