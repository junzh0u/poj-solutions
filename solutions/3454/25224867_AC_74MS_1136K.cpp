// POJ 3454 - Hypertheseus
// Model: gpt-6.1-sol
// Flatten the recursive map in mixed-radix order. BFS from Theseus with
// Minotaur blocked reaches the sword; BFS from Minotaur gives both remaining
// distances. The sword must be collected before entering Minotaur's cell.
#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
int d, n[20], stride[20];
vector<char> cell;
vector<int> dista, que;
void bfs(int start, int blocked) {
    int total = (int)cell.size();
    dista.assign(total, -1);
    que.resize(total);
    int head=0, tail=0;
    que[tail++]=start; dista[start]=0;
    while(head<tail) {
        int u=que[head++];
        for(int k=0;k<d;++k) {
            int coordinate=(u/stride[k])%n[k];
            for(int sign=-1;sign<=1;sign+=2) {
                if((sign<0 && coordinate==0) || (sign>0 && coordinate==n[k]-1)) continue;
                int v=u+sign*stride[k];
                if(v==blocked || cell[v]=='#' || dista[v]>=0) continue;
                dista[v]=dista[u]+1; que[tail++]=v;
            }
        }
    }
}
int main() {
    while(scanf("%d", &d)==1 && d) {
        int total=1;
        for(int k=0;k<d;++k) { scanf("%d", &n[k]); stride[k]=total; total*=n[k]; }
        cell.resize(total);
        int t=-1,s=-1,m=-1;
        for(int i=0;i<total;++i) {
            scanf(" %c", &cell[i]);
            if(cell[i]=='T') t=i;
            if(cell[i]=='S') s=i;
            if(cell[i]=='M') m=i;
        }
        bfs(t,m);
        int first=dista[s];
        if(first<0) { puts("No solution. Poor Theseus!"); continue; }
        bfs(m,-1);
        if(dista[s]<0 || dista[t]<0) puts("No solution. Poor Theseus!");
        else printf("Theseus needs %d steps.\n",first+dista[s]+dista[t]);
    }
    return 0;
}
