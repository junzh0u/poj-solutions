// POJ 3586 - The missionaries and cannibals
// Model: gpt-6.1-sol
// Safe banks have zero missionaries, all missionaries, or equal populations.
// BFS explores interval neighbors on these three lines; successor disjoint
// sets remove visited states, making the search nearly linear in m.
// Both banks must remain safe; every resulting legal boat is safe as well.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct State { int t,k,s,d; State(int T,int K,int S,int D):t(T),k(K),s(S),d(D){} };
vector<int> nxt[2][3];
vector<State> q;
int m,n;
int findnext(vector<int>& p,int x) { int r=x; while(p[r]!=r) r=p[r]; while(p[x]!=x) { int y=p[x]; p[x]=r; x=y; } return r; }
void interval(int t,int l,int r,int side,int dist) {
    l=max(l,t==1?1:0); r=min(r,t==1?m-1:m);
    if(l>r) return;
    vector<int>& p=nxt[side][t];
    for(int k=findnext(p,l);k<=r;k=findnext(p,k)) {
        q.push_back(State(t,k,side,dist)); p[k]=findnext(p,k+1);
    }
}
void dest(int t,int l,int r,int side,int dist) {
    if(side==0) interval(t,l,r,1,dist);
    else interval(2-t,m-r,m-l,0,dist);
}
int solve() {
    for(int s=0;s<2;s++) for(int t=0;t<3;t++) {
        nxt[s][t].resize(m+2); for(int k=0;k<=m+1;k++) nxt[s][t][k]=k;
    }
    q.clear(); interval(2,m,m,0,0);
    for(unsigned int i=0;i<q.size();i++) {
        State z=q[i]; int a=z.t==0?0:(z.t==2?m:z.k), b=z.k;
        if(a==0 && b==0 && z.s==1) return z.d;
        if(z.s) { a=m-a; b=m-b; }
        if(a==m) dest(2,max(0,b-n),b-1,z.s,z.d+1);
        if(a<=n) dest(0,max(0,a+b-n),b-(a==0),z.s,z.d+1);
        int low=max(1,(a+b-n+1)/2);
        int high=min(m-1,min(min(a,b),(a+b-1)/2));
        dest(1,low,high,z.s,z.d+1);
    }
    return -1;
}
int main() { int T; scanf("%d",&T); while(T--) { scanf("%d%d",&m,&n); printf("%d\n",solve()); } }
