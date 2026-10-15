// POJ 3142 - Escape Plan
// Model: gpt-6.1-sol
// Expand time modulo the tunnel-period LCM. Reverse Dijkstra supplies exact
// remaining times; best-first enumeration retains only the needed prefixes.
// Different legal departure times count separately, even on the same route.
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
using namespace std;
typedef long long LL;
const LL INF = (LL)1000000000*1000000000;
struct Edge { int u,v,c,w; };
struct Item { LL d; int s; Item(LL a,int b):d(a),s(b){} bool operator<(const Item& b)const {return d>b.d;} };
struct Path { LL f,g; int s; LL serial; Path(LL a,LL b,int c,LL d):f(a),g(b),s(c),serial(d){} bool operator<(const Path& b)const {if(f!=b.f)return f<b.f;return serial<b.serial;} };
int gcd(int a,int b){return b?gcd(b,a%b):a;}
int main(){
 ios::sync_with_stdio(false);cin.tie(0);
 int n,m,k,t,cas=0;
 while(cin>>n>>m>>k>>t && n){
  vector<Edge> e(m);vector<vector<int> > in(n),out(n);int p=1;
  for(int i=0;i<m;++i){cin>>e[i].u>>e[i].v>>e[i].c>>e[i].w;in[e[i].v].push_back(i);out[e[i].u].push_back(i);p=p/gcd(p,e[i].c)*e[i].c;}
  vector<LL> h(n*p,INF);priority_queue<Item> q;
  for(int r=0;r<p;++r){int s=(n-1)*p+r;h[s]=0;q.push(Item(0,s));}
  while(!q.empty()){
   Item z=q.top();q.pop();if(z.d!=h[z.s])continue;
   int v=z.s/p,r=z.s%p;
   for(unsigned i=0;i<in[v].size();++i){Edge a=e[in[v][i]];
    int dep=(r-a.w%p+p)%p;if(dep%a.c)continue;
    for(int wait=0;wait<=t;++wait){int prev=(dep-wait%p+p)%p,s=a.u*p+prev;LL d=z.d+a.w+wait;
     if(d<h[s]){h[s]=d;q.push(Item(d,s));}
    }
   }
  }
  LL ans=-1;int need=k+1;LL serial=0;
  multiset<Path> frontier;if(h[0]!=INF)frontier.insert(Path(h[0],0,0,serial++));
  while(!frontier.empty()){
   Path z=*frontier.begin();frontier.erase(frontier.begin());int u=z.s/p,r=z.s%p;
   if(u==n-1){if(--need==0){ans=z.g;break;}}
   for(unsigned i=0;i<out[u].size();++i){Edge a=e[out[u][i]];int first=(a.c-r%a.c)%a.c;
    for(int wait=first;wait<=t;wait+=a.c){LL g=z.g+wait+a.w;int s=a.v*p+(int)(g%p);if(h[s]==INF)continue;
     frontier.insert(Path(g+h[s],g,s,serial++));
     if((int)frontier.size()>need){multiset<Path>::iterator it=frontier.end();--it;frontier.erase(it);}
    }
   }
  }
  cout<<"Case "<<++cas<<": "<<ans<<'\n';
 }
}
