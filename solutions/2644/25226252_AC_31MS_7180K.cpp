// POJ 2644 - Maze
// Model: gpt-6-astra
// IDA* on the set of positions that have not escaped. Exact two-position
// escape distances form an admissible lower bound; a bounded transposition
// cache prunes repeated sets. Each command takes one step, not a whole slide.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned long long Mask;
int n, cnt, pos[8][8], go[37][4], dista[37][37];
char grid[8][9];
Mask stay[4], moving[4];
int dr[4]={0,-1,1,0}, dc[4]={1,0,0,-1};
const char *names[4]={"east","north","south","west"};
const int HS=1<<20;
Mask keys[HS];
int budget[HS], stamp[HS], iteration;
vector<int> path;
Mask advance(Mask s,int d) {
    Mask t=s&moving[d];
    if(d==0) t<<=1;
    if(d==1) t>>=6;
    if(d==2) t<<=6;
    if(d==3) t>>=1;
    return t|(s&stay[d]);
}
int lower(Mask s) {
    int a[36], k=0, ans=0;
    for(int r=1;r<n-1;r++) for(int c=1;c<n-1;c++)
        if(s&(Mask(1)<<((r-1)*6+c-1))) a[k++]=pos[r][c];
    for(int i=0;i<k;i++) for(int j=0;j<=i;j++)
        ans=max(ans,dista[a[i]][a[j]]);
    return ans;
}
bool search(Mask s,int left) {
    if(!s) return true;
    if(lower(s)>left) return false;
    unsigned h=(unsigned)((s^(s>>23))*2654435761U)&(HS-1);
    if(stamp[h]==iteration && keys[h]==s && budget[h]>=left) return false;
    keys[h]=s; budget[h]=left; stamp[h]=iteration;
    Mask next[4]; int order[4], value[4], k=0;
    for(int d=0;d<4;d++) {
        Mask t=advance(s,d);
        if(t==s) continue;
        int hval=lower(t);
        if(hval>=left) continue;
        int j=k++;
        while(j && value[j-1]>hval) {
            value[j]=value[j-1]; next[j]=next[j-1]; order[j]=order[j-1]; --j;
        }
        value[j]=hval; next[j]=t; order[j]=d;
    }
    for(int i=0;i<k;i++) {
        path.push_back(order[i]);
        if(search(next[i],left-1)) return true;
        path.pop_back();
    }
    return false;
}
int main() {
    if(scanf("%d",&n)!=1) return 0;
    for(int r=0;r<n;r++) scanf("%s",grid[r]);
    Mask initial=0;
    for(int r=1;r<n-1;r++) for(int c=1;c<n-1;c++) if(grid[r][c]=='.') {
        pos[r][c]=++cnt; initial|=Mask(1)<<((r-1)*6+c-1);
    }
    for(int r=1;r<n-1;r++) for(int c=1;c<n-1;c++) if(pos[r][c]) {
        Mask b=Mask(1)<<((r-1)*6+c-1);
        for(int d=0;d<4;d++) {
            int rr=r+dr[d], cc=c+dc[d];
            if(grid[rr][cc]=='O') { go[pos[r][c]][d]=pos[r][c]; stay[d]|=b; }
            else { go[pos[r][c]][d]=pos[rr][cc]; if(pos[rr][cc]) moving[d]|=b; }
        }
    }
    int m=cnt+1;
    vector<vector<int> > rev(m*m);
    for(int a=0;a<m;a++) for(int b=0;b<m;b++) for(int d=0;d<4;d++)
        rev[go[a][d]*m+go[b][d]].push_back(a*m+b);
    vector<int> q(1,0), ds(m*m,-1); ds[0]=0;
    for(unsigned h=0;h<q.size();h++) for(unsigned j=0;j<rev[q[h]].size();j++) {
        int v=rev[q[h]][j]; if(ds[v]<0) {ds[v]=ds[q[h]]+1; q.push_back(v);}
    }
    for(int a=0;a<m;a++) for(int b=0;b<m;b++) dista[a][b]=ds[a*m+b];
    for(int limit=lower(initial);;limit++) { ++iteration; if(search(initial,limit)) break; }
    for(unsigned i=0;i<path.size();i++) puts(names[path[i]]);
}
