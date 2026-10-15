// POJ 2519 - Cutting Necklace
// Model: gpt-6.1-sol
// Exact fractional programming: maximize q*sum-p*length with a sliding
// minimum deque, then replace p/q by the maximizing segment's average.
// A zero residual certifies the optimum; no floating-point rounding is used.
// Duplicate the ring, restricting starting positions to its first copy.
#include <cstdio>
#include <vector>
using namespace std;
typedef long long Int;
int gcd(int a,int b) { if(a<0)a=-a; while(b){int t=a%b;a=b;b=t;} return a; }
int main(){
 int n,l,u;
 while(scanf("%d%d%d",&n,&l,&u)==3){
  vector<int> s(2*n+1,0),dq(n+1);
  for(int i=1;i<=n;++i){int v;scanf("%d",&v);s[i]=s[i-1]+v;}
  for(int i=n+1;i<=2*n;++i)s[i]=s[n]+s[i-n];
  int p=s[l],q=l;
  for(;;){
   int head=0,tail=0,bp=p,bq=q;
   Int best=0;
   for(int e=l;e<=n-1+u;++e){
    int a=e-l;
    if(a<n){
     Int v=(Int)q*s[a]-(Int)p*a;
     while(head<tail && (Int)q*s[dq[tail-1]]-(Int)p*dq[tail-1]>=v)--tail;
     dq[tail++]=a;
    }
    while(head<tail && dq[head]<e-u)++head;
    int a0=dq[head],len=e-a0,sum=s[e]-s[a0];
    Int val=(Int)q*sum-(Int)p*len;
    if(val>best){best=val;bp=sum;bq=len;}
   }
   if(best==0)break;
   int g=gcd(bp,bq);p=bp/g;q=bq/g;
  }
  int g=gcd(p,q);printf("%d/%d\n",p/g,q/g);
 }
 return 0;
}
