// POJ 3350 - Bowling for Numbers++
// Model: gpt-6.1-sol
// DP by balls and rightmost covered position, with a monotone queue for
// overlapping throws. Pad both ends with empty space; an internal component
// must begin with a full-width throw, but later throws may overlap it.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
const int NEG = -1000000000;
int main() {
    int t; scanf("%d", &t);
    while (t--) {
        int n,k,w; scanf("%d%d%d", &n,&k,&w);
        int m=n+2*(w-1);
        vector<int> s(m+1,0);
        for(int i=1;i<=n;++i) scanf("%d", &s[i+w-1]);
        for(int i=1;i<=m;++i) s[i]+=s[i-1];
        vector<int> p(m+1,0),c(m+1,0), e(m+1,NEG),f(m+1,NEG),q(m+1);
        for(int b=1;b<=k;++b) {
            int h=0,tail=0; c[0]=0; f[0]=NEG;
            for(int i=1;i<=m;++i) {
                int j=i-1;
                if(e[j]!=NEG) {
                    while(h<tail && e[q[tail-1]]-s[q[tail-1]]<=e[j]-s[j]) --tail;
                    q[tail++]=j;
                }
                while(h<tail && q[h]<i-w) ++h;
                int v=NEG;
                if(i>=w) v=p[i-w]-s[i-w];
                if(h<tail) v=max(v,e[q[h]]-s[q[h]]);
                f[i]=(v==NEG ? NEG : v+s[i]);
                c[i]=max(c[i-1],f[i]);
            }
            p.swap(c); e.swap(f);
        }
        printf("%d\n",p[m]);
    }
    return 0;
}
