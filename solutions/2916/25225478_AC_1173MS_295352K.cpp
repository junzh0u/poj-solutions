// POJ 2916 - The Wolves and the Sheep Plus
// Model: gpt-6.1-sol
// Retrograde minimax on unordered triples of wolves and the sheep position.
// Wolves minimize capture time; the sheep maximizes it. Unresolved states draw.
// Only a sheep turn with no legal move is terminal; immobile wolves must pass.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
struct Triple { unsigned char p[3], count, target[12]; int next[12]; };
int choose2[101],choose3[101];
int rank3(int a,int b,int c) {
    if(a>b) swap(a,b); if(b>c) swap(b,c); if(a>b) swap(a,b);
    return a+choose2[b]+choose3[c];
}
int main() {
    for(int i=0;i<=100;++i) {choose2[i]=i*(i-1)/2; choose3[i]=i*(i-1)*(i-2)/6;}
    int n,m;
    while(scanf("%d%d",&n,&m)==2) {
        char board[10][12]; int id[10][10],k=0,sheep=0,w[3],wc=0;
        for(int i=0;i<n;++i) {scanf("%s",board[i]); for(int j=0;j<m;++j) id[i][j]=board[i][j]=='#'?-1:k++;}
        int adj[100][4],deg[100]={0}; bool near[100][100]; memset(near,0,sizeof(near));
        int dr[]={-1,1,0,0},dc[]={0,0,-1,1};
        for(int i=0;i<n;++i) for(int j=0;j<m;++j) if(id[i][j]>=0) {
            int u=id[i][j]; if(board[i][j]=='X') sheep=u; if(board[i][j]=='O') w[wc++]=u;
            for(int z=0;z<4;++z) {int r=i+dr[z],c=j+dc[z]; if(r>=0&&r<n&&c>=0&&c<m&&id[r][c]>=0) {adj[u][deg[u]++]=id[r][c];near[u][id[r][c]]=true;}}
        }
        int triples=choose3[k]; vector<Triple> t(triples);
        for(int c=2;c<k;++c) for(int b=1;b<c;++b) for(int a=0;a<b;++a) {
            Triple &x=t[rank3(a,b,c)]; x.p[0]=a;x.p[1]=b;x.p[2]=c;x.count=0;
            for(int z=0;z<3;++z) for(int j=0;j<deg[x.p[z]];++j) {
                int v=adj[x.p[z]][j]; if(v==a||v==b||v==c) continue;
                int p[3]={a,b,c};p[z]=v; int h=x.count++;x.target[h]=v;x.next[h]=rank3(p[0],p[1],p[2]);
            }
        }
        int total=triples*k*2; vector<int> distance(total,-1),q(total); vector<unsigned char> remaining(total,0);
        int head=0,tail=0;
        for(int r=0;r<triples;++r) {
            Triple &x=t[r];
            for(int s=0;s<k;++s) if(s!=x.p[0]&&s!=x.p[1]&&s!=x.p[2]) {
                int index=(r*k+s)*2; int d=deg[s]-near[s][x.p[0]]-near[s][x.p[1]]-near[s][x.p[2]];
                remaining[index+1]=(unsigned char)d;
                if(d==0) {distance[index+1]=0;q[tail++]=index+1;}
            }
        }
        int initial=(rank3(w[0],w[1],w[2])*k+sheep)*2;
        while(head<tail&&distance[initial]<0) {
            int state=q[head++],s=(state/2)%k,r=(state/2)/k,nd=distance[state]+1; Triple &x=t[r];
            if(state&1) {
                int legal=0;
                for(int j=0;j<x.count;++j) if(x.target[j]!=s) {
                    ++legal;int prev=(x.next[j]*k+s)*2;
                    if(distance[prev]<0) {distance[prev]=nd;q[tail++]=prev;}
                }
                if(legal==0) {int prev=state-1;if(distance[prev]<0) {distance[prev]=nd;q[tail++]=prev;}}
            } else {
                for(int j=0;j<deg[s];++j) {
                    int v=adj[s][j];if(v==x.p[0]||v==x.p[1]||v==x.p[2]) continue;
                    int prev=(r*k+v)*2+1;
                    if(distance[prev]<0&&--remaining[prev]==0) {distance[prev]=nd;q[tail++]=prev;}
                }
            }
        }
        if(distance[initial]<0) puts("Lucky Mmxl will never be captured:)");
        else printf("Poor Mmxl may be captured after %d steps:(\n",distance[initial]);
    }
    return 0;
}
