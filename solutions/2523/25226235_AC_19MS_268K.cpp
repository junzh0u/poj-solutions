// POJ 2523 - ASCII Labyrinth
// Model: gpt-6-astra
// Enumerate simple square paths, respecting straight and turning pieces.
// Count all paths, not just shortest ones; endpoint rotations are not new
// paths. Choose outward-facing endpoint arms when rendering a shortest path.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int m,n,kind[64],adj[64][4],arm[64],best[64],bestlen;
bool used[64];
unsigned long countPaths;
// Directions: up, right, down, left.
void search(int v,int entry,int depth) {
    if(v==m*n-1) {
        int out=kind[v]==1 ? (entry+2)%4 : (entry==0?1:2);
        arm[v]=(1<<entry)|(1<<out);
        ++countPaths;
        if(depth<bestlen) { bestlen=depth; copy(arm,arm+m*n,best); }
        arm[v]=0;
        return;
    }
    used[v]=true;
    for(int d=0;d<4;++d) {
        if(d==entry) continue;
        if((kind[v]==1)!=(d==(entry+2)%4)) continue;
        int w=adj[v][d];
        if(w<0 || used[w] || !kind[w]) continue;
        arm[v]=(1<<entry)|(1<<d);
        search(w,(d+2)%4,depth+1);
    }
    arm[v]=0; used[v]=false;
}
int main() {
    int cases; cin>>cases;
    while(cases--) {
        cin>>m>>n;
        string line; getline(cin,line);
        vector<string> board(4*m+1);
        for(int i=0;i<=4*m;++i) { getline(cin,board[i]); if(!board[i].empty() && board[i][board[i].size()-1]=='\r') board[i].erase(board[i].size()-1); }
        for(int r=0;r<m;++r) for(int c=0;c<n;++c) {
            int v=r*n+c;
            kind[v]=0; arm[v]=0; used[v]=false;
            if(board[4*r+2][4*c+2]=='*') {
                bool up=board[4*r+1][4*c+2]=='*';
                bool down=board[4*r+3][4*c+2]=='*';
                bool left=board[4*r+2][4*c+1]=='*';
                bool right=board[4*r+2][4*c+3]=='*';
                kind[v]=((up&&down)||(left&&right))?1:2;
            }
            adj[v][0]=r?v-n:-1; adj[v][1]=c+1<n?v+1:-1;
            adj[v][2]=r+1<m?v+n:-1; adj[v][3]=c?v-1:-1;
        }
        countPaths=0; bestlen=m*n+1;
        if(kind[0] && kind[m*n-1]) {
            if(m*n==1) { countPaths=1; best[0]=kind[0]==1?10:3; }
            else {
                // Each possible first step has exactly one external entrance.
                used[0]=true;
                for(int d=1;d<=2;++d) {
                    int w=adj[0][d]; if(w<0 || !kind[w]) continue;
                    int entry=kind[0]==1?(d+2)%4:(d==1?0:3);
                    arm[0]=(1<<entry)|(1<<d);
                    search(w,(d+2)%4,2);
                }
            }
        }
        if(countPaths) {
            vector<string> out(4*m+1,string(4*n+1,' '));
            for(int r=0;r<=4*m;++r) for(int c=0;c<=4*n;++c)
                if(r%4==0) out[r][c]=c%4==0?'+':'-';
                else if(c%4==0) out[r][c]='|';
            for(int v=0;v<m*n;++v) if(best[v]) {
                int r=4*(v/n)+2,c=4*(v%n)+2;
                out[r][c]='*';
                if(best[v]&1) out[r-1][c]='*';
                if(best[v]&2) out[r][c+1]='*';
                if(best[v]&4) out[r+1][c]='*';
                if(best[v]&8) out[r][c-1]='*';
            }
            for(int r=0;r<=4*m;++r) cout<<out[r]<<'\n';
        }
        cout<<"Number of solutions: "<<countPaths<<'\n';
    }
}
