// POJ 3932 - Matches
// Model: gpt-6.1-sol
// Column DP tracks carry and exactly one removed and one added segment.
// Leading zeroes and original digit widths are preserved; absent columns
// are arithmetic zeroes but cannot receive or donate a match.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Opt { int d,r,a; Opt(int D,int R,int A):d(D),r(R),a(A){} };
struct Step { int next,code; Step(int N,int C):next(N),code(C){} };
vector<Opt> opts[11];
vector<Step> trans[1331][8];
int bits(int x) { int n=0; while(x) { n+=x&1; x>>=1; } return n; }
int main() {
 int mask[10]={63,6,91,79,102,109,125,7,127,111};
 for(int x=0;x<10;++x) for(int y=0;y<10;++y) {
  int r=bits(mask[x]&~mask[y]),a=bits(mask[y]&~mask[x]);
  if(r<=1&&a<=1) opts[x].push_back(Opt(y,r,a));
 }
 opts[10].push_back(Opt(0,0,0));
 for(int x=0;x<11;++x) for(int y=0;y<11;++y) for(int z=0;z<11;++z) {
  int key=(x*11+y)*11+z;
  for(int s=0;s<8;++s) for(unsigned i=0;i<opts[x].size();++i)
   for(unsigned j=0;j<opts[y].size();++j) for(unsigned k=0;k<opts[z].size();++k) {
    Opt p=opts[x][i],q=opts[y][j],t=opts[z][k];
    int r=(s>>2)+p.r+q.r+t.r,a=((s>>1)&1)+p.a+q.a+t.a;
    int sum=p.d+q.d+(s&1);
    if(r>1||a>1||sum%10!=t.d) continue;
    trans[key][s].push_back(Step(r*4+a*2+sum/10,p.d+q.d*10+t.d*100));
   }
 }
 string a,b,c,op,eq;
 while(cin>>a>>op>>b>>eq>>c) {
  int n=max(a.size(),max(b.size(),c.size()));
  vector<int> prev((n+1)*8,-1); prev[0]=0;
  for(int i=0;i<n;++i) {
   int x=i<(int)a.size()?a[a.size()-1-i]-'0':10;
   int y=i<(int)b.size()?b[b.size()-1-i]-'0':10;
   int z=i<(int)c.size()?c[c.size()-1-i]-'0':10;
   int key=(x*11+y)*11+z;
   for(int s=0;s<8;++s) if(prev[i*8+s]>=0) {
    const vector<Step>& v=trans[key][s];
    for(unsigned j=0;j<v.size();++j) prev[(i+1)*8+v[j].next]=v[j].code*8+s;
   }
  }
  int s=6;
  if(prev[n*8+s]<0) return 1;
  for(int i=n-1;i>=0;--i) {
   int code=prev[(i+1)*8+s]; s=code%8; code/=8;
   if(i<(int)a.size()) a[a.size()-1-i]=char('0'+code%10); code/=10;
   if(i<(int)b.size()) b[b.size()-1-i]=char('0'+code%10); code/=10;
   if(i<(int)c.size()) c[c.size()-1-i]=char('0'+code%10);
  }
  cout<<a<<" + "<<b<<" = "<<c<<"\n*****************\n";
 }
}
