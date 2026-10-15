// POJ 2237 - Can 1 Marine win 2 Zerglings?
// Model: gpt-6.1-sol
// Breadth-first search of complete rounds, retaining the greatest marine HP
// seen for each position/enemy-health state. Earlier states with at least
// as much HP dominate later ones. Shots have no range restriction.
// Both Z and z identify enemies, including the historical duplicate-label case.
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
int n,H,adj[25][4],distv[25][25],stepv[25][25];
vector<unsigned char> best;
int key(int p,int a,int b,int x,int y) {
    if (!x) a=0;
    if (!y) b=0;
    if (a>b || (a==b && x>y)) { swap(a,b); swap(x,y); }
    return ((((p*n+a)*n+b)*H+x)*H+y);
}
int main() {
    char grid[5][6]; int id[5][5], marine=0,enemy[2],ec=0;
    memset(id,-1,sizeof(id));
    for(int r=0;r<5;++r) { cin>>grid[r]; for(int c=0;c<5;++c)
        if(grid[r][c]!='1') id[r][c]=n++; }
    for(int r=0;r<5;++r) for(int c=0;c<5;++c) if(id[r][c]>=0) {
        int v=id[r][c];
        if(grid[r][c]=='M') marine=v;
        if(grid[r][c]=='Z'||grid[r][c]=='z') enemy[ec++]=v;
        int dr[4]={0,-1,0,1},dc[4]={-1,0,1,0};
        for(int k=0;k<4;++k) { int rr=r+dr[k],cc=c+dc[k];
            adj[v][k]=(rr>=0&&rr<5&&cc>=0&&cc<5)?id[rr][cc]:-1; }
    }
    int mh,z; cin>>mh>>z;
    if(z*2>34) { cout<<"LOSE\n"; return 0; }
    H=z+1;
    for(int p=0;p<n;++p) {
        for(int v=0;v<n;++v) distv[p][v]=100;
        int q[25],head=0,tail=0; q[tail++]=p; distv[p][p]=0;
        while(head<tail) { int v=q[head++]; for(int k=0;k<4;++k) {
            int w=adj[v][k]; if(w>=0&&distv[p][w]==100) {
                distv[p][w]=distv[p][v]+1; q[tail++]=w; }
        }}
        for(int v=0;v<n;++v) { stepv[p][v]=v;
            if(distv[p][v]>1&&distv[p][v]<100) for(int k=0;k<4;++k) {
                int w=adj[v][k]; if(w>=0&&distv[p][w]==distv[p][v]-1) {
                    stepv[p][v]=w; break; }
            }
        }
    }
    best.assign(n*n*n*H*H,0);
    vector<int> cur,nxt;
    int start=key(marine,enemy[0],enemy[1],z,z);
    best[start]=(unsigned char)mh; cur.push_back(start*17+mh);
    for(int round=1;round<=34&&!cur.empty();++round) {
        nxt.clear();
        for(size_t i=0;i<cur.size();++i) {
            int code=cur[i]/17,hp=cur[i]%17,t=code;
            int y=t%H;t/=H;int x=t%H;t/=H;
            int b=t%n;t/=n;int a=t%n;int p=t/n;
            for(int action=0;action<6;++action) {
                int np=p,xx=x,yy=y;
                if(action<4) { np=adj[p][action];
                    if(np<0||(x&&np==a)||(y&&np==b)) continue;
                } else if(action==4) { if(!xx) continue; --xx; }
                else { if(!yy) continue; --yy; }
                if(!xx&&!yy) { cout<<"WIN\n"<<round<<"\n"; return 0; }
                if(round+xx+yy>34) continue;
                bool hit1=xx&&distv[np][a]==1,hit2=yy&&distv[np][b]==1;
                int nh=hp-hit1-hit2+(hit1&&hit2&&a==b);
                if(nh<=0) continue;
                int na=xx?stepv[np][a]:0,nb=yy?stepv[np][b]:0;
                int nk=key(np,na,nb,xx,yy);
                if(best[nk]<nh) { best[nk]=(unsigned char)nh; nxt.push_back(nk*17+nh); }
            }
        }
        cur.swap(nxt);
    }
    cout<<"LOSE\n";
}
