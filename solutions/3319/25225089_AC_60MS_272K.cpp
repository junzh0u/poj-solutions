// POJ 3319 - Dragon Slayer Qualification Exam
// Model: gpt-6.1-sol
// Enumerate the colors of mandatory red rooks, rejecting row/column conflicts.
// For each color, remaining ordinary cells form a row-column matching graph.
// The two colors are independent after red assignments; all red cells remain
// unavailable to optional rooks. Parity bounds prune unpromising assignments.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int n,m,board[40][40],rr[10],cc[10],adj[2][40][40],deg[2][40];
bool usedR[2][40],usedC[2][40],seen[40];
int mate[40],best,upper;
int color;
bool augment(int r) {
    for(int i=0;i<deg[color][r];++i) {
        int c=adj[color][r][i];
        if(usedC[color][c] || seen[c]) continue;
        seen[c]=true;
        if(mate[c]<0 || augment(mate[c])) { mate[c]=r; return true; }
    }
    return false;
}
int matching(int co) {
    color=co; memset(mate,-1,sizeof mate);
    int ans=0; bool matched[40]={false};
    for(int r=0;r<n;++r) if(!usedR[co][r])
        for(int j=0;j<deg[co][r];++j) {
            int c=adj[co][r][j];
            if(!usedC[co][c] && mate[c]<0) {
                mate[c]=r; matched[r]=true; ++ans; break;
            }
        }
    for(int r=0;r<n;++r) if(!usedR[co][r] && !matched[r]) {
        memset(seen,0,sizeof seen);
        if(augment(r)) ++ans;
    }
    return ans;
}
int bound() {
    int res=m;
    for(int co=0;co<2;++co) for(int p=0;p<2;++p) {
        int rows=0,cols=0;
        for(int i=0;i<n;++i) {
            if((i&1)==p && !usedR[co][i]) ++rows;
            if((i&1)==(p^co) && !usedC[co][i]) ++cols;
        }
        res+=min(rows,cols);
    }
    return res;
}
void search(int at) {
    if(best==upper) return;
    if(at==m) {
        if(bound()<=best) return;
        int a=matching(0);
        if(m+a+n<=best) return;
        best=max(best,m+a+matching(1));
        return;
    }
    int r=rr[at],c=cc[at];
    for(int co=0;co<2;++co) if(!usedR[co][r] && !usedC[co][c]) {
        usedR[co][r]=usedC[co][c]=true;
        search(at+1);
        usedR[co][r]=usedC[co][c]=false;
    }
}
int main() {
    int tests,k; scanf("%d",&tests);
    while(tests--) {
        scanf("%d%d%d",&n,&m,&k);
        memset(board,0,sizeof board); memset(deg,0,sizeof deg);
        memset(usedR,0,sizeof usedR); memset(usedC,0,sizeof usedC);
        for(int i=0;i<m;++i) { scanf("%d%d",rr+i,cc+i); board[rr[i]][cc[i]]=1; }
        for(int i=0;i<k;++i) { int r,c; scanf("%d%d",&r,&c); board[r][c]=2; }
        for(int r=0;r<n;++r) for(int c=0;c<n;++c) if(!board[r][c]) {
            int co=(r+c)&1; adj[co][r][deg[co][r]++]=c;
        }
        // Independent matchings allowing either color at every red cell bound
        // any feasible solution, even though they may double-use red cells.
        int saved[2][40]; memcpy(saved,deg,sizeof deg);
        for(int i=0;i<m;++i) for(int co=0;co<2;++co)
            adj[co][rr[i]][deg[co][rr[i]]++]=cc[i];
        upper=matching(0)+matching(1);
        memcpy(deg,saved,sizeof deg);
        best=0; search(0); printf("%d\n",best);
    }
    return 0;
}
