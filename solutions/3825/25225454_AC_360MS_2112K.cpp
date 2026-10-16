// POJ 3825 - Here's a Product Which Will Make You Tensor
// Model: gpt-6.1-sol
// For every nontrivial block shape, normalize the first block by its gcd.
// All blocks must be integer multiples of that primitive block. Every
// divisor of the gcd of those multipliers gives one ordered factor pair.
// Scalar redistributions count separately; neither factor may be 1 by 1.
// Products are at most 2^32 and therefore exactly representable as double.
#include <cstdio>
static int a[500][500], b[500][500];
int gcd(int x,int y) { while(y) { int t=x%y; x=y; y=t; } return x; }
int divisors(int x) { int z=0; for(int d=1;d*d<=x;++d) if(x%d==0) z+=(d*d==x?1:2); return z; }
int main() {
 int r,c;
 while(std::scanf("%d%d",&r,&c)==2 && r) {
  for(int i=0;i<r;++i) for(int j=0;j<c;++j) std::scanf("%d",&a[i][j]);
  int ans=0;
  for(int h=1;h<=r;++h) if(r%h==0) for(int w=1;w<=c;++w) if(c%w==0) {
   if((h==1&&w==1)||(h==r&&w==c)) continue;
   int g=0;
   for(int i=0;i<h;++i) for(int j=0;j<w;++j) g=gcd(g,a[i][j]);
   for(int i=0;i<h;++i) for(int j=0;j<w;++j) b[i][j]=a[i][j]/g;
   bool ok=true; int common=0;
   for(int x=0;x<r&&ok;x+=h) for(int y=0;y<c&&ok;y+=w) {
    if(a[x][y]%b[0][0]) {ok=false;break;}
    int k=a[x][y]/b[0][0]; common=gcd(common,k);
    for(int i=0;i<h&&ok;++i) for(int j=0;j<w;++j)
     if(static_cast<double>(k)*b[i][j]!=a[x+i][y+j]) {ok=false;break;}
   }
   if(ok) ans+=divisors(common);
  }
  std::printf("%d\n",ans);
 }
 return 0;
}
