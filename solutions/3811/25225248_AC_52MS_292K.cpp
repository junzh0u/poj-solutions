// POJ 3811 - Infected Land
// Model: gpt-6.1-sol
// Breadth-first search over infection masks and vehicle positions.
// Move first; the new vehicle cell contributes a live neighbor but is
// forced uninfected after the simultaneous update. Bit planes count neighbors.
#include <cstdio>
#include <vector>
using namespace std;
typedef unsigned int U;
struct Seen {
    vector<U> t; unsigned used;
    Seen():t(1024,0),used(0) {}
    unsigned hash(U x) { x^=x>>16; x*=0x7feb352dU; x^=x>>15; return x; }
    bool insert(U state) {
        U key=state+1; unsigned i=hash(key)&(t.size()-1);
        while(t[i] && t[i]!=key) i=(i+1)&(t.size()-1);
        if(t[i]) return false;
        t[i]=key; ++used;
        if(used*10>t.size()*7) {
            vector<U> old; old.swap(t); t.assign(old.size()*2,0);
            for(unsigned j=0;j<old.size();++j) if(old[j]) {
                i=hash(old[j])&(t.size()-1);
                while(t[i]) i=(i+1)&(t.size()-1);
                t[i]=old[j];
            }
        }
        return true;
    }
};
int n,sz; U full,leftcol,rightcol; vector<int> adj[25];
U evolve(U infected,int pos) {
    U b=infected|(1U<<pos), l=(b&~leftcol)>>1, r=(b&~rightcol)<<1;
    U a[8]={l,r,b<<n,b>>n,l<<n,l>>n,r<<n,r>>n};
    U one=0,two=0,four=0,eight=0;
    for(int k=0;k<8;++k) {
        U c=one&a[k]; one^=a[k];
        U d=two&c; two^=c;
        U e=four&d; four^=d; eight^=e;
    }
    U eq2=~one&two&~four&~eight, eq3=one&two&~four&~eight;
    return (eq3|(infected&eq2))&full&~(1U<<pos);
}
int solve(U mask,int pos) {
    if(!mask) return 0;
    Seen seen; vector<U> q; U start=(mask<<5)|pos;
    seen.insert(start); q.push_back(start); unsigned head=0; int distance=0;
    while(head<q.size()) {
        unsigned end=q.size(); ++distance;
        while(head<end) {
            U state=q[head++], m=state>>5; int p=state&31;
            for(unsigned j=0;j<adj[p].size();++j) {
                int v=adj[p][j]; if(m&(1U<<v)) continue;
                U next=evolve(m,v); if(!next) return distance;
                U code=(next<<5)|v;
                if(seen.insert(code)) q.push_back(code);
            }
        }
    }
    return -1;
}
int main() {
    while(scanf("%d",&n)==1 && n) {
        sz=n*n; full=(1U<<sz)-1; leftcol=rightcol=0;
        U mask=0; int pos=0; char s[10];
        for(int i=0;i<n;++i) {
            scanf("%s",s); leftcol|=1U<<(i*n); rightcol|=1U<<(i*n+n-1);
            for(int j=0;j<n;++j) {
                int p=i*n+j; if(s[j]=='#') mask|=1U<<p; if(s[j]=='@') pos=p;
                adj[p].clear();
                for(int di=-1;di<=1;++di) for(int dj=-1;dj<=1;++dj)
                    if((di||dj)&&i+di>=0&&i+di<n&&j+dj>=0&&j+dj<n)
                        adj[p].push_back((i+di)*n+j+dj);
            }
        }
        printf("%d\n",solve(mask,pos));
    }
    return 0;
}
