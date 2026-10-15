// POJ 3975 - Shortcut
// Model: gpt-6.1-sol
// Sort break points along each axis and inspect neighboring aligned points.
// A shortcut must save distance, so an unchanged straight subroute is excluded.
// Any non-neighbor candidate contains a shorter shortcut, unless its entire
// subroute is straight. Compare length, earliest beginning, then latest ending.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
using namespace std;
struct Point { int x,y,id; };
bool byX(const Point&a,const Point&b) { return a.x!=b.x?a.x<b.x:a.y<b.y; }
bool byY(const Point&a,const Point&b) { return a.y!=b.y?a.y<b.y:a.x<b.x; }
int best,beginning,ending; char direction;
void consider(Point a,Point b) {
 if(a.id>b.id) swap(a,b);
 int len=abs(a.x-b.x)+abs(a.y-b.y);
 if(len>=b.id-a.id) return;
 if(len<best || (len==best && (a.id<beginning || (a.id==beginning && b.id>ending)))) {
  best=len; beginning=a.id; ending=b.id;
  direction=a.x==b.x?(a.y<b.y?'N':'S'):(a.x<b.x?'E':'W');
 }
}
char route[250005];
int main() {
 int t,n; scanf("%d",&t);
 while(t--) {
  scanf("%d%s",&n,route); vector<Point> p(n+1); p[0].x=p[0].y=p[0].id=0;
  for(int i=1;i<=n;++i) {
   p[i]=p[i-1]; p[i].id=i;
   if(route[i-1]=='N') ++p[i].y;
   if(route[i-1]=='S') --p[i].y;
   if(route[i-1]=='E') ++p[i].x;
   if(route[i-1]=='W') --p[i].x;
  }
  best=n+1; beginning=n+1; ending=-1; direction='N';
  sort(p.begin(),p.end(),byX);
  for(int i=1;i<=n;++i) if(p[i].x==p[i-1].x) consider(p[i-1],p[i]);
  sort(p.begin(),p.end(),byY);
  for(int i=1;i<=n;++i) if(p[i].y==p[i-1].y) consider(p[i-1],p[i]);
  printf("%d %d %d %c\n",best,beginning,ending,direction);
 }
 return 0;
}
