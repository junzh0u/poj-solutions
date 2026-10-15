// POJ 1162 - Building with Blocks
// Model: gpt-6.1-sol
// Enumerate all connected subsets of at most four cubes, then use iterative
// deepening exact cover. Component volume bounds prune impossible remainders.
// All block types are permitted; only the minimum count is required by POJ.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
typedef unsigned long long Mask;
int n,x[50],y[50],z[50];
Mask adj[50];
vector<Mask> blocks[50];
set<Mask> shapes;
int countbits(Mask a) { int c=0; while(a) { a&=a-1; ++c; } return c; }
int first(Mask a) { int i=0; while(!(a&1)) { a>>=1; ++i; } return i; }
void generate(Mask a, Mask edge, int size) {
    if(!shapes.insert(a).second || size==4) return;
    Mask e=edge & ~a;
    while(e) { int v=first(e); Mask b=Mask(1)<<v; e^=b;
        generate(a|b,edge|adj[v],size+1);
    }
}
bool larger(Mask a,Mask b) { return countbits(a)>countbits(b); }
bool search(Mask left,int slots) {
    if(!left) return true;
    if(slots<=0 || countbits(left)>slots*4) return false;
    Mask un=left;
    int lower=0;
    while(un) {
        Mask comp=un & -un, frontier=comp;
        un^=comp;
        while(frontier) {
            int v=first(frontier); frontier^=Mask(1)<<v;
            Mask more=adj[v]&un; un^=more; comp|=more; frontier|=more;
        }
        lower+=(countbits(comp)+3)/4;
        if(lower>slots) return false;
    }
    int chosen=-1,best=100000;
    for(int v=0;v<n;++v) if(left&(Mask(1)<<v)) {
        int choices=0;
        for(size_t j=0;j<blocks[v].size();++j) {
            Mask a=blocks[v][j];
            if((a&left)==a && countbits(left)-countbits(a)<=4*(slots-1)) ++choices;
        }
        if(!choices) return false;
        if(choices<best) { best=choices; chosen=v; if(best==1) break; }
    }
    for(size_t j=0;j<blocks[chosen].size();++j) {
        Mask a=blocks[chosen][j];
        if((a&left)==a && search(left^a,slots-1)) return true;
    }
    return false;
}
int main() {
    if(scanf("%d",&n)!=1) return 0;
    for(int i=0;i<n;++i) scanf("%d%d%d",x+i,y+i,z+i);
    for(int i=0;i<n;++i) for(int j=0;j<n;++j)
        if(abs(x[i]-x[j])+abs(y[i]-y[j])+abs(z[i]-z[j])==1) adj[i]|=Mask(1)<<j;
    for(int i=0;i<n;++i) generate(Mask(1)<<i,adj[i],1);
    for(set<Mask>::iterator it=shapes.begin();it!=shapes.end();++it)
        for(int i=0;i<n;++i) if(*it&(Mask(1)<<i)) blocks[i].push_back(*it);
    for(int i=0;i<n;++i) sort(blocks[i].begin(),blocks[i].end(),larger);
    Mask all=(Mask(1)<<n)-1;
    int answer=(n+3)/4;
    while(!search(all,answer)) ++answer;
    printf("%d\n",answer);
    return 0;
}
