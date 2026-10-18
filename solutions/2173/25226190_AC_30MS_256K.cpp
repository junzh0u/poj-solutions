// POJ 2173 - Cricket Field
// Model: gpt-6-astra
// Control: C++98 translation of Roman Elizarov's published NEERC 2002 jury
// solution, https://neerc.ifmo.ru/archive/2002/solutions.zip (c_public.pas).
// Try every southwest corner from zero or a tree coordinate. For each corner,
// grow a square until a park edge or a strictly northeast tree stops it.
// Trees on the boundary are allowed. O(N^3) time and O(N) space.
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
    int n,w,h,x[101],y[101];
    if(scanf("%d%d%d",&n,&w,&h)!=3) return 0;
    x[0]=y[0]=0;
    for(int i=1;i<=n;++i) scanf("%d%d",&x[i],&y[i]);
    int bl=0,bp=0,bq=0;
    for(int i=0;i<=n;++i) for(int j=0;j<=n;++j) {
        int p=x[i],q=y[j],l=min(w-p,h-q);
        for(int k=1;k<=n;++k)
            if(x[k]>p && y[k]>q) l=min(l,max(x[k]-p,y[k]-q));
        if(l>bl) {bl=l;bp=p;bq=q;}
    }
    printf("%d %d %d\n",bp,bq,bl);
    return 0;
}
