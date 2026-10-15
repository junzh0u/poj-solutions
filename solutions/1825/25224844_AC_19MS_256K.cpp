// POJ 1825 - Young
// Model: gpt-6.1-sol
// Use the hook-length formula N! / product(hook lengths). Cancel prime
// exponents before multiplying a base-10000 integer, avoiding division and
// overflow. The example fixes the column orientation despite the prose's
// bottom-to-top wording; reflecting the drawing does not change the count.
#include <cstdio>
#include <vector>
using namespace std;
int power[241];
void factor(int x, int sign) {
    for (int p=2; p<=x; ++p)
        while (x%p==0) { power[p]+=sign; x/=p; }
}
int main() {
    int m;
    if (scanf("%d", &m)!=1) return 0;
    int a[20], n=0;
    for (int i=0;i<m;++i) { scanf("%d", &a[i]); n+=a[i]; }
    for (int x=2;x<=n;++x) factor(x,1);
    for (int i=0;i<m;++i) for (int j=0;j<a[i];++j) {
        int h=a[i]-j;
        for (int k=i+1;k<m;++k) if (a[k]>j) ++h;
        factor(h,-1);
    }
    vector<int> b(1,1);
    for (int p=2;p<=n;++p) for (int e=0;e<power[p];++e) {
        int carry=0;
        for (int k=0;k<(int)b.size();++k) {
            int t=b[k]*p+carry; b[k]=t%10000; carry=t/10000;
        }
        while (carry) { b.push_back(carry%10000); carry/=10000; }
    }
    printf("%d", b.back());
    for (int i=(int)b.size()-2;i>=0;--i) printf("%04d",b[i]);
    puts("");
    return 0;
}
