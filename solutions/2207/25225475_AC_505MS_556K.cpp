// POJ 2207 - Intuitionistic Logic
// Model: gpt-6.1-sol
// Enumerate all antichains, precompute their algebra, and test all valuations.
// Downward closures turn conjunction into union and disjunction into intersection.
// Implication retains beta's maximal vertices outside alpha's downward closure.
// Implication associates right; an equivalence chain conjoins adjacent pairs.
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <map>
#include <cctype>
using namespace std;
typedef bitset<100> Bits;
int n,h,zero,reach[100][100],tab[4][101][101];
vector<Bits> sets,down;
void gen(int start,Bits a){
 sets.push_back(a);
 for(int i=start;i<n;++i){
  bool ok=true;
  for(int j=0;j<n;++j)if(a[j]&&(reach[i][j]||reach[j][i])){ok=false;break;}
  if(ok){a.set(i);gen(i+1,a);a.reset(i);}
 }
}
struct Node{int op,a,b;Node(int x,int y,int z):op(x),a(y),b(z){}};
vector<Node> nodes;
string s;int pos;int vars[26],nv;
int add(int op,int a,int b){nodes.push_back(Node(op,a,b));return (int)nodes.size()-1;}
int equiv();
int atom(){
 char c=s[pos++];
 if(c=='~'){int x=atom();return add(2,x,add(5,zero,0));}
 if(c=='('){int x=equiv();++pos;return x;}
 if(c=='0'||c=='1')return add(5,c=='0'?zero:0,0);
 if(vars[c-'A']<0)vars[c-'A']=nv++;
 return add(4,vars[c-'A'],0);
}
int conj(){int a=atom();while(s[pos]=='&'){++pos;int b=atom();a=add(0,a,b);}return a;}
int disj(){int a=conj();while(s[pos]=='|'){++pos;int b=conj();a=add(1,a,b);}return a;}
int impl(){int a=disj();if(s[pos]=='='&&s[pos+1]=='>'){pos+=2;int b=impl();a=add(2,a,b);}return a;}
int equiv(){int prev=impl(),ans=prev;bool first=true;
 while(s[pos]=='='&&s[pos+1]!='>'){++pos;int b=impl();int pair=add(3,prev,b);ans=first?pair:add(0,ans,pair);first=false;prev=b;}return ans;
}
int val[600],assignment[26];
bool check(int root,int depth){
 if(depth<nv){for(int i=0;i<h;++i){assignment[depth]=i;if(!check(root,depth+1))return false;}return true;}
 for(int i=0;i<(int)nodes.size();++i){Node &x=nodes[i];if(x.op==4)val[i]=assignment[x.a];else if(x.op==5)val[i]=x.a;else val[i]=tab[x.op][val[x.a]][val[x.b]];}
 return val[root]==0;
}
int main(){ios::sync_with_stdio(false);cin.tie(0);int m;
 while(cin>>n>>m){
  for(int i=0;i<n;++i)for(int j=0;j<n;++j)reach[i][j]=(i==j);
  for(int i=0,a,b;i<m;++i){cin>>a>>b;reach[a-1][b-1]=1;}
  for(int k=0;k<n;++k)for(int i=0;i<n;++i)if(reach[i][k])for(int j=0;j<n;++j)reach[i][j]|=reach[k][j];
  sets.clear();down.clear();gen(0,Bits());h=(int)sets.size();map<string,int> ids,ant;
  for(int i=0;i<h;++i){Bits d;for(int j=0;j<n;++j)if(sets[i][j])for(int k=0;k<n;++k)if(reach[k][j])d.set(k);down.push_back(d);ids[d.to_string()]=i;ant[sets[i].to_string()]=i;if((int)d.count()==n)zero=i;}
  for(int i=0;i<h;++i)for(int j=0;j<h;++j){tab[0][i][j]=ids[(down[i]|down[j]).to_string()];tab[1][i][j]=ids[(down[i]&down[j]).to_string()];tab[2][i][j]=ant[(sets[j]&~down[i]).to_string()];}
  for(int i=0;i<h;++i)for(int j=0;j<h;++j)tab[3][i][j]=tab[0][tab[2][i][j]][tab[2][j][i]];
  int k;cin>>k;getline(cin,s);
  while(k--){string line;getline(cin,line);s.clear();for(int i=0;i<(int)line.size();++i)if(!isspace((unsigned char)line[i]))s+=line[i];s+='\0';nodes.clear();pos=nv=0;for(int i=0;i<26;++i)vars[i]=-1;int root=equiv();cout<<(check(root,0)?"valid":"invalid")<<'\n';}
 }
}
