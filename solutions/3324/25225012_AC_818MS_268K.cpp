// POJ 3324 - Lucas-Lehmer Test
// Model: gpt-6.1-sol
// FFT squares base-2^16 integers; fold high bits using 2^p = 1 modulo
// 2^p-1. Compute residues for composite exponents too, without prime tests.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct C { double r,i; C(double a=0,double b=0):r(a),i(b){} };
C operator+(C a,C b){return C(a.r+b.r,a.i+b.i);}
C operator-(C a,C b){return C(a.r-b.r,a.i-b.i);}
C operator*(C a,C b){return C(a.r*b.r-a.i*b.i,a.r*b.i+a.i*b.r);}
struct FFT {
 int n; vector<int> rev; vector<C> roots,a;
 FFT(int size):n(size),rev(n),roots(n),a(n){
  for(int i=0;i<n;i++) { int x=i,y=0; for(int k=n;k>1;k>>=1){y=y*2+(x&1);x>>=1;} rev[i]=y; double t=2*acos(-1.0)*i/n; roots[i]=C(cos(t),sin(t)); }
 }
 void transform(bool inv){
  for(int i=0;i<n;i++)if(i<rev[i])swap(a[i],a[rev[i]]);
  for(int len=2;len<=n;len*=2)for(int s=0;s<n;s+=len)for(int j=0;j<len/2;j++){
   C w=roots[j*(n/len)]; if(inv)w.i=-w.i;
   C u=a[s+j],v=a[s+j+len/2]*w; a[s+j]=u+v;a[s+j+len/2]=u-v;
  }
  if(inv)for(int i=0;i<n;i++){a[i].r/=n;a[i].i/=n;}
 }
};
void trim(vector<int>& a){while(a.size()>1 && a.back()==0)a.pop_back();}
void reduce(vector<int>& a,int p){
 int q=p/16,b=p%16;
 while((int)a.size()>q+1 || ((int)a.size()==q+1 && (a[q]>>b))){
  vector<int> c(q+2,0); long long carry=0;
  for(int i=0;i<=q+1;i++){
   int lo=i<q?a[i]:(i==q?(a[i]&((1<<b)-1)):0);
   int hi=0; if(i+q<(int)a.size())hi=a[i+q]>>b;
   if(b && i+q+1<(int)a.size())hi|=(a[i+q+1]<<(16-b))&65535;
   long long v=(long long)lo+hi+carry;c[i]=int(v&65535);carry=v>>16;
  }
  a.swap(c);trim(a);
 }
 bool all=(int)a.size()==(p+15)/16;
 for(int i=0;all && i<(int)a.size();i++)if(a[i]!=(i==q?((1<<b)-1):65535))all=false;
 if(all)a.assign(1,0);
}
int main(){
 int p;
 while(scanf("%d",&p)==1){
  if(p==1){puts("0");continue;}
  int n=1;while(n<2*((p+15)/16))n*=2; FFT f(n);
  vector<int> s(1,4); reduce(s,p);
  for(int it=0;it<p-2;it++){
   for(int i=0;i<n;i++)f.a[i]=C(i<(int)s.size()?s[i]:0);
   f.transform(false); for(int i=0;i<n;i++)f.a[i]=f.a[i]*f.a[i]; f.transform(true);
   vector<int> t(n+2,0);long long carry=0;
   for(int i=0;i<n;i++){long long v=(long long)(f.a[i].r+0.5)+carry;t[i]=int(v&65535);carry=v>>16;}
   t[n]=int(carry);trim(t);reduce(t,p);
   if(t.size()==1 && t[0]<2){int v=t[0];t.assign((p+15)/16,65535);if(p%16)t.back()=(1<<(p%16))-1;t[0]-=2-v;}
   else {int borrow=2;for(int i=0;borrow;i++){int v=t[i]-borrow;borrow=v<0;t[i]=v&65535;}}
   trim(t);s.swap(t);
  }
  printf("%x",s.back());for(int i=(int)s.size()-2;i>=0;i--)printf("%04x",s[i]);puts("");
 }
}
