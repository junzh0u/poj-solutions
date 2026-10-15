// POJ 2746 - Bingo
// Model: gpt-6.1-sol
// Enumerate a winning line per card and sort their unions by size.
// Test each union by subset dynamic programming, deleting the last announcement.
// Every intermediate set must have a prefix of winners; simultaneous wins count.
// An optimum uses only numbers from one completed line per card: removing all
// other announcements cannot delay those wins or introduce an earlier later win.
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
struct Mask {
    unsigned int w[4];
    Mask() { for(int i=0;i<4;++i) w[i]=0; }
    void add(int x) { w[x/32] |= 1u<<(x%32); }
    bool has(int x) const { return (w[x/32]>>(x%32))&1u; }
    bool contains(const Mask& b) const {
        for(int i=0;i<4;++i) if((w[i]&b.w[i])!=b.w[i]) return false;
        return true;
    }
    bool operator<(const Mask& b) const {
        for(int i=0;i<4;++i) if(w[i]!=b.w[i]) return w[i]<b.w[i];
        return false;
    }
};
int p,m;
vector<int> line[4][10];
Mask lm[4][10];

vector<Mask> candidates;
void generate(int k,Mask s) {
    if(k==p) { candidates.push_back(s); return; }
    for(int j=0;j<2*m+2;++j) {
        Mask t=s; for(int z=0;z<4;++z) t.w[z]|=lm[k][j].w[z];
        generate(k+1,t);
    }
}
int sizeOf(const Mask& s) {
    int n=0; for(int x=0;x<100;++x) n+=s.has(x); return n;
}
bool smaller(const Mask& a,const Mask& b) {
    int x=sizeOf(a),y=sizeOf(b); return x!=y?x<y:a<b;
}
vector<int> smallLine[4];
signed char memo[65536];
bool feasible(int s) {
    if(s==0) return true;
    if(memo[s]) return memo[s]>0;
    bool gap=false;
    for(int k=0;k<p;++k) {
        bool win=false;
        for(int j=0;j<(int)smallLine[k].size();++j)
            if((s&smallLine[k][j])==smallLine[k][j]) { win=true; break; }
        if(win && gap) { memo[s]=-1; return false; }
        if(!win) gap=true;
    }
    memo[s]=-1;
    for(int b=s;b;b&=b-1) if(feasible(s^(b&-b))) { memo[s]=1; return true; }
    return false;
}
int solve() {
    candidates.clear(); generate(0,Mask());
    sort(candidates.begin(),candidates.end(),smaller);
    for(int q=0;q<(int)candidates.size();++q) {
        if(q && !(candidates[q-1]<candidates[q]) && !(candidates[q]<candidates[q-1])) continue;
        Mask s=candidates[q]; int id[100],n=0;
        for(int x=0;x<100;++x) if(s.has(x)) id[x]=n++;
        for(int k=0;k<p;++k) {
            smallLine[k].clear();
            for(int j=0;j<2*m+2;++j) if(s.contains(lm[k][j])) {
                int t=0; for(int z=0;z<m;++z) t|=1<<id[line[k][j][z]];
                smallLine[k].push_back(t);
            }
        }
        fill(memo,memo+(1<<n),0);
        if(feasible((1<<n)-1)) return n;
    }
    return 0;
}
int main() {
    while(scanf("%d%d",&p,&m)==2 && p) {
        for(int k=0;k<p;++k) {
            int a[4][4];
            for(int i=0;i<m;++i) for(int j=0;j<m;++j) scanf("%d",&a[i][j]);
            for(int j=0;j<2*m+2;++j) { line[k][j].clear(); lm[k][j]=Mask(); }
            for(int i=0;i<m;++i) for(int j=0;j<m;++j) {
                line[k][i].push_back(a[i][j]); line[k][m+j].push_back(a[i][j]);
                if(i==j) line[k][2*m].push_back(a[i][j]);
                if(i+j==m-1) line[k][2*m+1].push_back(a[i][j]);
            }
            for(int j=0;j<2*m+2;++j) for(int i=0;i<m;++i) lm[k][j].add(line[k][j][i]);
        }
        printf("%d\n",solve());
    }
    return 0;
}
