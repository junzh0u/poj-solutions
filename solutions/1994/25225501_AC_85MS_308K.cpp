// POJ 1994 - Logic
// Model: gpt-6.1-sol
// Parse formulas into a postorder tree and enumerate all role assignments.
// A third truth value represents an unsayable compound assertion. Quoted
// speech tests whether its child can be said, even for an alleged loser.
// Type 4 has its own period in addition to the main statement's period.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <algorithm>
using namespace std;
struct Node { int op,a,b,x; Node(int o,int aa=0,int bb=0,int xx=0):op(o),a(aa),b(bb),x(xx){} };
vector<Node> ns;
vector<string> ts;
map<string,int> ids;
int posi;
int add(int op,int a=0,int b=0,int x=0) { ns.push_back(Node(op,a,b,x)); return ns.size()-1; }
int number(string s) { int v=0; for(size_t i=0;i<s.size();++i) v=min(11,v*10+s[i]-'0'); return v; }
string norm(string s) { size_t i=s.find_first_not_of('0'); return i==string::npos?"0":s.substr(i); }
string sum(string a,string b) { string c; int i=a.size()-1,j=b.size()-1,k=0; while(i>=0||j>=0||k){ if(i>=0) k+=a[i--]-'0'; if(j>=0) k+=b[j--]-'0'; c+=char('0'+k%10);k/=10;} reverse(c.begin(),c.end());return norm(c); }
int parse() {
 string s=ts[posi++];
 if(s=="(") { int a=parse(); ++posi; string op=ts[posi++]; if(op=="a") ++posi; ++posi; int b=parse(); ++posi; return add(op=="a"?7:8,a,b); }
 if(isdigit(s[0])) { ++posi; string y=ts[posi++]; ++posi; string z=ts[posi++]; return add(0,0,0,sum(s,y)==norm(z)); }
 if(s=="Poctivcu"||s=="Lharu") { ++posi; string cmp=ts[posi++]; if(cmp!="alespon") ++posi; int x=number(ts[posi++]); return add(cmp=="mene"?4:cmp=="vice"?5:6,s=="Poctivcu",0,x); }
 int h=ids[s]; string op=ts[posi++];
 if(op=="je") return add(1,h,0,ts[posi++]=="poctivec");
 ++posi; int a=parse(); ++posi;
 string suffix=ts[posi++];
 if(suffix==".") return add(2,a,h);
 if(suffix=="a") { posi+=4; return add(3,a,h,1); }
 posi+=4; return add(3,a,h,0);
}
int main(){
 int Z;cin>>Z; for(int tc=1;tc<=Z;++tc){ int p;cin>>p; vector<string> names(p);ids.clear();for(int i=0;i<p;++i){cin>>names[i];ids[names[i]]=i;}
 int m;cin>>m;string line;getline(cin,line); ns.clear(); vector<int> roots,who;
 for(int i=0;i<m;++i){getline(cin,line);ts.clear();for(size_t j=0;j<line.size();){ if(isspace((unsigned char)line[j])){++j;continue;} size_t k=j;if(isalnum((unsigned char)line[j])){while(j<line.size()&&isalnum((unsigned char)line[j]))++j;}else ++j;ts.push_back(line.substr(k,j-k));} who.push_back(ids[ts[0]]);posi=2;roots.push_back(parse()); }
 int all=(1<<p)-1, yes=all,no=all,sol=0,losers=0;vector<int> v(ns.size());
 for(int mask=0;mask<=all;++mask){int cnt=0;for(int i=0;i<p;++i)cnt+=(mask>>i)&1;
 for(size_t j=0;j<ns.size();++j){Node n=ns[j];int a=(n.op==2||n.op==3||n.op==7||n.op==8)?v[n.a]:0,b=(n.op==7||n.op==8)?v[n.b]:0,r=0;
 if(n.op==0)r=n.x;else if(n.op==1)r=(((mask>>n.a)&1)==n.x);else if(n.op==2)r=(a==((mask>>n.b)&1));else if(n.op==3){int q=(a==((mask>>n.b)&1));int t=(a==n.x); r=q==t?q:-1;}else if(n.op>=4&&n.op<=6){int c=n.a?cnt:p-cnt;r=n.op==4?c<n.x:n.op==5?c>n.x:c>=n.x;}else if(n.op==7)r=(a==1&&b==1);else r=(a==1||b==1);v[j]=r;}
 int bad=0;for(int i=0;i<m;++i)if(v[roots[i]]!=((mask>>who[i])&1))bad|=1<<who[i];
 if(!bad){++sol;yes&=mask;no&=(all^mask);}else if((bad&(bad-1))==0)losers|=bad;
 }
 cout<<"Hra cislo "<<tc<<":\n";
 if(sol){bool any=false;for(int i=0;i<p;++i)if((yes|no)&(1<<i)){any=true;cout<<names[i]<<" je "<<((yes>>i)&1?"poctivec":"lhar")<<".\n";}if(!any)cout<<"Neda se nic zjistit.\n";}
 else if(!losers)cout<<"Pravidla porusilo vice hracu.\n";
 else if((losers&(losers-1))==0){for(int i=0;i<p;++i)if(losers&(1<<i))cout<<names[i]<<" prohral.\n";}
 else {cout<<"Prohral nektery z techto hracu: ";bool first=true;for(int i=0;i<p;++i)if(losers&(1<<i)){if(!first)cout<<", ";first=false;cout<<names[i];}cout<<".\n";}
 cout<<'\n';
 }
}
