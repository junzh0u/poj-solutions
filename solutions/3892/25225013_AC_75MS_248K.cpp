// POJ 3892 - RSA Factorization
// Model: gpt-6.1-sol
// Enumerate squares a*a = 4*k*n + s*s, where a=q+k*p and |s|<=100000.
// Exact decimal big integers avoid floating-point loss for 120-digit inputs.
// The contest PDF corrects POJ's reversed ordering: print the smaller factor first.
// Test both signs of s; accept factor 1 because the discussion reports prime inputs.
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include <cstdlib>
using namespace std;
struct Big {
 vector<int> v;
 Big(long long x=0) { do {v.push_back(int(x%10000)); x/=10000;} while(x); }
 Big(string s) { for(int i=(int)s.size();i>0;i-=4) {int j=i-4;if(j<0)j=0;v.push_back(atoi(s.substr(j,i-j).c_str()));} trim(); }
 void trim(){while(v.size()>1 && v.back()==0)v.pop_back();}
 string str()const{char b[20];string s;for(int i=(int)v.size()-1;i>=0;--i){sprintf(b,i==(int)v.size()-1?"%d":"%04d",v[i]);s+=b;}return s;}
 int cmp(const Big& b)const{if(v.size()!=b.v.size())return v.size()<b.v.size()?-1:1;for(int i=(int)v.size()-1;i>=0;--i)if(v[i]!=b.v[i])return v[i]<b.v[i]?-1:1;return 0;}
 Big operator+(const Big& b)const{Big c;c.v.clear();int carry=0;for(size_t i=0;i<v.size()||i<b.v.size()||carry;++i){int t=carry+(i<v.size()?v[i]:0)+(i<b.v.size()?b.v[i]:0);c.v.push_back(t%10000);carry=t/10000;}return c;}
 Big operator-(const Big& b)const{Big c=*this;int borrow=0;for(size_t i=0;i<v.size();++i){int t=v[i]-borrow-(i<b.v.size()?b.v[i]:0);borrow=t<0;if(borrow)t+=10000;c.v[i]=t;}c.trim();return c;}
 Big operator*(const Big& b)const{Big c;c.v.assign(v.size()+b.v.size()+1,0);for(size_t i=0;i<v.size();++i){int carry=0;for(size_t j=0;j<b.v.size()||carry;++j){int t=c.v[i+j]+carry+(j<b.v.size()?v[i]*b.v[j]:0);c.v[i+j]=t%10000;carry=t/10000;}}c.trim();return c;}
 Big mul(int b)const{Big c; c.v.clear();long long carry=0;for(size_t i=0;i<v.size()||carry;++i){long long t=carry+(i<v.size()?(long long)v[i]*b:0);c.v.push_back(int(t%10000));carry=t/10000;}c.trim();return c;}
 Big div(int b,int &r)const{Big c=*this;long long rem=0;for(int i=(int)v.size()-1;i>=0;--i){long long t=rem*10000+v[i];c.v[i]=int(t/b);rem=t%b;}r=int(rem);c.trim();return c;}
 long long small()const{long long x=0;for(int i=(int)v.size()-1;i>=0;--i)x=x*10000+v[i];return x;}
};
Big root(const Big& x){Big r;int digits=((int)x.str().size()+1)/2;for(int i=0;i<digits;++i){r=r.mul(10);int lo=0,hi=9;while(lo<hi){int m=(lo+hi+1)/2;Big t=r+Big(m);for(int j=i+1;j<digits;++j)t=t.mul(10);if((t*t).cmp(x)<=0)lo=m;else hi=m-1;}r=r+Big(lo);}return r;}
int main(){string ns;int k;while(cin>>ns>>k){Big n(ns),x=n.mul(k).mul(4),a=root(x);if((a*a).cmp(x)<0)a=a+Big(1);Big d=a*a-x,limit(10000000000LL);bool done=false;while(d.cmp(limit)<=0&&!done){long long z=d.small(),s=(long long)sqrt((double)z);while((s+1)*(s+1)<=z)++s;while(s*s>z)--s;if(s*s==z){for(int sign=-1;sign<=1;sign+=2){Big low=a-Big(s),high=a+Big(s);int r1,r2;Big p=(sign==-1?low:high).div(2*k,r1),q=(sign==-1?high:low).div(2,r2);if(!r1&&!r2&&(p*q).cmp(n)==0){if(p.cmp(q)>0){Big t=p;p=q;q=t;}if(p.cmp(Big(1))==0)continue;cout<<p.str()<<" * "<<q.str()<<'\n';done=true;break;}}}d=d+a.mul(2)+Big(1);a=a+Big(1);}if(!done)cout<<"1 * "<<n.str()<<'\n'; }
}
