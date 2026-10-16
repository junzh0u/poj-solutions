// POJ 1793 - Storehouse
// Model: gpt-6-astra
// Offline optimal replacement: evict the goods used farthest in the future.
// An indexed max-heap stores bays, with the next request as its priority.
// Initially unused bays are filled first; end-of-day goods need no removal.
// O(N log B + G) time and O(N + G + B) memory.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int MAX=1000005;
int req[MAX],nxt[MAX],last[MAX],where[MAX];
int heap[1005],pos[1005],key[1005],goods[1005],sz;
void exchange(int a,int b) { swap(heap[a],heap[b]); pos[heap[a]]=a; pos[heap[b]]=b; }
void up(int p) { while(p>1 && key[heap[p]]>key[heap[p/2]]) { exchange(p,p/2); p/=2; } }
void down(int p) { for(;;) { int c=p*2; if(c>sz) break; if(c<sz && key[heap[c+1]]>key[heap[c]]) ++c; if(key[heap[p]]>=key[heap[c]]) break; exchange(p,c); p=c; } }
int readint() { int c=getchar(),x=0; while(c<=32 && c!=EOF) c=getchar(); while(c>32 && c!=EOF) { x=x*10+c-'0'; c=getchar(); } return x; }
int main() {
 int T=readint();
 for(int tc=1;tc<=T;++tc) {
  int B=readint(),G=readint(),N=readint();
  for(int i=0;i<N;++i) req[i]=readint();
  fill(last,last+G+1,N); fill(where,where+G+1,0);
  for(int i=N-1;i>=0;--i) { nxt[i]=last[req[i]]; last[req[i]]=i; }
  sz=0;
  if(tc>1) puts(""); printf("Case %d:\n",tc);
  for(int i=0;i<N;++i) {
   int g=req[i],b=where[g];
   if(b) { puts("NO ACTION"); key[b]=nxt[i]; up(pos[b]); }
   else {
    if(sz<B) { b=sz+1; ++sz; heap[sz]=b; pos[b]=sz; key[b]=nxt[i]; up(sz); }
    else { b=heap[1]; where[goods[b]]=0; key[b]=nxt[i]; down(1); }
    goods[b]=g; where[g]=b; printf("LOAD %d %d\n",b,g);
   }
  }
 }
 return 0;
}
