// POJ 2861 - Cycle with 6 vertices
// Model: gpt-6.1-sol
// Enumerate each unordered alternating triple of a six-cycle. Inclusion-
// exclusion chooses three distinct common neighbors outside that triple.
// Each undirected cycle has exactly two alternating triples; divide by two
// modulo 9901. Diagonal entries never connect distinct selected cities.
#include <cstdio>
char g[112][113];
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i=0;i<n;++i) scanf("%s",g[i]);
    int ans=0;
    for (int x=0;x<n;++x) for (int y=x+1;y<n;++y)
        for (int z=y+1;z<n;++z) {
            int a=0,b=0,c=0,t=0;
            for (int v=0;v<n;++v) if(v!=x && v!=y && v!=z) {
                int p=g[x][v]=='1', q=g[y][v]=='1', r=g[z][v]=='1';
                a+=p&q; b+=q&r; c+=r&p; t+=p&q&r;
            }
            ans=(ans+a*b*c-t*(a+b+c)+2*t)%9901;
        }
    printf("%d\n",ans*4951%9901);
    return 0;
}
