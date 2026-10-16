// POJ 3758 - The story of three kingdoms XI
// Model: gpt-6.1-sol
// BFS fixes movement ranges because allies are traversable and dead enemies block.
// Branch and bound assigns attacks and distinct final squares. Destination-to-
// original-owner dependencies must be acyclic, so an action order exists.
// Nonattacking troops are assigned legal final squares too, allowing evacuation.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
struct Unit { int p,t,s; };
struct Action {int p,e,d; Action(int P,int E,int D):p(P),e(E),d(D){} };
Unit a[10],b[10];
int n,m,na,nb,am[3],aa[3],owner[100],terrain[100],hp[10];
int dista[10][100],damage[10][10],dep[10],dest[10],orderA[10];
bool occupied[100];
vector<Action> act[10];
vector<int> reach[10];
int best,allmask;
bool cycle(int i,int p) {
 int j=owner[p],seen=1<<i;
 while(j>=0) { if(seen&(1<<j)) return j!=i || p!=a[i].p; seen|=1<<j; j=dep[j]; }
 return false;
}
bool evacuate(int mask) {
 if(!mask) return true;
 int who=-1,cnt=1000;
 for(int i=0;i<na;++i) if(mask&(1<<i)) {
  int c=0; for(size_t k=0;k<reach[i].size();++k) {int p=reach[i][k];if(!occupied[p]&&!cycle(i,p))++c;}
  if(c<cnt){cnt=c;who=i;}
 }
 if(!cnt) return false;
 // Staying put avoids needless displacement and is usually sufficient.
 for(int pass=0;pass<2;++pass) for(size_t k=0;k<reach[who].size();++k) {
  int p=reach[who][k]; if((p==a[who].p)!=(pass==0))continue;
  if(occupied[p]||cycle(who,p))continue;
  occupied[p]=true; dep[who]=(owner[p]==who?-1:owner[p]);
  if(evacuate(mask^(1<<who))) {occupied[p]=false;dep[who]=-1;return true;}
  occupied[p]=false;dep[who]=-1;
 }
 return false;
}
void dfs(int level,int score,int idle) {
 if(score<=best && level==na)return;
 int upper1=0,upper2=0,possible[10]={0};
 for(int k=level;k<na;++k){int i=orderA[k],v=0;for(int e=0;e<nb;++e){int d=min(hp[e],damage[i][e]);v=max(v,d);possible[e]+=damage[i][e];}upper1+=v;}
 for(int e=0;e<nb;++e)upper2+=min(hp[e],possible[e]);
 if(score+min(upper1,upper2)<=best)return;
 if(level==na){if(evacuate(idle))best=score;return;}
 int i=orderA[level];
 vector<Action> choices;
 for(size_t k=0;k<act[i].size();++k){Action x=act[i][k];x.d=min(x.d,hp[x.e]);if(x.d && !occupied[x.p]&&!cycle(i,x.p))choices.push_back(x);}
 for(size_t k=0;k<choices.size();++k)for(size_t j=k+1;j<choices.size();++j)if(choices[j].d>choices[k].d)swap(choices[k],choices[j]);
 for(size_t k=0;k<choices.size();++k){Action x=choices[k];occupied[x.p]=true;dep[i]=(owner[x.p]==i?-1:owner[x.p]);hp[x.e]-=x.d;dfs(level+1,score+x.d,idle);hp[x.e]+=x.d;dep[i]=-1;occupied[x.p]=false;if(score+min(upper1,upper2)<=best)return;}
 dfs(level+1,score,idle|(1<<i));
}
int main(){
 while(scanf("%d%d%d",am,am+1,am+2)==3&&(am[0]||am[1]||am[2])){
 scanf("%d%d%d",aa,aa+1,aa+2);for(int z=0;z<3;++z)aa[z]=min(aa[z],100);scanf("%d%d",&n,&m);na=nb=0;memset(owner,-1,sizeof(owner));
 for(int p=0;p<n*m;++p){char c;scanf(" %c",&c);terrain[p]=(c=='X');if(c>='A'&&c<='F'){int s;scanf("%d",&s);Unit u;u.p=p;u.t=(c-'A')%3;u.s=s;if(c<='C'){owner[p]=na;a[na++]=u;}else {b[nb++]=u;terrain[p]=1;}}}
 memset(damage,0,sizeof(damage));
 for(int i=0;i<na;++i){act[i].clear();reach[i].clear();memset(dista[i],-1,sizeof(dista[i]));int q[100],head=0,tail=0;q[tail++]=a[i].p;dista[i][a[i].p]=0;
 while(head<tail){int p=q[head++];reach[i].push_back(p);if(dista[i][p]>=am[a[i].t])continue;for(int z=0;z<4;++z){int r=p/m+(z==0)-(z==1),c=p%m+(z==2)-(z==3);if(r<0||r>=n||c<0||c>=m)continue;int v=r*m+c;if(!terrain[v]&&dista[i][v]<0){dista[i][v]=dista[i][p]+1;q[tail++]=v;}}}
 for(int e=0;e<nb;++e)for(size_t k=0;k<reach[i].size();++k){int p=reach[i][k];if(abs(p/m-b[e].p/m)+abs(p%m-b[e].p%m)==1){int d=min(b[e].s,a[i].s*aa[a[i].t]*((a[i].t+1)%3==b[e].t?2:1));act[i].push_back(Action(p,e,d));damage[i][e]=d;}}
 orderA[i]=i;}
 for(int i=0;i<na;++i)for(int j=i+1;j<na;++j){int x=0,y=0;for(int e=0;e<nb;++e){x=max(x,damage[orderA[i]][e]);y=max(y,damage[orderA[j]][e]);}if(y>x)swap(orderA[i],orderA[j]);}
 for(int e=0;e<nb;++e)hp[e]=b[e].s;memset(dep,-1,sizeof(dep));memset(occupied,0,sizeof(occupied));best=-1;dfs(0,0,0);printf("%d\n",max(0,best));
 }
 return 0;
}
