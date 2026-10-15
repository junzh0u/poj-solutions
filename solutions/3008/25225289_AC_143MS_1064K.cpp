// POJ 3008 - Hexerpents of Hexwamp
// Model: gpt-6.1-sol
// A* over complete serpent configurations. A reverse BFS of the head and
// neck alone supplies an admissible obstacle-aware heuristic. Each step
// enumerates independent moving sections and rejects all new self-contacts.
// Coordinates use the six neighbors shown in the statement's figure.
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <ext/hash_map>
using namespace std;
typedef unsigned long long Key;
struct Hash { size_t operator()(Key k) const { return (size_t)(k^(k>>23)); } };
const int dx[6]={1,0,-1,-1,0,1},dy[6]={0,1,1,0,-1,-1};
struct P { int x,y; P(int a=0,int b=0):x(a),y(b){} };
P operator+(P a,P b){return P(a.x+b.x,a.y+b.y);}
int dist(P a,P b){int x=a.x-b.x,y=a.y-b.y;return (abs(x)+abs(y)+abs(x+y))/2;}
bool rock[65][65]; int h[65][65][6],n;
bool freecell(P p){return p.x>=0&&p.x<65&&p.y>=0&&p.y<65&&!rock[p.x][p.y];}
int dir(P a,P b){for(int d=0;d<6;d++)if(a.x+dx[d]==b.x&&a.y+dy[d]==b.y)return d;return -1;}
Key encode(P *p){Key k=p[0].x*65+p[0].y;for(int i=1;i<n;i++)k=k*6+dir(p[i-1],p[i]);return k;}
void decode(Key k,P *p){int d[8];for(int i=n-1;i>0;i--){d[i]=k%6;k/=6;}p[0]=P(k/65,k%65);for(int i=1;i<n;i++)p[i]=p[i-1]+P(dx[d[i]],dy[d[i]]);}
struct Node{Key key;int g,f;Node(Key k,int a,int b):key(k),g(a),f(b){}bool operator<(const Node&o)const{return f!=o.f?f>o.f:g<o.g;}};
priority_queue<Node> pq; __gnu_cxx::hash_map<Key,int,Hash> best;
P oldp[8],np[8]; int currentG;
void generate(int i,bool moved,bool any){
 if(i==n){if(!any)return;int d=dir(np[0],np[1]);int hv=h[np[0].x][np[0].y][d];if(hv<0||currentG+1+hv>20)return;Key k=encode(np);int ng=currentG+1;__gnu_cxx::hash_map<Key,int,Hash>::iterator it=best.find(k);if(it==best.end()||ng<it->second){best[k]=ng;pq.push(Node(k,ng,ng+hv));}return;}
 for(int c=-1;c<6;c++){
  bool move=c>=0;if(move&&moved)continue;P q=oldp[i];if(move)q=q+P(dx[c],dy[c]);if(!freecell(q))continue;
  if(move&&i+1<n&&dist(q,oldp[i+1])!=1)continue;
  bool ok=true;for(int j=0;j<i;j++)if((j==i-1&&dist(q,np[j])!=1) || (j<i-1&&dist(q,np[j])<=1)){ok=false;break;}
  if(!ok)continue;np[i]=q;generate(i+1,move,any||move);
 }
}
void heuristic(){memset(h,-1,sizeof(h));queue<int> q;for(int d=0;d<6;d++){P b=P(32,32)+P(dx[d],dy[d]);if(freecell(b)){h[32][32][d]=0;q.push((32*65+32)*6+d);}}
 while(!q.empty()){int k=q.front();q.pop();int d=k%6;k/=6;P a(k/65,k%65),b=a+P(dx[d],dy[d]);int hv=h[a.x][a.y][d];if(hv==20)continue;
 for(int who=0;who<2;who++)for(int c=0;c<6;c++){P aa=a,bb=b;if(who==0)aa=aa+P(dx[c],dy[c]);else bb=bb+P(dx[c],dy[c]);int nd=dir(aa,bb);if(nd<0||!freecell(aa)||!freecell(bb)||h[aa.x][aa.y][nd]>=0)continue;h[aa.x][aa.y][nd]=hv+1;q.push((aa.x*65+aa.y)*6+nd);}}
}
int main(){while(scanf("%d",&n)==1&&n){P initial[8],rocks[100];for(int i=0;i<n;i++)scanf("%d%d",&initial[i].x,&initial[i].y);int k;scanf("%d",&k);for(int i=0;i<k;i++)scanf("%d%d",&rocks[i].x,&rocks[i].y);int x,y;scanf("%d%d",&x,&y);memset(rock,0,sizeof(rock));for(int i=0;i<k;i++){P p(rocks[i].x-x+32,rocks[i].y-y+32);if(p.x>=0&&p.x<65&&p.y>=0&&p.y<65)rock[p.x][p.y]=true;}for(int i=0;i<n;i++){initial[i].x+=32-x;initial[i].y+=32-y;}heuristic();best.clear();while(!pq.empty())pq.pop();Key start=encode(initial);int hv=h[initial[0].x][initial[0].y][dir(initial[0],initial[1])];best[start]=0;pq.push(Node(start,0,hv));int ans=-1;while(!pq.empty()){Node t=pq.top();pq.pop();if(best[t.key]!=t.g)continue;decode(t.key,oldp);if(oldp[0].x==32&&oldp[0].y==32){ans=t.g;break;}currentG=t.g;generate(0,false,false);}printf("%d\n",ans);}return 0;}
