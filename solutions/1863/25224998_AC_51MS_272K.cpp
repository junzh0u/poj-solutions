// POJ 1863 - Subnumber
// Model: gpt-6.1-sol
// Enumerate the length and offset of the first integer. Reconstruct the least
// candidate from the visible suffix and the following integer's prefix, then
// verify by concatenation. Handle powers of ten separately. Decimal strings
// keep both integers and sequence positions exact for 200-digit input.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
string norm(string s) { size_t p=s.find_first_not_of('0'); return p==string::npos?"0":s.substr(p); }
string inc(string s) { int i=(int)s.size()-1; while(i>=0 && s[i]=='9') s[i--]='0'; if(i<0)s="1"+s;else ++s[i];return s; }
string dec(string s) { int i=(int)s.size()-1;while(i>=0&&s[i]=='0')s[i--]='9';if(i>=0)--s[i];return norm(s); }
bool lessnum(const string&a,const string&b){return a.size()!=b.size()?a.size()<b.size():a<b;}
string mul(string s,int m){int c=0;for(int i=(int)s.size()-1;i>=0;--i){int v=(s[i]-'0')*m+c;s[i]=char('0'+v%10);c=v/10;}while(c){s=char('0'+c%10)+s;c/=10;}return norm(s);}
string sub(string a,string b){int c=0,j=(int)b.size()-1;for(int i=(int)a.size()-1;i>=0;--i,--j){int v=a[i]-'0'-c-(j>=0?b[j]-'0':0);c=v<0;if(c)v+=10;a[i]=char('0'+v);}return norm(a);}
string pat,bestN;int bestOff;
void test(string x,int L,int o){if((int)x.size()!=L||x[0]=='0')return;string s=x,y=x;while((int)s.size()<o+(int)pat.size()){y=inc(y);s+=y;}if(s.compare(o,pat.size(),pat)!=0)return;if(bestN.empty()||lessnum(x,bestN)||(x==bestN&&o<bestOff)){bestN=x;bestOff=o;}}
string solve(string a){pat=a;bestN="";int n=(int)a.size();for(int L=1;L<=n+1;++L)for(int o=0;o<L;++o){int k=L-o;test(string(L,'9'),L,o);if(n<=k){string x(L,'0');x[0]='1';x.replace(o,n,a);test(x,L,o);}else{int b=n-k;if(b>=L){string y=a.substr(k,L);if(y[0]!='0')test(dec(y),L,o);}else{string suf=inc(a.substr(0,k));if((int)suf.size()>k)suf=suf.substr(1);string y(L,'?');bool ok=true;for(int i=0;i<b;++i)y[i]=a[k+i];for(int i=0;i<k;++i){int p=L-k+i;if(y[p]!='?'&&y[p]!=suf[i])ok=false;y[p]=suf[i];}for(int i=0;i<L;++i)if(y[i]=='?')y[i]=(i==0?'1':'0');if(ok&&y[0]!='0')test(dec(y),L,o);}}}int L=(int)bestN.size();string ans=sub(mul(bestN,L),string(L,'1'));for(int i=0;i<=bestOff;++i)ans=inc(ans);return ans;}
int main(){string a;while(cin>>a)cout<<solve(a)<<'\n';return 0;}
