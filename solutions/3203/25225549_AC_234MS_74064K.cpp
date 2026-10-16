// POJ 3203 - Incremental Python Parsing
// Model: gpt-6.1-sol
// Segment summaries for both incoming quote states track bracket balance and
// the minimum balance at unquoted newlines, with its multiplicity. Validity
// at queries makes zero the only minimum that contributes logical newlines.
// Discussion reports newline replacement and columns beyond a physical line:
// positions are physical-line starts plus column offsets in the current text.
// Use the final standalone # as the delimiter, allowing # within the program.
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;
const int INF=100000000;
struct Node { int sum[2], low[2], num[2], flip, lines; };
vector<Node> tr;
int base;
void leaf(int p,char c) {
 Node &n=tr[p];n.flip=(c=='"');n.lines=(c=='\n');
 for(int q=0;q<2;++q){
  n.sum[q]=0;n.low[q]=INF;n.num[q]=0;
  if(!q){
   if(c=='('||c=='['||c=='{') n.sum[q]=1;
   if(c==')'||c==']'||c=='}') n.sum[q]=-1;
   if(c=='\n'){n.low[q]=0;n.num[q]=1;}
  }
 }
}
void pull(int p){
 Node &n=tr[p], &a=tr[p*2], &b=tr[p*2+1];
 n.flip=a.flip^b.flip;n.lines=a.lines+b.lines;
 for(int q=0;q<2;++q){int r=q^a.flip;
  n.sum[q]=a.sum[q]+b.sum[r];
  int v=b.low[r]==INF?INF:a.sum[q]+b.low[r];
  n.low[q]=min(a.low[q],v);n.num[q]=0;
  if(a.low[q]==n.low[q]) n.num[q]+=a.num[q];
  if(v==n.low[q]) n.num[q]+=b.num[r];
 }
}
int lineStart(int a){
 if(a<=1)return 0;
 int k=a-1,p=1;
 if(k>tr[1].lines)return base;
 while(p<base){if(tr[p*2].lines>=k)p*=2;else{k-=tr[p*2].lines;p=p*2+1;}}
 return p-base+1;
}
int state,depth,answer;
void consume(int p){
 Node &n=tr[p];
 if(n.low[state]!=INF && depth+n.low[state]==0)answer+=n.num[state];
 depth+=n.sum[state];state^=n.flip;
}
void prefix(int p,int l,int r,int end){
 if(r<=end){consume(p);return;}
 int m=(l+r)/2;
 if(end<=m)prefix(p*2,l,m,end);
 else{consume(p*2);prefix(p*2+1,m,r,end);}
}
int main(){
 ios::sync_with_stdio(false);cin.tie(0);
 vector<string> lines;string s;int delimiter=-1;
 while(getline(cin,s)){if(!s.empty()&&s[s.size()-1]=='\r')s.erase(s.size()-1);if(s=="#")delimiter=lines.size();lines.push_back(s);}
 if(delimiter<0)return 0;
 string text;
 for(int i=0;i<delimiter;++i){text+=lines[i];text+='\n';}
 base=1;while(base<(int)text.size())base*=2;
 tr.resize(base*2);
 for(int i=0;i<base;++i)leaf(base+i,i<(int)text.size()?text[i]:' ');
 for(int i=base-1;i;--i)pull(i);
 for(int i=delimiter+1;i<(int)lines.size();++i){
  istringstream in(lines[i]);char c;int a,b;if(!(in>>c>>a>>b))continue;
  int pos=lineStart(a)+b-1;
  if(c=='?'){
   state=depth=answer=0;if(pos>0)prefix(1,0,base,min(pos,base));
   cout<<answer+1<<'\n';
  }else if(pos>=0&&pos<(int)text.size()){
   text[pos]=c;int p=base+pos;leaf(p,c);while((p/=2)!=0)pull(p);
  }
 }
 return 0;
}
