// POJ 3387 - Ideal Frame
// Model: gpt-6.1-sol
// Split every bead of degree greater than two. Euler trail decomposition
// leaves odd-degree/2 paths per component; an even component must be opened
// to join others, costing an extra split only when it is a simple cycle.
// Each zero endpoint denotes a different free end, not a shared bead.
#include <cstdio>
#include <algorithm>
using namespace std;
const int N=101005;
int p[N], d[N], odd[N], branch[N];
int root(int x) { return p[x]==x?x:p[x]=root(p[x]); }
int main() {
    int n,m;
    if(scanf("%d%d",&n,&m)!=2) return 0;
    int cnt=n;
    for(int i=1;i<=n+2*m;++i) p[i]=i;
    for(int i=0;i<m;++i) {
        int a,b; scanf("%d%d",&a,&b);
        if(!a) a=++cnt;
        if(!b) b=++cnt;
        ++d[a]; ++d[b];
        int x=root(a),y=root(b); if(x!=y) p[x]=y;
    }
    int splits=0;
    for(int i=1;i<=cnt;++i) if(d[i]) {
        int r=root(i);
        odd[r]+=d[i]%2;
        if(d[i]>2) { ++splits; ++branch[r]; }
    }
    int components=0, paths=0, extra=0, totalodd=0;
    for(int i=1;i<=cnt;++i) if(d[i] && root(i)==i) {
        ++components; totalodd+=odd[i];
        paths+=max(1,odd[i]/2);
        if(!odd[i] && !branch[i]) ++extra;
    }
    int ans=splits+paths+extra;
    if(components==1 && totalodd==0) ans=splits;
    printf("%d\n",ans);
    return 0;
}
