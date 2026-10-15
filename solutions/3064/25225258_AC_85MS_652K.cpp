// POJ 3064 - Payment System
// Model: gpt-6.1-sol
// Enumerate all exponent lists whose product is at most 332. For each
// product keep its optimal tower; discard towers dominated by cheaper ones.
// Use the largest affordable integer base, found by exact roots.
// Compare towers through repeated logarithms; retain lexicographic ties.
// Input is a sequence of balances ending at EOF.
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std;
struct Big {
 vector<int> a;
 Big(int x=0) { do {a.push_back(x%10000);x/=10000;}while(x); }
 Big(string s) { for(int p=(int)s.size();p>0;p-=4) a.push_back(atoi(s.substr(max(0,p-4),min(4,p)).c_str())); }
 void trim(){while(a.size()>1 && a.back()==0)a.pop_back();}
 string str()const {string s; char b[20]; for(int i=(int)a.size()-1;i>=0;--i){sprintf(b,i==(int)a.size()-1?"%d":"%04d",a[i]);s+=b;}return s;}
};
int cmp(const Big&a,const Big&b){if(a.a.size()!=b.a.size())return a.a.size()<b.a.size()?-1:1;for(int i=(int)a.a.size()-1;i>=0;--i)if(a.a[i]!=b.a[i])return a.a[i]<b.a[i]?-1:1;return 0;}
Big mul(const Big&a,const Big&b){Big c;c.a.assign(a.a.size()+b.a.size(),0);for(size_t i=0;i<a.a.size();++i){int carry=0;for(size_t j=0;j<b.a.size();++j){int v=c.a[i+j]+a.a[i]*b.a[j]+carry;c.a[i+j]=v%10000;carry=v/10000;}size_t k=i+b.a.size();while(carry){int v=c.a[k]+carry;c.a[k]=v%10000;carry=v/10000;++k;if(k==c.a.size()&&carry)c.a.push_back(0);}}c.trim();return c;}
Big power(Big b,int p,const Big*cap=0){Big r(1);while(p){if(p&1){r=mul(r,b);if(cap&&cmp(r,*cap)>0)return r;}p/=2;if(p){b=mul(b,b);if(cap&&cmp(b,*cap)>0)b=*cap,b.a.push_back(1);}}return r;}
string root(const Big&n,int p){string s=n.str(),r;int digits=((int)s.size()+p-1)/p;for(int i=0;i<digits;++i){int lo=0,hi=9,best=0;while(lo<=hi){int m=(lo+hi)/2;string t=r+char('0'+m)+string(digits-i-1,'0');if(cmp(power(Big(t),p,&n),n)<=0)best=m,lo=m+1;else hi=m-1;}r+=char('0'+best);}while(r.size()>1&&r[0]=='0')r.erase(0,1);return r;}
const long double LIMIT=1e100L;
long double tower(const vector<int>&v,int i){if(i==(int)v.size())return 1;long double e=tower(v,i+1);if(e>=LIMIT||e*log((long double)v[i])>log(LIMIT))return LIMIT;return exp(e*log((long double)v[i]));}
int sign(long double x,long double scale){return x>1e-15L*scale?1:x< -1e-15L*scale?-1:0;}
// Compare c*tower(a)+u and d*tower(b)+v. Once a tower exceeds
// 10^100, bounded additive terms cannot change unequal leading terms.
int compare(long double c,const vector<int>&a,int i,long double u,long double d,const vector<int>&b,int j,long double v){long double x=tower(a,i),y=tower(b,j);if(x<LIMIT&&y<LIMIT){x=c*x+u;y=d*y+v;return sign(x-y,max((long double)1,max(fabsl(x),fabsl(y))));}if(x<LIMIT){long double z=c*x+u;vector<int> empty;return -compare(d,b,j,v,1,empty,0,z);}if(y<LIMIT){long double z=d*y+v;return compare(log((long double)a[i]),a,i+1,log(c),1,vector<int>(),0,log(z));}int k=compare(log((long double)a[i]),a,i+1,log(c),log((long double)b[j]),b,j+1,log(d));return k?k:sign(u-v,1);}
struct Candidate{string base;vector<int> tail;};
bool lexless(const Candidate&a,const Candidate&b){if(a.base.size()!=b.base.size())return a.base.size()<b.base.size();if(a.base!=b.base)return a.base<b.base;return a.tail<b.tail;}
int exactExponent(const vector<int>&a){int x=1;for(int i=(int)a.size()-1;i>=0;--i){int y=1;for(int j=0;j<x;++j){if(y>1000/a[i])return -1;y*=a[i];}x=y;}return x;}
int better(const Candidate&a,const Candidate&b){int x=exactExponent(a.tail),y=exactExponent(b.tail);if(x>0&&y>0)return cmp(power(Big(a.base),x),power(Big(b.base),y));return compare(log(strtold(a.base.c_str(),0)),a.tail,0,0,log(strtold(b.base.c_str(),0)),b.tail,0,0);}
vector<vector<int> > lists;
void gen(vector<int>&v,int p){if(!v.empty())lists.push_back(v);for(int x=2;x*p<=332;++x){v.push_back(x);gen(v,p*x);v.pop_back();}}
int main(){vector<int> v;gen(v,1);
vector<vector<int> > optimal(333); vector<bool> used(333,false);
for(size_t k=0;k<lists.size();++k){int p=1;for(size_t j=0;j<lists[k].size();++j)p*=lists[k][j];Candidate a,b;a.base=b.base="2";a.tail=lists[k];b.tail=optimal[p];int z=used[p]?better(a,b):1;if(z>0||(z==0&&a.tail<b.tail)){optimal[p]=a.tail;used[p]=true;}}
lists.clear();Candidate record;record.base="2";
for(int p=2;p<=332;++p)if(used[p]){Candidate c;c.base="2";c.tail=optimal[p];int z=better(c,record);if(z>0||(z==0&&c.tail<record.tail)){lists.push_back(c.tail);record=c;}}
reverse(lists.begin(),lists.end());
string s;while(cin>>s){Big n(s);long double ln=log(strtold(s.c_str(),0));vector<string> roots(333);roots[1]=s;Candidate best;best.base=s;for(size_t k=0;k<lists.size();++k){int p=1;for(size_t j=0;j<lists[k].size();++j)p*=lists[k][j];if(ln/p<log(2.0L)-1e-15L)continue;
if(compare(ln/p,lists[k],0,0,log(strtold(best.base.c_str(),0)),best.tail,0,0)<0)continue;
if(roots[p].empty())roots[p]=root(n,p);if(roots[p]=="1"||roots[p]=="0")continue;Candidate c;c.base=roots[p];c.tail=lists[k];int z=better(c,best);if(z>0||(z==0&&lexless(c,best)))best=c;}cout<<best.base;for(size_t j=0;j<best.tail.size();++j)cout<<'^'<<best.tail[j];cout<<'\n';}}
