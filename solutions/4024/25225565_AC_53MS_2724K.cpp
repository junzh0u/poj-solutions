// POJ 4024 - Caption
// Model: gpt-6.1-sol
// Render the old caption, then minimize pixel changes with a placement DP.
// Blank columns have a prefix cost; each glyph contributes its XOR cost.
// The recovered NEERC statement permits any optimum. For deterministic output,
// ties choose the smallest final position and then the smallest preceding gap.
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;
unsigned int font[256][30], board[2000];
int cost[256][2000], pref[2001], dp[30][2000], gap[30][2000];
int bits(unsigned int x) { int s=0; while(x) {x&=x-1; ++s;} return s; }
void line(string &s) { getline(cin,s); if(!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1); }
int main() {
 int m,n,k,lo,hi; if(!(cin>>m>>n>>k>>lo>>hi)) return 0;
 string s,oldtext,newtext; line(s);
 string rows[30]; for(int r=0;r<m;++r) line(rows[r]);
 int t=(rows[0].size()+1)/(k+3);
 for(int i=0;i<t;++i) { unsigned char c=rows[0][i*(k+3)];
  for(int r=0;r<m;++r) for(int j=0;j<k;++j)
   if(rows[r][i*(k+3)+2+j]=='*') font[c][j]|=1U<<r;
 }
 line(oldtext); int pos=0;
 for(int i=0;i<(int)oldtext.size();++i) {int x;cin>>x;pos+=x;
  for(int j=0;j<k;++j) board[pos+j]=font[(unsigned char)oldtext[i]][j]; pos+=k;
 }
 line(s);line(newtext);
 for(int j=0;j<n;++j) pref[j+1]=pref[j]+bits(board[j]);
 memset(cost,0,sizeof(cost)); bool seen[256]={false};
 for(int i=0;i<(int)newtext.size();++i) {unsigned char c=newtext[i]; if(seen[c])continue;seen[c]=true;
  for(int p=0;p+k<=n;++p) for(int j=0;j<k;++j)cost[c][p]+=bits(board[p+j]^font[c][j]);
 }
 const int INF=100000000;
 int len=newtext.size();
 for(int i=0;i<len;++i) for(int p=0;p+k<=n;++p) {
  dp[i][p]=INF;gap[i][p]=-1;
  if(i==0) {dp[i][p]=pref[p]+cost[(unsigned char)newtext[i]][p];gap[i][p]=p;}
  else for(int g=lo;g<=hi && g+k<=p;++g) {
   int q=p-k-g;
   int v=dp[i-1][q]+pref[p]-pref[q+k]+cost[(unsigned char)newtext[i]][p];
   if(v<dp[i][p]) {dp[i][p]=v;gap[i][p]=g;}
  }
 }
 int best=INF,at=0;for(int p=0;p+k<=n;++p) {int v=dp[len-1][p]+pref[n]-pref[p+k];if(v<best){best=v;at=p;}}
 int ans[30];for(int i=len-1;i>=0;--i) {ans[i]=gap[i][at];at-=k+ans[i];}
 for(int i=0;i<len;++i) {if(i)cout<<' ';cout<<ans[i];}cout<<'\n';
 return 0;
}
