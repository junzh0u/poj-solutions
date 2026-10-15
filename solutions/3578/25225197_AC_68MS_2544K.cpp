// POJ 3578 - Card Game
// Model: gpt-6.1-sol
// Jack puts every enhancement into apo, maximizing his card lexicographically.
// For equal apo, buy wins in increasing bqu deficit. A cri enhancement can
// save exactly one bqu point; keep the cheapest such savings in a max heap.
// 'Another' is treated as a distinct card: a card's two costs sum to at most M.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
struct Card { int a,b,c; };
int main() {
 int n;
 while (scanf("%d",&n)==1 && n) {
  vector<Card> v(n); int i,m;
  for(i=0;i<n;++i) scanf("%d%d%d",&v[i].a,&v[i].b,&v[i].c);
  scanf("%d",&m);
  int a=v[0].a+m,b=v[0].b,c=v[0].c,ans=0;
  vector<pair<int,int> > costs;
  for(i=1;i<n;++i) {
   if(v[i].a>a || (v[i].a==a && (v[i].b>b || (v[i].b==b && v[i].c>c)))) ++ans;
   else if(v[i].a==a) costs.push_back(make_pair(b-v[i].b,max(0,c-v[i].c+1)));
  }
  sort(costs.begin(),costs.end());
  priority_queue<int> q; int sum=0,base=0,best=0;
  for(i=0;i<(int)costs.size();++i) {
   int d=costs[i].first,e=costs[i].second;
   base+=d+1;
   if(d+e<=m) { q.push(e); sum+=e; }
   while(sum>m) { sum-=q.top(); q.pop(); }
   if(base-(int)q.size()<=m) best=i+1;
  }
  printf("%d\n",ans+best);
 }
 return 0;
}
