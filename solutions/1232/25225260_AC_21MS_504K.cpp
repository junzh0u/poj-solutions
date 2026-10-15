// POJ 1232 - Microfiches
// Model: gpt-6.1-sol
// Enumerate distinguishing bit subsets using subset sums of XOR differences.
// A target-one bit costs one removal. For z target-zero bits, moving the
// target costs z operations and recycling auxiliary stacks costs max(0,z-2).
// Perform zero-bit filters first, then all one-bit removals; order is irrelevant.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int cnt[1<<15], bits[1<<15];
int main() {
    for (int i=1;i<(1<<15);++i) bits[i]=bits[i>>1]+(i&1);
    int tests; scanf("%d",&tests);
    while (tests--) {
        int n,g,target=0; scanf("%d%d",&n,&g);
        int size=1<<g; memset(cnt,0,size*sizeof(int));
        char s[32];
        for (int i=0;i<n;++i) {
            scanf("%s",s); int x=0;
            for (int j=0;j<g;++j) x=x*2+s[j]-'0';
            if (!i) target=x; else ++cnt[x^target];
        }
        for (int b=0;b<g;++b)
            for (int m=0;m<size;++m)
                if (m&(1<<b)) cnt[m]+=cnt[m^(1<<b)];
        int answer=100;
        for (int m=0;m<size;++m) if (!cnt[(size-1)^m]) {
            int z=bits[m&((size-1)^target)], o=bits[m&target];
            answer=min(answer,o+z+max(0,z-2));
        }
        printf("%d\n",answer);
    }
    return 0;
}
