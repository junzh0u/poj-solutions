// POJ 2422 - The Wolves and the Sheep
// Model: gpt-6.1-sol
// Retrograde reachability game with unordered wolf triples. Wolves need one
// winning successor; sheep lose only when every legal successor loses.
// Boundary sheep positions are safe, and a wolf pass is legal only if stuck.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Triple { unsigned char p[3]; };
int c2[101],c3[101];
int rank3(int a,int b,int c) {
    if(a>b)swap(a,b); if(b>c)swap(b,c); if(a>b)swap(a,b);
    return c3[c]+c2[b]+a;
}
bool solve(int r,int c,char board[10][11]) {
    int id[10][10],n=0,k=0,si=-1,sw[3],nw=0;
    int adj[100][4],nd[100]={0},inner[100],pos[100];
    for(int i=0;i<r;i++)for(int j=0;j<c;j++)id[i][j]=board[i][j]=='X'?-1:n++;
    int dr[4]={-1,1,0,0},dc[4]={0,0,-1,1};
    for(int i=0;i<r;i++)for(int j=0;j<c;j++)if(id[i][j]>=0) {
        int u=id[i][j]; inner[u]=-1;
        if(i>0&&j>0&&i<r-1&&j<c-1) {inner[u]=k;pos[k++]=u;}
        if(board[i][j]=='S')si=u;
        if(board[i][j]=='W')sw[nw++]=u;
        for(int d=0;d<4;d++) {
            int x=i+dr[d],y=j+dc[d];
            if(x>=0&&x<r&&y>=0&&y<c&&id[x][y]>=0)adj[u][nd[u]++]=id[x][y];
        }
    }
    if(inner[si]<0)return false;
    int nt=c3[n];
    vector<Triple> triples(nt);
    for(int z=2;z<n;z++)for(int y=1;y<z;y++)for(int x=0;x<y;x++) {
        Triple &t=triples[rank3(x,y,z)];t.p[0]=x;t.p[1]=y;t.p[2]=z;
    }
    int total=nt*k*2,target=(rank3(sw[0],sw[1],sw[2])*k+inner[si])*2;
    vector<unsigned char> state(total,0);
    vector<int> q; q.reserve(total);
    for(int t=0;t<nt;t++)for(int s=0;s<k;s++) {
        int u=pos[s],a=triples[t].p[0],b=triples[t].p[1],cc=triples[t].p[2];
        if(u==a||u==b||u==cc)continue;
        int deg=0;
        for(int d=0;d<nd[u];d++){int v=adj[u][d];if(v!=a&&v!=b&&v!=cc)deg++;}
        int ix=(t*k+s)*2+1;
        state[ix]=deg;
        if(deg==0){state[ix]=255;q.push_back(ix);}
    }
    for(size_t head=0;head<q.size();head++) {
        int ix=q[head],s=(ix/2)%k,t=(ix/2)/k,u=pos[s];
        int w[3]={triples[t].p[0],triples[t].p[1],triples[t].p[2]};
        if(ix&1) {
            bool moved=false;
            for(int a=0;a<3;a++)for(int d=0;d<nd[w[a]];d++) {
                int v=adj[w[a]][d];
                if(v==u||v==w[(a+1)%3]||v==w[(a+2)%3])continue;
                moved=true;
                int pre=(rank3(v,w[(a+1)%3],w[(a+2)%3])*k+s)*2;
                if(state[pre]!=255){state[pre]=255;q.push_back(pre);if(pre==target)return true;}
            }
            if(!moved&&state[ix-1]!=255){state[ix-1]=255;q.push_back(ix-1);if(ix-1==target)return true;}
        } else {
            for(int d=0;d<nd[u];d++) {
                int v=adj[u][d];
                if(inner[v]<0||v==w[0]||v==w[1]||v==w[2])continue;
                int pre=(t*k+inner[v])*2+1;
                if(state[pre]!=255&&--state[pre]==0){state[pre]=255;q.push_back(pre);}
            }
        }
    }
    return state[target]==255;
}
int main(){
    for(int i=0;i<=100;i++){c2[i]=i*(i-1)/2;c3[i]=i*(i-1)*(i-2)/6;}
    int r,c;char b[10][11];
    while(scanf("%d%d",&r,&c)==2){for(int i=0;i<r;i++)scanf("%s",b[i]);
        puts(solve(r,c,b)?"Poor Mmxl is in danger:(":"Lucky Mmxl is safe:)");}
    return 0;
}
