// POJ 3584 - Small Polynomial Factorization
// Model: gpt-6.1-sol
// Enumerate subsets of approximate complex roots and divisors of the leading
// coefficient. Accept a candidate only after exact integer polynomial division.
// Input coefficients are ascending; output factors use degree then magnitude/value.
#include <iostream>
#include <sstream>
#include <vector>
#include <complex>
#include <algorithm>
#include <cmath>
#include <cstdlib>
using namespace std;
typedef long long I;
typedef complex<long double> C;
typedef vector<I> P;
I ab(I x){return x<0?-x:x;}
I gcdv(I a,I b){while(b){I t=a%b;a=b;b=t;}return ab(a);}
C root(vector<C> a,int m,C x){
 for(int it=1;it<=200;it++){
  C b=a[m],d=0,f=0; long double er=abs(b),ax=abs(x);
  for(int j=m-1;j>=0;j--){f=x*f+d;d=x*d+b;b=x*b+a[j];er=abs(b)+ax*er;}
  if(abs(b)<=1e-18L*er)return x;
  C g=d/b,h=g*g-2.0L*f/b,s=sqrt((long double)(m-1)*((long double)m*h-g*g));
  C gp=g+s,gm=g-s;if(abs(gp)<abs(gm))gp=gm;
  C dx=abs(gp)>0?(long double)m/gp:polar(1.0L+ax,(long double)it);
  C y=x-dx;if(y==x)return x;
  if(it%10==0)x-=dx*0.5L;else x=y;
 }
 return x;
}
vector<C> roots(P p,int trial){int n=p.size()-1;vector<C>a(p.size()),z(n);for(int i=0;i<=n;i++)a[i]=(long double)p[i];
 for(int m=n;m>=1;m--){C x=root(a,m, trial==0?C(0,0):polar(0.7L,(long double)(trial*m)));if(fabsl(x.imag())<1e-16L*fabsl(x.real()))x=C(x.real(),0);z[m-1]=x;C b=a[m];for(int j=m-1;j>=0;j--){C c=a[j];a[j]=b;b=x*b+c;}}
 return z;}
bool divide(const P&a,const P&b,P&q){int n=a.size()-1,m=b.size()-1;P r=a;q.assign(n-m+1,0);for(int i=n;i>=m;i--){if(r[i]%b[m])return false;I t=r[i]/b[m];if(ab(t)>4000000)return false;q[i-m]=t;for(int j=0;j<=m;j++)r[i-m+j]-=t*b[j];}for(int i=0;i<m;i++)if(r[i])return false;return true;}
vector<P> ans;
void factor(P p){int n=p.size()-1;if(n<=1){ans.push_back(p);return;}if(p[0]==0){P x(2,0);x[1]=1;ans.push_back(x);p.erase(p.begin());factor(p);return;}
 vector<int>ds;for(int d=1;d<=p[n];d++)if(p[n]%d==0)ds.push_back(d);
 for(int trial=0;trial<3;trial++){vector<C>z=roots(p,trial);for(int mask=1;mask<(1<<n);mask++){
 int k=0;for(int i=0;i<n;i++)if(mask>>i&1)k++;if(k>n/2)continue;
 vector<C>c(k+1);c[0]=1;int t=0;for(int i=0;i<n;i++)if(mask>>i&1){for(int j=t+1;j>=0;j--){C v=j?c[j-1]:C(0,0);if(j<=t)v-=z[i]*c[j];c[j]=v;}t++;}
 for(size_t di=0;di<ds.size();di++){int d=ds[di];P b(k+1),q;bool ok=true;for(int j=0;j<=k;j++){C v=c[j]*(long double)d;long double w=floor(v.real()+0.5L);if(fabsl(w)>4000000 || fabsl(v.imag())>0.2L || fabsl(v.real()-w)>0.2L){ok=false;break;}b[j]=(I)w;}if(!ok||b[0]==0)continue;I g=0;for(int j=0;j<=k;j++)g=gcdv(g,b[j]);if(g!=1)continue;if(divide(p,b,q)){factor(b);factor(q);return;}}
 }}ans.push_back(p);}
bool cmp(const P&a,const P&b){if(a.size()!=b.size())return a.size()<b.size();for(int i=(int)a.size()-1;i>=0;i--){if(ab(a[i])!=ab(b[i]))return ab(a[i])<ab(b[i]);if(a[i]!=b[i])return a[i]<b[i];}return false;}
int main(){string s;while(getline(cin,s)){istringstream in(s);P p;I a;while(in>>a)p.push_back(a);if(p.empty())continue;while(p.size()>1&&p.back()==0)p.pop_back();I g=0;for(size_t i=0;i<p.size();i++)g=gcdv(g,p[i]);if(p.back()<0)g=-g;cout<<g<<'\n';if(p.size()==1)continue;for(size_t i=0;i<p.size();i++)p[i]/=g;ans.clear();factor(p);sort(ans.begin(),ans.end(),cmp);for(size_t i=0;i<ans.size();i++){for(size_t j=0;j<ans[i].size();j++){if(j)cout<<' ';cout<<ans[i][j];}cout<<'\n';}}}
