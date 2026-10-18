// POJ 3240 - Solution to the n^2 - 1 Puzzle
// Model: gpt-6-astra
// Check sliding-puzzle parity, then fix rows and the final two rows' columns.
// BFS tracks only the blank and one or two selected tiles; all other unfixed
// tiles are interchangeable. Place row ends and final columns as pairs so a
// single fixed tile never traps its partner. Replay paths on the real board.
// The diagram specifies row-major order, with the blank at bottom right.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
using namespace std;
int n,m,board[100],blank,adj[100][4];
bool fixedCell[100];
string answer;
const char *letters="LURD";
void place(int g1,int g2,int endBlank) {
    int a=0,b=0;
    for(int i=0;i<m;++i) {
        if(board[i]==g1+1) a=i;
        if(g2>=0 && board[i]==g2+1) b=i;
    }
    bool pair=g2>=0;
    int count=pair?m*m*m:m*m;
    vector<int> prev(count,-1),q;
    vector<char> step(count);
    int start=pair?(a*m+b)*m+blank:a*m+blank;
    prev[start]=start; q.push_back(start);
    int finish=-1;
    for(size_t head=0;head<q.size();++head) {
        int s=q[head],z=s%m,t=s/m;
        int x=pair?t/m:t,y=pair?t%m:0;
        if(x==g1 && (!pair || y==g2) && (endBlank<0 || z==endBlank)) {
            finish=s; break;
        }
        for(int d=0;d<4;++d) {
            int nz=adj[z][d];
            if(nz<0 || fixedCell[nz]) continue;
            int nx=(nz==x?z:x),ny=(pair && nz==y?z:y);
            int ns=pair?(nx*m+ny)*m+nz:nx*m+nz;
            if(prev[ns]!=-1) continue;
            prev[ns]=s; step[ns]=static_cast<char>(d); q.push_back(ns);
        }
    }
    assert(finish>=0);
    vector<int> path;
    for(int s=finish;s!=start;s=prev[s]) path.push_back(step[s]);
    reverse(path.begin(),path.end());
    for(size_t i=0;i<path.size();++i) {
        int d=path[i],z=adj[blank][d];
        swap(board[blank],board[z]); blank=z; answer+=letters[d];
    }
    fixedCell[g1]=true;
    if(pair) fixedCell[g2]=true;
}
int main() {
    if(!(cin>>n)) return 0;
    m=n*n;
    for(int i=0;i<m;++i) { cin>>board[i]; if(!board[i]) blank=i; }
    int inv=0;
    for(int i=0;i<m;++i) if(board[i])
        for(int j=i+1;j<m;++j) if(board[j] && board[i]>board[j]) ++inv;
    if((n%2 && inv%2) || (!(n%2) && (inv+n-blank/n)%2!=1)) {
        cout<<"No Answer!\n"; return 0;
    }
    if(n==1) { cout<<'\n'; return 0; }
    for(int i=0;i<m;++i) {
        adj[i][0]=i%n?i-1:-1; adj[i][1]=i>=n?i-n:-1;
        adj[i][2]=i%n+1<n?i+1:-1; adj[i][3]=i+n<m?i+n:-1;
    }
    for(int r=0;r<n-2;++r) {
        for(int c=0;c<n-2;++c) place(r*n+c,-1,-1);
        place(r*n+n-2,r*n+n-1,-1);
    }
    for(int c=0;c<n-2;++c) place((n-2)*n+c,(n-1)*n+c,-1);
    place((n-2)*n+n-2,(n-2)*n+n-1,m-1);
    for(int i=0;i<m;++i) assert(board[i]==(i+1)%m);
    cout<<answer<<'\n';
}
