// POJ 1858 - Interesting Maze Game
// Model: gpt-6.1-sol
// Enumerate all insertions and rotations, then flood-fill reciprocal paths.
// A displaced piece wraps onto the inserted card; a displaced target is lost.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
int dr[4]={-1,0,1,0}, dc[4]={0,1,0,-1};
int readcard(const string &a,const string &b,const string &c,int k) {
    return (a[k+1]=='|'?1:0)|(b[k+2]=='-'?2:0)|(c[k+1]=='|'?4:0)|(b[k]=='-'?8:0);
}
bool reachable(int a[7][7],int sr,int sc,int tr,int tc) {
    bool seen[7][7]; memset(seen,0,sizeof(seen));
    int q[49],head=0,tail=0; q[tail++]=sr*7+sc; seen[sr][sc]=true;
    while(head<tail) {
        int p=q[head++],r=p/7,c=p%7;
        if(r==tr && c==tc) return true;
        for(int d=0;d<4;++d) {
            int x=r+dr[d],y=c+dc[d];
            if(x>=0&&x<7&&y>=0&&y<7&&!seen[x][y]&&(a[r][c]&(1<<d))&&(a[x][y]&(1<<((d+2)%4)))) {
                seen[x][y]=true; q[tail++]=x*7+y;
            }
        }
    }
    return false;
}
int main() {
    int sr,sc,tr,tc;
    while(cin>>sr>>sc>>tr>>tc && sr) {
        --sr;--sc;--tr;--tc;
        int a[7][7],extra; string s[3];
        for(int r=0;r<7;++r) {
            for(int i=0;i<3;++i) { cin>>ws; getline(cin,s[i]); }
            for(int c=0;c<7;++c) a[r][c]=readcard(s[0],s[1],s[2],4*c);
        }
        for(int i=0;i<3;++i) { cin>>ws; getline(cin,s[i]); }
        extra=readcard(s[0],s[1],s[2],0);
        bool win=false;
        for(int d=0;d<4;++d) for(int line=1;line<7;line+=2) for(int rot=0;rot<4;++rot) {
            int b[7][7]; memcpy(b,a,sizeof(b));
            int rr=sr,cc=sc,tt=tr,uu=tc;
            for(int r=0;r<7;++r) for(int c=0;c<7;++c) {
                if((d%2==0?c:r)!=line) continue;
                int x=r+dr[d],y=c+dc[d];
                if(x>=0&&x<7&&y>=0&&y<7) b[x][y]=a[r][c];
            }
            int er=d==0?6:(d==2?0:line),ec=d==1?0:(d==3?6:line);
            int mask=extra;
            for(int i=0;i<rot;++i) mask=((mask<<1)&15)|(mask>>3);
            b[er][ec]=mask;
            if((d%2==0?sc:sr)==line) {rr=(sr+dr[d]+7)%7;cc=(sc+dc[d]+7)%7;}
            if((d%2==0?tc:tr)==line) {tt+=dr[d];uu+=dc[d];}
            if(tt<0||tt>6||uu<0||uu>6) continue;
            if(reachable(b,rr,cc,tt,uu)) win=true;
        }
        cout<<(win?"You can win in one move.":"Bad luck!")<<'\n';
    }
    return 0;
}
