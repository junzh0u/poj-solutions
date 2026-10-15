// POJ 1181 - Bus Terminals
// Model: gpt-6.1-sol
// Enumerate ordered hubs and sweep a prefix of stops sorted by distance to
// the first hub. In an optimum one side has radius at most half the diameter;
// moving all nearer stops to that side cannot increase any route. Thus some
// ordered prefix partition is optimal. Suffix maxima evaluate each cut in O(1).
// Distances are Manhattan street distances; hubs themselves have radius zero.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;
int n, x[500], y[500], d[500][500], ord[500][500];
int root;
bool cmp(int a,int b) { return d[root][a]<d[root][b]; }
int main() {
    if(scanf("%d",&n)!=1) return 0;
    for(int i=0;i<n;++i) scanf("%d%d",&x[i],&y[i]);
    for(int i=0;i<n;++i) for(int j=0;j<n;++j)
        d[i][j]=abs(x[i]-x[j])+abs(y[i]-y[j]);
    for(int i=0;i<n;++i) {
        for(int j=0;j<n;++j) ord[i][j]=j;
        root=i; sort(ord[i],ord[i]+n,cmp);
    }
    int ans=1000000000;
    for(int a=0;a<n;++a) for(int b=0;b<n;++b) if(a!=b) {
        int v[500], hi[501], lo[501], m=0;
        for(int i=0;i<n;++i) if(ord[a][i]!=a && ord[a][i]!=b)
            v[m++]=ord[a][i];
        hi[m]=lo[m]=0;
        for(int k=m-1;k>=0;--k) {
            int z=d[b][v[k]];
            hi[k]=hi[k+1]; lo[k]=lo[k+1];
            if(z>=hi[k]) { lo[k]=hi[k]; hi[k]=z; }
            else if(z>lo[k]) lo[k]=z;
        }
        int p=0,q=0;
        for(int k=0;k<=m;++k) {
            int val=max(max(p+q,hi[k]+lo[k]),p+d[a][b]+hi[k]);
            if(val<ans) ans=val;
            if(k<m) { q=p; p=d[a][v[k]]; }
        }
    }
    printf("%d\n",ans);
    return 0;
}
