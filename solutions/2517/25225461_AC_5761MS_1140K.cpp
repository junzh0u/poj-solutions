// POJ 2517 - Cover
// Model: gpt-6.1-sol
// Enumerate a rectangle touching two sides of the remaining marks' bounding
// box. Pigeonhole over four extreme marks proves one of at most three
// rectangles has this property. Remove its marks and recurse; overlaps remain
// allowed and their matrix costs are charged separately. Empty rectangles cost 0.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct B {int t,b,l,r; B():t(30),b(-1),l(30),r(-1){} void add(int x,int y){t=min(t,x);b=max(b,x);l=min(l,y);r=max(r,y);} void merge(const B& z){t=min(t,z.t);b=max(b,z.b);l=min(l,z.l);r=max(r,z.r);} };
struct R { int t,b,l,r,c; bool operator<(const R& o)const {return c<o.c;} };
int n,m,p[32][32],a[32][32],w[32][32],num[32][32],best;
int lo[65536],hi[65536];
int query(int q[32][32],int t,int b,int l,int r){if(t>b||l>r)return 0;return q[b+1][r+1]-q[t][r+1]-q[b+1][l]+q[t][l];}
R first;
unsigned int bits[32];
int cost(int t,int b,int l,int r){return p[b+1][r+1]-p[t][r+1]-p[b+1][l]+p[t][l];}
void solve(unsigned int *s,int k,int paid,int cnt,int lb){
 int t=n,b=-1,l=n,r=-1;
 for(int i=0;i<n;i++)if(s[i]){if(t==n)t=i;b=i;unsigned int z=s[i];l=min(l,(z&65535)?lo[z&65535]:16+lo[z>>16]);r=max(r,(z>>16)?16+hi[z>>16]:hi[z]);}
 if(b<0){best=min(best,paid);return;}
 if(cnt>k*m||paid+lb>=best)return;
 if((b-t+1)*(r-l+1)<=m){best=min(best,paid+cost(t,b,l,r));if(k==1||paid+lb>=best)return;}
 if(k==1)return;
 B rt[32],rb[32],cl[32],cr[32];
 if(k==2){for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(s[i]&(1u<<j)){rt[i+1].add(i,j);rb[i].add(i,j);cl[j+1].add(i,j);cr[j].add(i,j);}
 for(int i=1;i<=n;i++){rt[i].merge(rt[i-1]);cl[i].merge(cl[i-1]);}
 for(int i=n-1;i>=0;i--){rb[i].merge(rb[i+1]);cr[i].merge(cr[i+1]);}}
 vector<R> v;
 for(int x=t;x<=b;x++)for(int y=l;y<=r;y++){
  R q[4]={{t,x,l,y,0},{t,x,y,r,0},{x,b,l,y,0},{x,b,y,r,0}};
  for(int h=0;h<4;h++){R z=q[h];if((z.b-z.t+1)*(z.r-z.l+1)<=m){z.c=cost(z.t,z.b,z.l,z.r);v.push_back(z);}}
 }
 for(int x=t;x<=b;x++)for(int y=x;y<=b;y++)if((y-x+1)*(r-l+1)<=m){R z={x,y,l,r,cost(x,y,l,r)};v.push_back(z);}
 for(int x=l;x<=r;x++)for(int y=x;y<=r;y++)if((b-t+1)*(y-x+1)<=m){R z={t,b,x,y,cost(t,b,x,y)};v.push_back(z);}
 if(k==3)sort(v.begin(),v.end());
 for(size_t h=0;h<v.size();h++){
  R z=v[h];if(paid+z.c>=best){if(k==3)break;else continue;}
  if(k==2){B rem=rt[z.t];rem.merge(rb[z.b+1]);rem.merge(cl[z.l]);rem.merge(cr[z.r+1]);if(rem.b<0)best=min(best,paid+z.c);else if((rem.b-rem.t+1)*(rem.r-rem.l+1)<=m)best=min(best,paid+z.c+cost(rem.t,rem.b,rem.l,rem.r));continue;}
  unsigned int mask=((1u<<(z.r+1))-1)^((1u<<z.l)-1),ns[32];
  // Only tight rectangles can be optimal, since all costs are positive.
  if(!(s[z.t]&mask)||!(s[z.b]&mask))continue;
  unsigned int u=0;for(int i=z.t;i<=z.b;i++)u|=s[i];
  if(!(u&(1u<<z.l))||!(u&(1u<<z.r)))continue;
  for(int i=0;i<n;i++)ns[i]=s[i]&((i>=z.t&&i<=z.b)?~mask:~0u);
  int removed=query(num,z.t,z.b,z.l,z.r), val=query(w,z.t,z.b,z.l,z.r);
  if(k==2){removed-=query(num,max(z.t,first.t),min(z.b,first.b),max(z.l,first.l),min(z.r,first.r));val-=query(w,max(z.t,first.t),min(z.b,first.b),max(z.l,first.l),min(z.r,first.r));}
  if(k==3)first=z;
  solve(ns,k-1,paid+z.c,cnt-removed,lb-val);
  if(best==paid+lb)return;
 }
}
int main(){for(int i=1;i<65536;i++){lo[i]=(i&1)?0:1+lo[i>>1];hi[i]=i==1?0:1+hi[i>>1];}int T;scanf("%d",&T);while(T--){scanf("%d%d",&n,&m);int c;scanf("%d",&c);for(int i=0;i<n;i++)bits[i]=0;while(c--){int x,y;scanf("%d%d",&x,&y);bits[x-1]|=1u<<(y-1);}for(int i=0;i<=n;i++)for(int j=0;j<=n;j++)p[i][j]=0;for(int i=0;i<n;i++)for(int j=0;j<n;j++){scanf("%d",&a[i][j]);p[i+1][j+1]=a[i][j]+p[i][j+1]+p[i+1][j]-p[i][j];}int cnt=0,lb=0;for(int i=0;i<=n;i++)for(int j=0;j<=n;j++)w[i][j]=num[i][j]=0;for(int i=0;i<n;i++)for(int j=0;j<n;j++){int z=(bits[i]>>j)&1;cnt+=z;lb+=z*a[i][j];w[i+1][j+1]=z*a[i][j]+w[i][j+1]+w[i+1][j]-w[i][j];num[i+1][j+1]=z+num[i][j+1]+num[i+1][j]-num[i][j];}best=1000000000;solve(bits,3,0,cnt,lb);if(best==1000000000)puts("Impossible");else printf("%d\n",best);}return 0;}
