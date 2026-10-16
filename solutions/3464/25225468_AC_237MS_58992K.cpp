// POJ 3464 - Approximations
// Model: gpt-6.1-sol
// Add each precision only on portions uncovered by finer rounding cells.
// Their union is connected because every cell contains the input fraction.
// A sparse decimal trie orders endpoints in linear space and time.
// Standard half-up rounding makes each cell lower-closed, upper-open.
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;
struct Node { int par,child,next,d,lo,hi,rank; Node(int p=-1,int z=0):par(p),child(-1),next(-1),d(z),lo(-2),hi(-2),rank(0){} };
vector<Node> t;
int child(int p,int d) {
 for(int v=t[p].child;v!=-1;v=t[v].next) if(t[v].d==d)return v;
 int v=t.size(); t.push_back(Node(p,d)); t[v].next=t[p].child;t[p].child=v;return v;
}
int adjacent(int v,int dir) {
 int cached=dir<0?t[v].lo:t[v].hi;if(cached!=-2)return cached;
 vector<int> path;int w=v;
 while(w && t[w].d==(dir<0?0:9) && (dir<0?t[w].lo:t[w].hi)==-2) {path.push_back(w);w=t[w].par;}
 int r;
 if(!w)r=-1;else if((dir<0?t[w].lo:t[w].hi)!=-2)r=dir<0?t[w].lo:t[w].hi;else r=child(t[w].par,t[w].d+dir);
 for(int i=(int)path.size()-1;i>=0;--i) {if(r!=-1)r=child(r,dir<0?9:0);if(dir<0)t[path[i]].lo=r;else t[path[i]].hi=r;}
 if(dir<0)t[v].lo=r;else t[v].hi=r;return r;
}
struct Event {int pos,id,k,add;Event(int p,int i,int z,int a):pos(p),id(i),k(z),add(a){} };
vector<Event> ev;
bool cmp(const Event&a,const Event&b){if(t[a.pos].rank!=t[b.pos].rank)return t[a.pos].rank<t[b.pos].rank;return a.add<b.add;}
char buf[400010];
int main(){int n;if(scanf("%d",&n)!=1)return 0;t.reserve(1600010);t.push_back(Node());int zero=child(0,0);
 for(int i=0;i<n;++i){scanf("%400009s",buf);string s(buf);int v=zero;for(int k=1;k<(int)s.size()-1;++k){v=child(v,s[k+1]-'0');bool up=k+2<(int)s.size()&&s[k+2]>='5';int l=up?v:adjacent(v,-1);int r=up?adjacent(v,1):v;int a=l<0?zero:child(l,5),b=child(r,5);ev.push_back(Event(a,i,k,1));ev.push_back(Event(b,i,k,0));}}
 // Numeric order: a finite prefix precedes its extensions. Endpoints never
 // differ only by trailing zeroes, except zero itself, which comes first.
 vector<int> st;st.push_back(0);int rank=0;
 while(!st.empty()){int v=st.back();st.pop_back();t[v].rank=rank++;int c[10];fill(c,c+10,-1);for(int w=t[v].child;w!=-1;w=t[w].next)c[t[w].d]=w;for(int d=9;d>=0;--d)if(c[d]!=-1)st.push_back(c[d]);}
 vector<int> delta(t.size()+1,0);
 int id=-1,L=0,R=0;
 for(int j=(int)ev.size()-2;j>=0;j-=2){Event e=ev[j];int l=t[e.pos].rank,r=t[ev[j+1].pos].rank;
 if(e.id!=id){id=e.id;L=l;R=r;delta[l]+=e.k;delta[r]-=e.k;}
 else {if(l<L){delta[l]+=e.k;delta[L]-=e.k;L=l;}if(r>R){delta[R]+=e.k;delta[r]-=e.k;R=r;}}}
 int sum=0,ans=0;for(size_t j=0;j<delta.size();++j){sum+=delta[j];if(sum>ans)ans=sum;}printf("%d\n",ans);
}
