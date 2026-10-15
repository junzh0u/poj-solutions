// POJ 3976 - Kings
// Model: gpt-6.1-sol
// Dijkstra over both king positions and the bitmask of black cells.
// Simultaneous moves price both destinations using the old colors, then
// repaint both cells; swapping positions is legal because endpoints differ.
#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <utility>
#include <string>
using namespace std;
int main() {
    int t; cin >> t;
    for (int cs=1; cs<=t; ++cs) {
        int n; cin >> n;
        vector< vector<int> > adj(n);
        for(int i=0;i<n;++i) {
            string s; cin>>s;
            for(int j=0;j<n;++j) if(i!=j && s[j]=='1') adj[i].push_back(j);
        }
        int wc,bc,cc,ws,we,bs,be;
        cin>>wc>>bc>>cc>>ws>>we>>bs>>be;
        int nn=n*n, total=(1<<n)*nn;
        vector<int> d(total,1000000000);
        typedef pair<int,int> Item;
        priority_queue<Item,vector<Item>,greater<Item> > q;
        int start=(1<<bs)*nn+ws*n+bs;
        d[start]=0; q.push(Item(0,start));
        int answer=-1;
        while(!q.empty()) {
            int cost=q.top().first, id=q.top().second; q.pop();
            if(cost!=d[id]) continue;
            int mask=id/nn, w=id%nn/n, b=id%n;
            if(w==we && b==be) { answer=cost; break; }
            for(int a=-1;a<(int)adj[w].size();++a) {
                int x=(a<0?w:adj[w][a]);
                for(int z=-1;z<(int)adj[b].size();++z) {
                    if(a<0 && z<0) continue;
                    int y=(z<0?b:adj[b][z]);
                    if(x==y) continue;
                    int add;
                    if(a<0) add=bc*((mask&(1<<y))?1:2);
                    else if(z<0) add=wc*((mask&(1<<x))?2:1);
                    else add=cc*((mask&(1<<x))?2:1)*((mask&(1<<y))?1:2);
                    int nextmask=(mask&~(1<<x))|(1<<y);
                    int next=nextmask*nn+x*n+y;
                    if(cost+add<d[next]) {
                        d[next]=cost+add; q.push(Item(d[next],next));
                    }
                }
            }
        }
        cout<<"Case #"<<cs<<": ";
        if(answer<0) cout<<"Stalemate\n"; else cout<<answer<<"\n";
    }
}
