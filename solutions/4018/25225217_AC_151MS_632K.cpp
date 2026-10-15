// POJ 4018 - High security
// Model: gpt-6.1-sol
// Count equal projections for every subset of the five positions, then
// invert the binomial sums to recover pairs with each exact Hamming distance.
// The POJ statement is empty; the original 2011 Northern contest PDF is used.
#include <cstdio>
#include <algorithm>
using namespace std;
unsigned int passwords[50000], keys[50000];
long long sums[6], exact[6];
int choose[6][6];
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i=0;i<n;++i) {
        char s[16]; scanf("%s",s);
        unsigned int v=0;
        for (int j=0;j<5;++j) {
            int c=s[j], x;
            if (c>='a' && c<='z') x=c-'a'+1;
            else if (c>='A' && c<='Z') x=c-'A'+27;
            else x=c-'0'+53;
            v |= (unsigned int)x << (6*j);
        }
        passwords[i]=v;
    }
    for (int mask=0;mask<32;++mask) {
        unsigned int bits=0; int k=0;
        for (int j=0;j<5;++j) if (mask & (1<<j)) {
            bits |= 63U << (6*j); ++k;
        }
        for (int i=0;i<n;++i) keys[i]=passwords[i]&bits;
        sort(keys,keys+n);
        for (int i=0;i<n;) {
            int j=i+1;
            while (j<n && keys[i]==keys[j]) ++j;
            sums[k]+=(long long)(j-i)*(j-i-1)/2;
            i=j;
        }
    }
    for (int i=0;i<=5;++i) {
        choose[i][0]=choose[i][i]=1;
        for (int j=1;j<i;++j) choose[i][j]=choose[i-1][j-1]+choose[i-1][j];
    }
    for (int i=5;i>=0;--i) {
        exact[i]=sums[i];
        for (int j=i+1;j<=5;++j) exact[i]-=exact[j]*choose[j][i];
    }
    for (int i=5;i>=0;--i) printf("%lld%c",exact[i],i==0?'\n':' ');
    return 0;
}
