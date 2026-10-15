// POJ 2730 - F1 Circuit Design
// Model: gpt-6.1-sol
// Trace the complete directed circuit, respecting straight orientations and
// the simple/composite turn figures. Propagate squared-speed bounds over
// three laps, then integrate the optimal accelerate/brake profile in lap two.
// Blank map lines are preserved; the next numeric line starts the next case.
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdio>
#include <algorithm>
using namespace std;
char g[60][110]; bool used[60][110];
int dr[4]={-1,0,1,0}, dc[4]={0,1,0,-1};
int sr,sc,sd,total; vector<pair<int,int> > path;
bool walk(int r,int c,int d,int prevplus) {
 if(r==sr && c==sc) return (int)path.size()==total && d==sd;
 if(r<0||r>=60||c<0||c>=110||used[r][c]) return false;
 char ch=g[r][c];
 if(ch!='+' && ch!='-' && ch!='|') return false;
 if(ch=='-' && d%2==0) return false;
 if(ch=='|' && d%2==1) return false;
 used[r][c]=true; path.push_back(make_pair(r,c));
 if(ch=='+') {
  for(int k=1;k<=3;k+=2) {
   int nd=(d+k)%4,nr=r+dr[nd],nc=c+dc[nd];
   if(nr<0||nr>=60||nc<0||nc>=110) continue;
   if(prevplus && g[nr][nc]=='+') continue;
   if(walk(nr,nc,nd,1)) return true;
  }
 } else if(walk(r+dr[d],c+dc[d],d,0)) return true;
 used[r][c]=false; path.pop_back(); return false;
}
bool number(const string &s,int &x) {
 istringstream in(s); string extra; return (in>>x) && !(in>>extra);
}
double solve(const vector<string>&a,int m) {
 total=0; sr=sc=sd=0;
 for(int r=0;r<60;++r) for(int c=0;c<110;++c) {g[r][c]=' ';used[r][c]=false;}
 for(int r=0;r<(int)a.size()&&r<58;++r) for(int c=0;c<(int)a[r].size()&&c<108;++c) {
  char ch=a[r][c]; g[r+1][c+1]=ch;
  if(ch=='+'||ch=='-'||ch=='|') ++total;
  string dirs="^>v<"; size_t p=dirs.find(ch);
  if(p!=string::npos) {sr=r+1;sc=c+1;sd=(int)p;++total;}
 }
 path.clear(); path.push_back(make_pair(sr,sc)); used[sr][sc]=true;
 if(!walk(sr+dr[sd],sc+dc[sd],sd,0)) return 0;
 int n=path.size(),N=3*n; vector<double> len(N),v(N+1,1e50);
 for(int i=0;i<N;++i) {
  int j=i%n; char ch=g[path[j].first][path[j].second];
  len[i]=(ch=='-'||ch=='|')?m:0;
  if(ch=='+') {
   int before=(j+n-1)%n,after=(j+1)%n;
   bool composite=g[path[before].first][path[before].second]=='+' || g[path[after].first][path[after].second]=='+';
   double cap=composite?400:2025;
   v[i]=min(v[i],cap); v[i+1]=min(v[i+1],cap);
  }
 }
 v[0]=v[N]=0;
 for(int i=0;i<N;++i) v[i+1]=min(v[i+1],v[i]+20*len[i]);
 for(int i=N-1;i>=0;--i) v[i]=min(v[i],v[i+1]+60*len[i]);
 double ans=0;
 for(int i=n;i<2*n;++i) if(len[i]>0) {
  double u=sqrt(v[i]),w=sqrt(v[i+1]);
  double peak=sqrt((60*v[i]+20*v[i+1]+1200*len[i])/80);
  ans+=(peak-u)/10+(peak-w)/30;
 }
 return ans;
}
int main(){
 vector<string> all; string s; while(getline(cin,s)){if(!s.empty()&&s[s.size()-1]=='\r')s.erase(s.size()-1);all.push_back(s);}
 for(int i=0;i<(int)all.size();) {
  int m; if(!number(all[i++],m))continue; if(!m)break;
  vector<string>a; int next;
  while(i<(int)all.size()&&!number(all[i],next))a.push_back(all[i++]);
  printf("%.2f\n",solve(a,m));
 }
}
