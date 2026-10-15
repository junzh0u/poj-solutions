// POJ 2995 - Go Endgame
// Model: gpt-6.1-sol
// Claim Alice's sente regions, then compare keeping the first gote move
// with taking the largest reverse sente. Gote regions are processed by
// descending combined value; equal values split by Alice's score.
// The official CTU reference resolves reverse-sente ties by Bob's score.
// Its scoring convention is followed here, including secondary-score ties.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Region { int a,b; Region(int x=0,int y=0):a(x),b(y) {} };
bool order(const Region& x,const Region& y) {
 if(x.a+x.b!=y.a+y.b) return x.a+x.b>y.a+y.b;
 return x.a>y.a;
}
Region play(const vector<Region>& g,int turn) {
 Region r;
 for(int i=0;i<(int)g.size();) {
  int j=i;
  while(j<(int)g.size() && g[j].a+g[j].b==g[i].a+g[i].b) ++j;
  int k=(j-i+(turn==0))/2;
  for(int p=i;p<i+k;++p) r.a+=g[p].a;
  for(int p=i+k;p<j;++p) r.b+=g[p].b;
  turn^=(j-i)%2;i=j;
 }
 return r;
}
int main() {
 int a,b,n;
 while(scanf("%d%d%d",&a,&b,&n)==3) {
  vector<Region> g; Region rev; char s[3],t[3];
  for(int i=0;i<n;++i) {
   int x,y;scanf("%d%d%s%s",&x,&y,s,t);
   if(s[0]=='S') a+=x;
   else if(t[0]=='S') {
    b+=y;
    if(x+y>rev.a+rev.b || (x+y==rev.a+rev.b && y>rev.b)) rev=Region(x,y);
   } else g.push_back(Region(x,y));
  }
  sort(g.begin(),g.end(),order);
  Region p=play(g,0),q=play(g,1);
  p.a+=a;p.b+=b;q.a+=a+rev.a;q.b+=b-rev.b;
  if(q.a-q.b>p.a-p.b || (q.a-q.b==p.a-p.b && q.a>p.a)) p=q;
  printf("%d %d\n",p.a,p.b);
 }
 return 0;
}
