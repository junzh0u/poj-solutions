// POJ 3942 - Stopped Watches
// Model: gpt-6.1-sol
// Canonicalize the unordered hand positions under all rotations. Enumerate
// every second of the twelve-hour period, then find a minimum covering window.
// The statement guarantees that the desired interval does not cross midnight.
#include <cstdio>
#include <algorithm>
using namespace std;
const int PERIOD = 43200;
int signature(int a, int b, int c) {
    int p[3] = {a,b,c}, best = 216000;
    for (int i=0;i<3;++i) {
        int q[3];
        for (int j=0;j<3;++j) q[j]=(p[j]-p[i]+60)%60;
        sort(q,q+3);
        best=min(best,(q[0]*60+q[1])*60+q[2]);
    }
    return best;
}
int sig[PERIOD], masks[PERIOD];
void output(int t) { printf("%02d:%02d:%02d",t/3600,t/60%60,t%60); }
int main() {
    for (int t=0;t<PERIOD;++t) sig[t]=signature(t/720,t/60%60,t%60);
    int n;
    while (scanf("%d",&n)==1 && n) {
        int keys[10];
        for (int i=0;i<n;++i) {
            int a,b,c; scanf("%d%d%d",&a,&b,&c);
            keys[i]=signature(a,b,c);
        }
        for (int t=0;t<PERIOD;++t) {
            masks[t]=0;
            for (int i=0;i<n;++i) if(sig[t]==keys[i]) masks[t]|=1<<i;
        }
        int count[10]={0}, covered=0,left=0,best=PERIOD,begin=0,end=0;
        for (int right=0;right<PERIOD;++right) {
            for(int i=0;i<n;++i) if(masks[right]&(1<<i)) {
                if(count[i]++==0) ++covered;
            }
            while(covered==n) {
                if(right-left<best) { best=right-left;begin=left;end=right; }
                for(int i=0;i<n;++i) if(masks[left]&(1<<i)) {
                    if(--count[i]==0) --covered;
                }
                ++left;
            }
        }
        output(begin);printf(" ");output(end);printf("\n");
    }
    return 0;
}
