// POJ 3215 - Median of Lines
// Model: gpt-6.1-sol
// The median is zero exactly when at most n/2 lines are strictly
// positive and at most n/2 are strictly negative. Sort the roots of
// increasing lines; constant lines shift the two boundary ranks.
// Finite boundaries are included, infinities are open. Remove a minus
// sign only after formatting a value as -0.00, without perturbing roots.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
void number(double x) {
    char s[80];
    sprintf(s, "%.2f", x);
    printf("%s", strcmp(s,"-0.00")==0 ? "0.00" : s);
}
int main() {
    int n;
    while (scanf("%d", &n)==1 && n) {
        vector<double> roots;
        int pos=0, neg=0;
        for(int i=0;i<n;++i) {
            int a,b; scanf("%d%d",&a,&b);
            if(b) roots.push_back(-double(a)/b);
            else if(a>0) ++pos;
            else if(a<0) ++neg;
        }
        int k=n/2, m=roots.size();
        if(pos>k || neg>k) { puts("-1"); continue; }
        sort(roots.begin(),roots.end());
        int lo=m-k+neg-1, hi=k-pos;
        if(lo<0) printf("(-inf,");
        else { printf("["); number(roots[lo]); printf(","); }
        if(hi>=m) puts("+inf)");
        else { number(roots[hi]); puts("]"); }
    }
    return 0;
}
