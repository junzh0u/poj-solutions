// POJ 2844 - Sum and Product
// Model: gpt-6-astra
// Enumerate non-unit factors in increasing absolute value, negative first
// among equal magnitudes. The remaining +1/-1 values are solved directly
// from their sum and the parity of their negative count. Zero is separate.
// The two samples are independent inputs; each run reads one triple.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;
vector<int> divisors, chosen;
int plusOnes, minusOnes;
bool units(int n, int s, int neg) {
    if (s < -n || s > n || (n-s)%2) return false;
    int m=(n-s)/2;
    if (m%2 != neg) return false;
    minusOnes=m; plusOnes=n-m; return true;
}
bool search(int q, int n, int s, int neg, int low, bool positive) {
    if (q==1) return units(n,s,neg);
    if (n==0 || s > q+n-1 || s < -q-n+1) return false;
    // An odd product forces every remaining integer to be odd.
    if (q%2 && ((s-n)%2)) return false;
    for (int sign=1; sign>=-1; sign-=2) {
        if (q<low || (q==low && positive && sign<0)) continue;
        if (units(n-1,s-sign*q,neg^(sign<0))) {
            chosen.push_back(sign*q); return true;
        }
    }
    if (n==1) return false;
    for (size_t i=0;i<divisors.size();++i) {
        int d=divisors[i];
        if (d<low) continue;
        if (d>q/d) break;
        if (q%d) continue;
        for (int sign=-1; sign<=1; sign+=2) {
            if (d==low && positive && sign<0) continue;
            chosen.push_back(sign*d);
            if (search(q/d,n-1,s-sign*d,neg^(sign<0),d,sign>0)) return true;
            chosen.pop_back();
        }
    }
    return false;
}
int main() {
    int n,s,p;
    if (scanf("%d%d%d",&n,&s,&p)!=3) return 0;
    bool ok=false;
    plusOnes=minusOnes=0;
    if (p==0) {
        if (n>=2 || s==0) {
            chosen.push_back(s);
            while ((int)chosen.size()<n) chosen.push_back(0);
            ok=true;
        }
    } else {
        int q=abs(p);
        for (int d=2;d<=q/d;++d) if (q%d==0) divisors.push_back(d);
        ok=search(q,n,s,p<0,2,false);
    }
    if (!ok) { puts("No solution"); return 0; }
    bool first=true;
    for (int i=0;i<plusOnes+minusOnes;++i) {
        if (!first) putchar(' '); first=false;
        printf("%d",i<plusOnes?1:-1);
    }
    for (size_t i=0;i<chosen.size();++i) {
        if (!first) putchar(' '); first=false; printf("%d",chosen[i]);
    }
    putchar('\n'); return 0;
}
