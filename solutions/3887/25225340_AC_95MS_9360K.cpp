// POJ 3887 - The Pharaoh's Curse
// Model: gpt-6.1-sol
// Breadth-first search over the player and two interchangeable box positions.
// Each edge is one walking or pushing step. Missing boxes occupy a dummy cell.
// The exit can be entered only when every button is held down by a box;
// standing on a button cannot keep it pressed after taking the exit step.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int cases; cin >> cases;
    while (cases--) {
        int h,w; cin >> h >> w;
        vector<string> g(h); for(int r=0;r<h;++r) cin >> g[r];
        int id[50][50], n=0, start=-1, exit=-1;
        vector<int> boxes, buttons;
        for(int r=0;r<h;++r) for(int c=0;c<w;++c) {
            id[r][c]=-1;
            if(g[r][c]!='#') {
                id[r][c]=n;
                if(g[r][c]=='S') start=n;
                if(g[r][c]=='E') exit=n;
                if(g[r][c]=='X') boxes.push_back(n);
                if(g[r][c]=='B') buttons.push_back(n);
                ++n;
            }
        }
        if(buttons.size()>boxes.size()) { cout << "impossible\n"; continue; }
        int base=n+1;
        while(boxes.size()<2) boxes.push_back(n);
        sort(boxes.begin(),boxes.end());
        vector<int> adj(n*4,-1);
        const int dr[4]={-1,1,0,0}, dc[4]={0,0,-1,1};
        for(int r=0;r<h;++r) for(int c=0;c<w;++c) if(id[r][c]>=0)
            for(int d=0;d<4;++d) {
                int rr=r+dr[d], cc=c+dc[d];
                if(rr>=0 && rr<h && cc>=0 && cc<w) adj[id[r][c]*4+d]=id[rr][cc];
            }
        int total=n*base*base;
        vector<unsigned char> seen(total,0);
        vector<int> q(total);
        int head=0,tail=0,steps=0,answer=-1;
        int first=(start*base+boxes[0])*base+boxes[1];
        q[tail++]=first; seen[first]=1;
        while(head<tail && answer<0) {
            int end=tail;
            while(head<end && answer<0) {
                int state=q[head++], b=state%base, a=(state/base)%base, p=state/base/base;
                for(int d=0;d<4;++d) {
                    int t=adj[p*4+d]; if(t<0) continue;
                    int x=a,y=b;
                    if(t==a || t==b) {
                        int u=adj[t*4+d];
                        // A box on the sole boundary exit cannot be moved out
                        // of the way again, so such a push never helps escape.
                        if(u<0 || u==a || u==b || u==exit) continue;
                        if(t==a) x=u; else y=u;
                        if(x>y) swap(x,y);
                    }
                    if(t==exit) {
                        bool open=true;
                        for(unsigned k=0;k<buttons.size();++k)
                            if(buttons[k]!=x && buttons[k]!=y) open=false;
                        if(open) { answer=steps+1; break; }
                        continue;
                    }
                    int next=(t*base+x)*base+y;
                    if(!seen[next]) { seen[next]=1; q[tail++]=next; }
                }
            }
            ++steps;
        }
        if(answer<0) cout << "impossible\n"; else cout << answer << '\n';
    }
}
