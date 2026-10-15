// POJ 2450 - A New Kind of Chess
// Model: gpt-6.1-sol
// Count paths by their first move out of the occupied rectangle. The
// remaining moves can be permuted freely. Enumerate the thin boundary
// strips and maintain their binomial coefficients by exact recurrences.
// Equal displacement vectors remain distinct move types.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Big {
 vector<int> v;
 Big(int x=0) { v.push_back(x); }
 void mul(int x) { int carry=0; for(size_t i=0;i<v.size();++i) { int z=v[i]*x+carry; v[i]=z%10000; carry=z/10000; } while(carry) {v.push_back(carry%10000);carry/=10000;} }
 void divi(int x) { int rem=0; for(int i=(int)v.size()-1;i>=0;--i) {int z=rem*10000+v[i];v[i]=z/x;rem=z%x;} while(v.size()>1 && v.back()==0)v.pop_back(); }
 void add(const Big& b) {int carry=0;size_t n=max(v.size(),b.v.size());v.resize(n,0);for(size_t i=0;i<n;++i){int z=v[i]+(i<b.v.size()?b.v[i]:0)+carry;v[i]=z%10000;carry=z/10000;}if(carry)v.push_back(carry);}
 void print() {printf("%d",v.back());for(int i=(int)v.size()-2;i>=0;--i)printf("%04d",v[i]);puts("");}
};
int ceilpos(int x,int y) {return x<=0?0:1+(x-1)/y;}
void countFirst(Big& ans,int N,int M,int P,int Q,int a,int b,int c,int d) {
 // i includes the first move (a,b); j is the number of (c,d) moves.
 bool have=false; int oldi=0,oldj=0; Big base;
 int imax=min(N/a,M/b);
 for(int i=1;i<=imax;++i) {
  int x=N-a*i,y=M-b*i;
  int lo=max(ceilpos(x-P,c),ceilpos(y-Q,d));
  int hi=min(x/c,y/d);
  hi=min(hi,max(ceilpos(x+a-P,c),ceilpos(y+b-Q,d))-1);
  if(lo>hi)continue;
  if(!have) {base=Big(1);for(int k=1;k<=min(i-1,lo);++k){base.mul(i-1+lo-min(i-1,lo)+k);base.divi(k);}have=true;}
  else {
   // First decrease j, then increase i: intermediate coefficients stay small.
   while(oldj>lo){base.mul(oldj);base.divi(oldi-1+oldj);--oldj;}
   while(oldi<i){base.mul(oldi+oldj);base.divi(oldi);++oldi;}
  }
  oldi=i;oldj=lo;
  Big cur=base;
  for(int j=lo;j<=hi;++j){ans.add(cur);if(j<hi){cur.mul(i+j);cur.divi(j+1);}}
 }
}
int main(){int z;scanf("%d",&z);while(z--){int n,m,p,q,a,b,c,d;scanf("%d%d%d%d%d%d%d%d",&n,&m,&p,&q,&a,&b,&c,&d);Big ans;countFirst(ans,n,m,p,q,a,b,c,d);countFirst(ans,n,m,p,q,c,d,a,b);ans.print();}return 0;}
