// POJ 2919 - Traveling Queen Problem
// Model: gpt-6.1-sol
// Breadth-first search over (visited knights, square). Sorted destinations
// make first discovery the lexicographically first shortest path. Visits
// occur only at stopping squares; several knights may be visited together.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
using namespace std;
int main() {
    int T; cin >> T;
    for (int tc=1; tc<=T; ++tc) {
        char board[64]; int start=0, bishop=0; vector<int> knights;
        string s;
        for(int y=7;y>=0;--y) {
            cin >> s;
            for(int x=0;x<8;++x) {
                int p=x*8+y; board[p]=s[x];
                if(s[x]=='Q') {start=p;board[p]='.';}
                if(s[x]=='B') bishop=p;
                if(s[x]=='N') knights.push_back(p);
            }
        }
        vector<int> adj[64]; int visit[64]={0}; bool finish[64]={false};
        for(int p=0;p<64;++p) if(board[p]=='.') {
            int x=p/8,y=p%8;
            for(int k=0;k<(int)knights.size();++k)
                if(abs(x-knights[k]/8)<=1 && abs(y-knights[k]%8)<=1) visit[p]|=1<<k;
            finish[p]=abs(x-bishop/8)<=1 && abs(y-bishop%8)<=1;
            for(int dx=-1;dx<=1;++dx) for(int dy=-1;dy<=1;++dy) if(dx||dy) {
                int a=x+dx,b=y+dy;
                while(a>=0 && a<8 && b>=0 && b<8 && board[a*8+b]=='.') {
                    adj[p].push_back(a*8+b); a+=dx;b+=dy;
                }
            }
            sort(adj[p].begin(),adj[p].end());
        }
        int full=(1<<knights.size())-1, root=visit[start]*64+start;
        vector<int> parent((full+1)*64,-1), q; q.reserve((full+1)*64);
        parent[root]=root; q.push_back(root); int goal=-1;
        for(int head=0;head<(int)q.size();++head) {
            int state=q[head],p=state%64,m=state/64;
            if(m==full && finish[p]) {goal=state;break;}
            for(int j=0;j<(int)adj[p].size();++j) {
                int v=adj[p][j],next=(m|visit[v])*64+v;
                if(parent[next]<0) {parent[next]=state;q.push_back(next);}
            }
        }
        cout << "Scenario #" << tc << ":\n";
        if(goal<0) cout << "impossible\n\n";
        else {
            vector<int> path;
            while(goal!=root) {path.push_back(goal%64);goal=parent[goal];}
            path.push_back(start); reverse(path.begin(),path.end());
            for(int i=0;i<(int)path.size();++i) cout << char('a'+path[i]/8) << char('1'+path[i]%8);
            cout << "\n\n";
        }
    }
}
