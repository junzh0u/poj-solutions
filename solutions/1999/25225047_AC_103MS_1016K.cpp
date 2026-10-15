// POJ 1999 - Quorf
// Model: gpt-6.1-sol
// Shared cards must be removed simultaneously, so their relative order
// must agree in both packets. Nonshared cards can be removed between them.
// The turn wording is ambiguous; the common-order reading is checked
// against the original CTU Open 1999 judge data.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
int main() {
    int z; if (scanf("%d", &z) != 1) return 0;
    while (z--) {
        int n,m,x; scanf("%d%d",&n,&m);
        vector<pair<int,int> > a(n);
        for (int i=0;i<n;++i) { scanf("%d",&x); a[i]=make_pair(x,i); }
        sort(a.begin(),a.end());
        int last=-1; bool ok=true;
        for (int i=0;i<m;++i) {
            scanf("%d",&x);
            vector<pair<int,int> >::iterator p=lower_bound(a.begin(),a.end(),make_pair(x,-1));
            if (p!=a.end() && p->first==x) {
                if (p->second<last) ok=false;
                last=p->second;
            }
        }
        puts(ok?"Hrac ma sanci vyhrat.":"Spatne usporadani.");
    }
    return 0;
}
