// POJ 3807 - Chemist's Math
// Model: gpt-6.1-sol
// Parse atom counts and solve the one-dimensional nullspace modulo a prime.
// Enumerate the bounded last coefficient and verify conservation over integers.
// Positivity and uniqueness imply rank n-1; unlucky primes are skipped.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
using namespace std;
typedef long long LL;
typedef map<string,int> Counts;
int number(const string &s,int &p) { int v=0; while(p<(int)s.size() && isdigit(s[p])) v=v*10+s[p++]-'0'; return v?v:1; }
Counts molecule(const string &s,int &p) {
 Counts a;
 while(p<(int)s.size() && s[p]!=')' && s[p]!='+' && s[p]!='-' && s[p]!='.') {
  Counts b;
  if(s[p]=='(') { ++p; b=molecule(s,p); ++p; }
  else { string e; e+=s[p++]; if(p<(int)s.size() && islower(s[p])) e+=s[p++]; b[e]=1; }
  int k=number(s,p); for(Counts::iterator i=b.begin();i!=b.end();++i) a[i->first]+=k*i->second;
 }
 return a;
}
LL power(LL a,LL b,LL mod) { LL r=1; while(b) { if(b&1) r=r*a%mod; a=a*a%mod;b/=2;} return r; }
bool prime(LL x) { for(LL d=2;d*d<=x;++d) if(x%d==0)return false;return true; }
int main() {
 string s;
 while(cin>>s && s!=".") {
  vector<Counts> mol; vector<int> sign; map<string,int> ids; int p=0,sg=1;
  while(p<(int)s.size() && s[p]!='.') {
   Counts c=molecule(s,p); mol.push_back(c); sign.push_back(sg);
   for(Counts::iterator i=c.begin();i!=c.end();++i) if(!ids.count(i->first)) { int z=ids.size();ids[i->first]=z; }
   if(s[p]=='+') ++p; else if(s[p]=='-') {p+=2;sg=-1;}
  }
  int n=mol.size(),m=ids.size();vector< vector<LL> > original(m,vector<LL>(n));
  for(int j=0;j<n;++j)for(Counts::iterator i=mol[j].begin();i!=mol[j].end();++i)original[ids[i->first]][j]=sign[j]*i->second;
  vector<LL> answer; LL mod=1000000007;
  while(answer.empty()) {
   vector< vector<LL> > a=original;for(int i=0;i<m;++i)for(int j=0;j<n;++j)a[i][j]=(a[i][j]%mod+mod)%mod;
   int r=0;vector<int> pivot;
   for(int col=0;col<n-1 && r<m;++col) {
    int k=r;while(k<m && !a[k][col])++k;if(k==m)continue;swap(a[k],a[r]);
    LL inv=power(a[r][col],mod-2,mod);for(int j=col;j<n;++j)a[r][j]=a[r][j]*inv%mod;
    for(int i=0;i<m;++i)if(i!=r) { LL t=a[i][col];for(int j=col;j<n;++j)a[i][j]=(a[i][j]-t*a[r][j]%mod+mod)%mod; }
    pivot.push_back(col);++r;
   }
   if(r==n-1) {
    vector<LL> ratio(n,1);for(int i=0;i<r;++i)ratio[pivot[i]]=(mod-a[i][n-1])%mod;
    for(int d=1;d<=40000 && answer.empty();++d) {
     vector<LL> v(n);bool ok=true;for(int j=0;j<n;++j) {v[j]=ratio[j]*d%mod;if(v[j]<1 || v[j]>40000) {ok=false;break;}}
     if(!ok)continue;
     for(int i=0;i<m && ok;++i) {LL sum=0;for(int j=0;j<n;++j)sum+=original[i][j]*v[j];if(sum)ok=false;}
     if(ok)answer=v;
    }
   }
   if(answer.empty()) {do {mod+=2;}while(!prime(mod));}
  }
  for(int j=0;j<n;++j) {if(j)cout<<' ';cout<<answer[j];}cout<<'\n';
 }
}
