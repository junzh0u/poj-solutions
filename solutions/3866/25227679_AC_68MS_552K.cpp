// POJ 3866 - Exclusive Access 2
// Model: gpt-6-astra
// Partition the resource graph into the minimum number of independent sets.
// Orient each process toward the higher color. Any acyclic orientation's
// longest-path levels are a coloring, so this minimizes the longest path.
// A chain of length m contains m+1 processes (edges), hence colors minus two.
#include <cstdio>
#include <algorithm>
using namespace std;
int adj[15], dp[1<<15], take[1<<15], col[15];
bool independent[1<<15];
int main() {
    int n, a[100], b[100], used=0;
    if (scanf("%d", &n)!=1) return 0;
    for(int i=0;i<n;++i) {
        char x,y; scanf(" %c %c", &x,&y);
        a[i]=x-'L'; b[i]=y-'L';
        adj[a[i]]|=1<<b[i]; adj[b[i]]|=1<<a[i];
        used|=(1<<a[i])|(1<<b[i]);
    }
    independent[0]=true;
    for(int s=1;s<(1<<15);++s) {
        int bit=s&-s, v=0;
        while((1<<v)!=bit) ++v;
        independent[s]=independent[s^bit] && !(adj[v]&(s^bit));
        dp[s]=16;
        if(independent[s]) { dp[s]=1; take[s]=s; continue; }
        for(int t=s;t;t=(t-1)&s)
            if((t&bit) && independent[t] && dp[s^t]+1<dp[s]) {
                dp[s]=dp[s^t]+1; take[s]=t;
            }
    }
    printf("%d\n", dp[used]-2);
    int c=0;
    for(int s=used;s;s^=take[s],++c)
        for(int v=0;v<15;++v) if(take[s]&(1<<v)) col[v]=c;
    for(int i=0;i<n;++i) {
        if(col[a[i]]>col[b[i]]) swap(a[i],b[i]);
        printf("%c %c\n", 'L'+a[i], 'L'+b[i]);
    }
    return 0;
}
