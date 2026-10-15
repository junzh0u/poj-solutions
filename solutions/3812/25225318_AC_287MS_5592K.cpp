// POJ 3812 - Block Game
// Model: gpt-6.1-sol
// Breadth-first search over packed piece positions, with chained hash deduplication.
// Each unobstructed slide of any positive distance is one edge. A clear exit
// contributes one final move; the special piece need not first reach column 4.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
typedef unsigned long long U;
const int H=1<<20, CH=1<<16;
struct Chunk { U key[CH]; int next[CH]; };
int head[H], n, target, len[18], axis[18], fixedAt[18];
U maskAt[18][6];
vector<Chunk*> chunks;
int total;
U keyAt(int i) { return chunks[i>>16]->key[i&(CH-1)]; }
bool add(U key) {
    U z=key^(key>>23); z*=0x2127599bf4325c37ULL; z^=z>>47;
    int h=(unsigned)z&(H-1);
    for(int i=head[h];i!=-1;i=chunks[i>>16]->next[i&(CH-1)])
        if(keyAt(i)==key) return false;
    if((total&(CH-1))==0) chunks.push_back(new Chunk);
    Chunk* c=chunks[total>>16]; int k=total&(CH-1);
    c->key[k]=key; c->next[k]=head[h]; head[h]=total++;
    return true;
}
int solve(U start) {
    memset(head,-1,sizeof(head)); total=0; add(start);
    int begin=0,end=1,depth=0;
    while(begin<end) {
        for(int q=begin;q<end;++q) {
            U s=keyAt(q),occ=0; int pos[18];
            for(int i=0;i<n;++i) { pos[i]=(int)((s>>(3*i))&7); occ|=maskAt[i][pos[i]]; }
            bool clear=true;
            for(int c=pos[target]+2;c<6;++c)
                if(occ&(1ULL<<(fixedAt[target]*6+c))) { clear=false; break; }
            if(clear) return depth+1;
            for(int i=0;i<n;++i) {
                U rest=occ^maskAt[i][pos[i]];
                for(int d=-1;d<=1;d+=2) {
                    for(int p=pos[i]+d;p>=0 && p+len[i]<=6;p+=d) {
                        if(rest&maskAt[i][p]) break;
                        U t=(s&~(7ULL<<(3*i)))|((U)p<<(3*i)); add(t);
                    }
                }
            }
        }
        begin=end; end=total; ++depth;
    }
    return -1;
}
int main() {
    char special[8],b[6][8];
    while(scanf("%7s",special)==1 && special[0]!='*') {
        for(int r=0;r<6;++r) scanf("%7s",b[r]);
        n=0; U start=0;
        for(char ch='A';ch<='Z';++ch) {
            int r0=6,c0=6,r1=-1,c1=-1;
            for(int r=0;r<6;++r) for(int c=0;c<6;++c) if(b[r][c]==ch) {
                if(r<r0) r0=r; if(c<c0) c0=c; if(r>r1) r1=r; if(c>c1) c1=c;
            }
            if(r1<0) continue;
            axis[n]=(r0==r1); len[n]=axis[n]?c1-c0+1:r1-r0+1;
            fixedAt[n]=axis[n]?r0:c0; int p=axis[n]?c0:r0;
            if(ch==special[0]) target=n;
            start|=(U)p<<(3*n);
            for(int k=0;k+len[n]<=6;++k) {
                U m=0;
                for(int j=0;j<len[n];++j) {
                    int cell=axis[n]?fixedAt[n]*6+k+j:(k+j)*6+fixedAt[n];
                    m|=1ULL<<cell;
                }
                maskAt[n][k]=m;
            }
            ++n;
        }
        printf("%d\n",solve(start));
        for(unsigned i=0;i<chunks.size();++i) delete chunks[i];
        chunks.clear();
    }
    return 0;
}
