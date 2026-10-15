// POJ 1772 - New Go Game
// Model: gpt-6.1-sol
// Reconstruct the unique binary board using line-sum propagation and search.
// Diagonal groups are indexed by row+column and column-row+N-1.
// The input promises uniqueness; stop at the first complete reconstruction.
// Empty cells unreachable from the boundary are the enclosed intersections.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
int n, groups, ids[225][4], target[90];
vector<int> cells[90];
struct State { signed char a[225]; int need[90], left[90]; };
bool put(State &s,int p,int v) {
    if(s.a[p]>=0) return s.a[p]==v;
    s.a[p]=(signed char)v;
    for(int k=0;k<4;k++) {int g=ids[p][k]; s.need[g]-=v; --s.left[g];
        if(s.need[g]<0 || s.need[g]>s.left[g]) return false;}
    return true;
}
bool solve(State &s) {
    bool changed=true;
    while(changed) {
        changed=false;
        for(int g=0;g<groups;g++) {
            if(s.need[g]<0 || s.need[g]>s.left[g]) return false;
            if(s.left[g] && (s.need[g]==0 || s.need[g]==s.left[g])) {
                int v=s.need[g]!=0; changed=true;
                for(unsigned j=0;j<cells[g].size();j++)
                    if(s.a[cells[g][j]]<0 && !put(s,cells[g][j],v)) return false;
            }
        }
    }
    int best=-1; double cost=1e100;
    for(int g=0;g<groups;g++) if(s.left[g]) {
        int k=min(s.need[g],s.left[g]-s.need[g]); double c=1;
        for(int i=1;i<=k;i++) c=c*(s.left[g]-i+1)/i;
        if(c<cost) {cost=c;best=g;}
    }
    if(best<0) return true;
    int p=-1; double score=-1;
    for(unsigned j=0;j<cells[best].size();j++) {
        int q=cells[best][j]; if(s.a[q]>=0) continue;
        double x=0;
        for(int k=0;k<4;k++) x+=1.0/s.left[ids[q][k]];
        if(x>score) {score=x;p=q;}
    }
    int first=s.need[best]*2>s.left[best];
    for(int k=0;k<2;k++) {State t=s; if(put(t,p,first^k) && solve(t)) {s=t;return true;}}
    return false;
}
int main() {
    while(scanf("%d",&n)==1 && n) {
        groups=6*n-2;
        for(int g=0;g<groups;g++) {cells[g].clear();scanf("%d",&target[g]);}
        for(int r=0;r<n;r++) for(int c=0;c<n;c++) {
            int p=r*n+c; ids[p][0]=r;ids[p][1]=n+c;
            ids[p][2]=2*n+r+c;ids[p][3]=4*n-1+c-r+n-1;
            for(int k=0;k<4;k++) cells[ids[p][k]].push_back(p);
        }
        State s; memset(s.a,-1,sizeof(s.a));
        for(int g=0;g<groups;g++) {s.need[g]=target[g];s.left[g]=(int)cells[g].size();}
        solve(s);
        int q[225],head=0,tail=0; bool seen[225]={false};
        for(int r=0;r<n;r++) for(int c=0;c<n;c++) if(r==0||c==0||r==n-1||c==n-1) {
            int p=r*n+c;if(s.a[p]==0&&!seen[p]) {seen[p]=true;q[tail++]=p;}
        }
        const int dr[4]={-1,1,0,0},dc[4]={0,0,-1,1};
        while(head<tail) {int p=q[head++],r=p/n,c=p%n;
            for(int k=0;k<4;k++) {int rr=r+dr[k],cc=c+dc[k],v=rr*n+cc;
                if(rr>=0&&rr<n&&cc>=0&&cc<n&&!seen[v]&&s.a[v]==0) {seen[v]=true;q[tail++]=v;}}
        }
        int answer=0;for(int p=0;p<n*n;p++) if(s.a[p]==0&&!seen[p]) ++answer;
        printf("%d\n",answer);
    }
    return 0;
}
