// POJ 4037 - Distributing Ballot Boxes
// Model: gpt-6.1-sol
// Binary search the smallest capacity whose required box count fits B.
// A city of a people needs 1+(a-1)/capacity boxes. Stop counting once
// the budget is exceeded, keeping every integer operation within 32 bits.
// POJ's statement is blank; framing and bounds follow SWERC 2011 / UVa 12390.
// Buffered input addresses the POJ discussion's input-speed warning.
#include <cstdio>
static char buf[65536];
static int pos=0, len=0;
static int nextchar() {
    if(pos==len) { len=(int)fread(buf,1,sizeof(buf),stdin); pos=0; if(!len) return EOF; }
    return buf[pos++];
}
static bool readint(int &x) {
    int c; do { c=nextchar(); if(c==EOF) return false; } while(c<=32);
    int sign=1; if(c=='-') { sign=-1; c=nextchar(); }
    x=0; while(c>='0' && c<='9') { x=x*10+c-'0'; c=nextchar(); }
    x*=sign; return true;
}
static int a[500000];
int main() {
    int n,b;
    while(readint(n) && readint(b) && n!=-1) {
        int hi=1;
        for(int i=0;i<n;++i) { readint(a[i]); if(a[i]>hi) hi=a[i]; }
        int lo=1;
        while(lo<hi) {
            int mid=lo+(hi-lo)/2, need=0;
            for(int i=0;i<n && need<=b;++i) need+=1+(a[i]-1)/mid;
            if(need<=b) hi=mid; else lo=mid+1;
        }
        printf("%d\n",lo);
    }
    return 0;
}
