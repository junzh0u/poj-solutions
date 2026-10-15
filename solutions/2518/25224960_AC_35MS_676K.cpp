// POJ 2518 - Little guys' game
// Model: gpt-6.1-sol
// Track only the four positions of the chosen target character.
// A multi-source BFS from the nine winning squares precomputes shortest
// distances and counts of shortest swap sequences for all 1820 masks.
// Non-target swaps cannot occur in a shortest winning strategy; target
// choices are distinct, so counts for all equally good characters are added.
#include <cstdio>
#include <cstring>
int dista[65536], ways[65536], que[1820];
int ea[24], eb[24], ne;
int main() {
    int r,c;
    for(r=0;r<4;++r) for(c=0;c<4;++c) {
        int p=r*4+c;
        if(c<3) { ea[ne]=p; eb[ne++]=p+1; }
        if(r<3) { ea[ne]=p; eb[ne++]=p+4; }
    }
    memset(dista,-1,sizeof(dista));
    int head=0,tail=0;
    for(r=0;r<3;++r) for(c=0;c<3;++c) {
        int p=r*4+c, m=51<<p;
        dista[m]=0; ways[m]=1; que[tail++]=m;
    }
    while(head<tail) {
        int m=que[head++];
        for(int e=0;e<ne;++e) {
            int a=1<<ea[e], b=1<<eb[e];
            if(((m&a)!=0)==((m&b)!=0)) continue;
            int n=m^a^b;
            if(dista[n]<0) {
                dista[n]=dista[m]+1; que[tail++]=n;
            }
            if(dista[n]==dista[m]+1) ways[n]+=ways[m];
        }
    }
    char ch;
    while(std::scanf(" %c",&ch)==1) {
        int mask[4]={0,0,0,0};
        mask[ch-'A']|=1;
        for(int i=1;i<16;++i) {
            if(std::scanf(" %c",&ch)!=1) return 0;
            mask[ch-'A']|=1<<i;
        }
        int best=100, count=0;
        for(int k=0;k<4;++k) {
            int d=dista[mask[k]];
            if(d<best) { best=d; count=0; }
            if(d==best) count+=ways[mask[k]];
        }
        std::printf("%d %d\n",best,count);
    }
    return 0;
}
