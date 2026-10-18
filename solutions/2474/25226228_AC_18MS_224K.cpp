// POJ 2474 - European railroad tracks
// Model: gpt-6-astra
// Iterative deepening over connected sets of rails. In a minimum solution,
// useful gauge edges connect all rails: otherwise translate two components
// until one vertex coincides, preserving every gauge with fewer rails.
// Anchor an edge of maximum required length at 0 and D, then enumerate all
// ways to attach a rail by a required length, in either direction. The
// anchor need not be the leftmost rail; normalize only the final answer.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;
int n, d[8], p[5], limit, all;
bool dfs(int k, int mask) {
    if (mask == all) return true;
    if (k == limit) return false;
    int missing = 0, first = 0;
    for (int i=0;i<n;++i) if (!(mask & (1<<i))) { ++missing; first=i; }
    if (missing > (limit*(limit-1)-k*(k-1))/2) return false;
    int candidates[80], count=0;
    for (int i=0;i<k;++i)
        for (int j=0;j<n;++j) {
            if (k+1 == limit && j != first) continue;
            candidates[count++]=p[i]+d[j];
            candidates[count++]=p[i]-d[j];
        }
    sort(candidates,candidates+count);
    count=unique(candidates,candidates+count)-candidates;
    for (int c=0;c<count;++c) {
        int x=candidates[c], next=mask;
        bool duplicate=false;
        for (int i=0;i<k;++i) {
            if (p[i]==x) duplicate=true;
            int delta=abs(p[i]-x);
            for (int j=0;j<n;++j) if (delta==d[j]) next |= 1<<j;
        }
        if (duplicate) continue;
        p[k]=x;
        if (dfs(k+1,next)) return true;
    }
    return false;
}
int main() {
    int tests; if (scanf("%d", &tests)!=1) return 0;
    for (int tc=1;tc<=tests;++tc) {
        scanf("%d",&n);
        for (int i=0;i<n;++i) scanf("%d",&d[i]);
        sort(d,d+n); n=unique(d,d+n)-d; all=(1<<n)-1;
        for (limit=2;limit<=5;++limit) {
            p[0]=0; p[1]=d[n-1];
            if (dfs(2,1<<(n-1))) break;
        }
        sort(p,p+limit);
        int origin=p[0];
        printf("Scenario #%d\n%d:",tc,limit);
        for (int i=0;i<limit;++i) printf(" %d",p[i]-origin);
        printf("\n\n");
    }
    return 0;
}
