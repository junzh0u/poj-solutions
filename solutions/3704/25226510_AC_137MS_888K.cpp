// POJ 3704 - Mover
// Model: gpt-6-astra
// Grow a central largest component by pulling nearest pieces through empty
// cells. Update only strictly improved distances; prefer newer roots when
// reconstructing a shortest path. Maintain the empty frontier and discard
// pieces once collected, avoiding repeated scans of completed regions.
// On equal distances prefer a landing cell joining more outside neighbors.
// First try a two-step bridge that preserves an outside attachment point.
// Intermediate outside components need not remain connected.
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC optimize("O3")
#endif
#include <cstdio>
#include <vector>
#include <cstdlib>
using namespace std;
struct Move{int a,b;Move(int x,int y):a(x),b(y){}};
int n,m,a[10000],joined[10000],d[10000],parent[10000],land[10000],root[10000],age[10000];
int q[2000000],tick;
vector<int> component,pieces,frontier;
int added[10000];
vector<Move> moves;
int nb(int v,int k){if(k==0)return v>=n?v-n:-1;if(k==1)return v+n<n*n?v+n:-1;if(k==2)return v%n?v-1:-1;return v%n+1<n?v+1:-1;}
void absorb(int s){int h=0,t=0;q[t++]=s;joined[s]=1;while(h<t){int v=q[h++];component.push_back(v);for(int k=0;k<4;k++){int w=nb(v,k);if(w>=0&&a[w]&&!joined[w]){joined[w]=1;q[t++]=w;}}}}
void update(int begin){if((int)component.size()==m)return;int h=0,t=0;for(int i=begin;i<(int)component.size();i++){int v=component[i];d[v]=0;root[v]=v;age[v]=++tick;q[t++]=v;}
 while(h<t){int v=q[h++];for(int k=0;k<4;k++){int w=nb(v,k);if(w>=0&&d[w]>d[v]+1){d[w]=d[v]+1;root[w]=root[v];parent[w]=v;land[w]=d[w]==1?w:land[v];if(d[w]==1&&!added[w]){added[w]=1;frontier.push_back(w);}q[t++]=w;}}}}
int main(){if(scanf("%d%d",&n,&m)!=2)return 0;for(int i=0,x,y;i<m;i++){scanf("%d%d",&x,&y);int v=(x-1)*n+y-1;a[v]=1;pieces.push_back(v);}
 vector<int> best;for(int s=0;s<n*n;s++)if(a[s]&&!joined[s]){component.clear();absorb(s);if(component.size()>best.size()||(component.size()==best.size()&&abs(s/n-n/2)+abs(s%n-n/2)<abs(best[0]/n-n/2)+abs(best[0]%n-n/2)))best=component;}
 for(int i=0;i<n*n;i++){joined[i]=0;d[i]=100000;}component=best;for(int i=0;i<(int)component.size();i++)joined[component[i]]=1;update(0);
 while((int)component.size()<m){
  bool bridged=false;
  for(int fi=0;fi<(int)frontier.size()&&!bridged;fi++){int dest=frontier[fi];if(a[dest]){frontier[fi]=frontier.back();frontier.pop_back();--fi;continue;}
   int protect=-1;for(int k=0;k<4;k++){int w=nb(dest,k);if(w>=0&&a[w]&&!joined[w])protect=w;}
   if(protect<0)continue;
   for(int k=0;k<4&&!bridged;k++){int mid=nb(dest,k);if(mid<0||a[mid])continue;
    for(int z=0;z<4;z++){int src=nb(mid,z);if(src<0||src==protect||!a[src]||joined[src])continue;
     moves.push_back(Move(src,mid));moves.push_back(Move(mid,dest));a[src]=0;a[dest]=1;
     int begin=component.size();absorb(dest);update(begin);bridged=true;break;
    }
   }
  }
  if(bridged)continue;
  int found=-1,bd=100000,bs=-1,ba=-1;
  for(int i=0;i<(int)pieces.size();i++){int v=pieces[i];if(!a[v]||joined[v]){pieces[i]=pieces.back();pieces.pop_back();--i;continue;}if(d[v]>bd)continue;int dest=land[v],sc=0;
   for(int k=0;k<4;k++){int w=nb(dest,k);if(w>=0&&w!=v&&a[w]&&!joined[w])sc++;}
   int ag=age[parent[dest]];
   if(d[v]<bd||sc>bs||(sc==bs&&ag>ba)){found=v;bd=d[v];bs=sc;ba=ag;}
  }
  int v=found;while(d[v]>1){int w=parent[v];for(int k=0;k<4;k++){int u=nb(v,k);if(u>=0&&d[u]==d[v]-1&&age[root[u]]>age[root[w]])w=u;}moves.push_back(Move(v,w));a[v]=0;a[w]=1;v=w;}
  int begin=component.size();absorb(v);update(begin);
 }
 printf("%d\n",(int)moves.size());for(int i=0;i<(int)moves.size();i++)printf("%d %d %d %d\n",moves[i].a/n+1,moves[i].a%n+1,moves[i].b/n+1,moves[i].b%n+1);
}
