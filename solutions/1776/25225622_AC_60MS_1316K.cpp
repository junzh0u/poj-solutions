// POJ 1776 - Task Sequences
// Model: gpt-6-astra
// Insert each task before the first path vertex it can reach. All earlier
// vertices reach it, so insertion preserves a directed Hamiltonian path.
// The semicomplete-graph guarantee makes one startup always optimal.
// Cases continue to EOF; buffered input avoids per-integer scanf overhead.
#include <cstdio>
#include <cstring>
static unsigned char a[1000][1000];
static int path[1000];
static char buf[65536];
static int pos=0, len=0;
int nextchar() {
    if(pos==len) { len=(int)fread(buf,1,sizeof(buf),stdin); pos=0; }
    return pos<len ? (unsigned char)buf[pos++] : EOF;
}
int readint() {
    int c; do { c=nextchar(); } while(c!=EOF && c<=32);
    if(c==EOF) return -1;
    int x=0;
    do { x=x*10+c-'0'; c=nextchar(); } while(c>32 && c!=EOF);
    return x;
}
int main() {
    int n;
    while((n=readint())>0) {
        for(int i=0;i<n;++i) for(int j=0;j<n;++j) a[i][j]=(unsigned char)readint();
        for(int v=0;v<n;++v) {
            int p=0;
            while(p<v && !a[v][path[p]]) ++p;
            for(int j=v;j>p;--j) path[j]=path[j-1];
            path[p]=v;
        }
        printf("1\n%d\n",n);
        for(int i=0;i<n;++i) printf(i ? " %d" : "%d",path[i]+1);
        printf("\n");
    }
    return 0;
}
