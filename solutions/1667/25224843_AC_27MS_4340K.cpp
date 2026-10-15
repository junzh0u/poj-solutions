// POJ 1667 - Peg Game
// Model: gpt-6.1-sol
// Depth-first search removes one peg per jump, caching failed positions.
// Scan pegs in row order and jumps up, left, right, down: the discussion
// and its posted test data correct the statement's down/right ordering.
#include <cstdio>
#include <cstring>
using namespace std;
typedef unsigned long long Bits;
struct Move { int a,b,c; Bits need,land,flip; } moves[196];
int nm,goal,path[49],solution[49],length;
Bits failed[524288];
bool dfs(Bits s,int count,int depth) {
    if(count==1) { if(s!=(Bits(1)<<goal)) return false;
        length=depth; memcpy(solution,path,depth*sizeof(int)); return true; }
    unsigned h=(unsigned)((s^(s>>23)^(s>>39))*2654435761U)&524287;
    if(failed[h]==s) return false;
    for(int i=0;i<nm;++i) if((s&moves[i].need)==moves[i].need && !(s&moves[i].land)) {
        path[depth]=i;
        if(dfs(s^moves[i].flip,count-1,depth+1)) return true;
    }
    failed[h]=s;
    return false;
}
int main() {
    int cases; scanf("%d",&cases);
    for(int tc=1;tc<=cases;++tc) {
        char b[49],ch; Bits s=0; int count=0;
        for(int i=0;i<49;++i) { scanf(" %c",&ch); b[i]=ch;
            if(ch=='o'||ch=='O') {s|=Bits(1)<<i;++count;}
            if(ch=='O'||ch=='E') goal=i;
        }
        nm=0; int dr[4]={-1,0,0,1},dc[4]={0,-1,1,0};
        for(int r=0;r<7;++r) for(int c=0;c<7;++c) if(b[r*7+c]!='x')
            for(int d=0;d<4;++d) {
                int rr=r+2*dr[d],cc=c+2*dc[d];
                if(rr<0||rr>=7||cc<0||cc>=7) continue;
                int a=r*7+c,mid=(r+dr[d])*7+c+dc[d],end=rr*7+cc;
                if(b[mid]=='x'||b[end]=='x') continue;
                Move &m=moves[nm++]; m.a=a;m.b=mid;m.c=end;
                m.need=(Bits(1)<<a)|(Bits(1)<<mid);m.land=Bits(1)<<end;m.flip=m.need|m.land;
            }
        memset(failed,0,sizeof(failed));
        if(tc>1) puts(""); printf("Dataset %d:\n",tc);
        if(!dfs(s,count,0)) puts("No solution.");
        else for(int i=0;i<length;++i) {Move &m=moves[solution[i]];
            printf("%d. (%d, %d) to (%d, %d)\n",i+1,m.a%7+1,m.a/7+1,m.c%7+1,m.c/7+1);}
    }
    return 0;
}
