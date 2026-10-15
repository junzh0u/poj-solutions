// POJ 1997 - Word Puzzle
// Model: gpt-6.1-sol
// Insert the words in a trie, then walk from each cell in all eight
// toroidal directions. Mark the longest terminal prefix on each walk;
// it covers every shorter matching word on the same walk as well.
// Walks may revisit cells, including when a word exceeds a dimension.
#include <cstdio>
#include <cstring>
int nextNode[20001][26], terminal[20001], nodes;
char board[200][201], word[21];
bool erased[200][200];
int main() {
    int cases; scanf("%d", &cases);
    while (cases--) {
        int r,s,k; scanf("%d%d", &r,&s);
        for(int i=0;i<r;++i) scanf("%s",board[i]);
        memset(nextNode,0,sizeof(nextNode));
        memset(terminal,0,sizeof(terminal));
        memset(erased,0,sizeof(erased)); nodes=1;
        scanf("%d",&k);
        for(int i=0;i<k;++i) {
            scanf("%s",word); int u=0;
            for(int j=0;word[j];++j) {
                int c=word[j]-'a';
                if(!nextNode[u][c]) nextNode[u][c]=nodes++;
                u=nextNode[u][c];
            }
            terminal[u]=1;
        }
        for(int i=0;i<r;++i) for(int j=0;j<s;++j)
            for(int di=-1;di<=1;++di) for(int dj=-1;dj<=1;++dj) {
                if(di==0 && dj==0) continue;
                int x=i,y=j,u=0,last=0;
                for(int len=1;len<=20;++len) {
                    u=nextNode[u][board[x][y]-'a'];
                    if(!u) break;
                    if(terminal[u]) last=len;
                    x+=di; y+=dj;
                    if(x<0) x+=r;
                    if(x==r) x=0;
                    if(y<0) y+=s;
                    if(y==s) y=0;
                }
                x=i;y=j;
                for(int len=0;len<last;++len) {
                    erased[x][y]=true;
                    x+=di;y+=dj;
                    if(x<0) x+=r;
                    if(x==r) x=0;
                    if(y<0) y+=s;
                    if(y==s) y=0;
                }
            }
        for(int i=0;i<r;++i) for(int j=0;j<s;++j)
            if(!erased[i][j]) putchar(board[i][j]);
        putchar('\n');
    }
    return 0;
}
