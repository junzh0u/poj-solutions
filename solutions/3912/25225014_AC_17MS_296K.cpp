// POJ 3912 - Up and Down
// Model: gpt-6.1-sol
// Keep every square within one jump of a chute/ladder beginning, plus
// all destinations and the two endpoints. Ordinary stretches without a
// beginning cost ceil(distance / s); short edges can jump over beginnings.
// Landing on a beginning forces its transfer in the same turn.
// Dijkstra on this compressed graph avoids dependence on the board length.
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <utility>
using namespace std;
int main() {
    int w,s,p;
    while (cin >> w && w) {
        cin >> s >> p;
        vector<int> b(p),e(p),x;
        x.push_back(0); x.push_back(w);
        for (int i=0;i<p;++i) {
            cin >> b[i] >> e[i];
            x.push_back(e[i]);
            for (int j=-s;j<=s;++j)
                if (b[i]+j>=0 && b[i]+j<=w) x.push_back(b[i]+j);
        }
        sort(x.begin(),x.end());
        x.erase(unique(x.begin(),x.end()),x.end());
        int n=x.size();
        vector<int> dest(n),first(n,w+1);
        for(int i=0;i<n;++i) {
            dest[i]=i;
            for(int j=0;j<p;++j) {
                if(b[j]==x[i]) dest[i]=lower_bound(x.begin(),x.end(),e[j])-x.begin();
                if(b[j]>x[i]) first[i]=min(first[i],b[j]);
            }
        }
        vector<int> d(n,2000000000);
        priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > > q;
        d[0]=0; q.push(make_pair(0,0));
        while(!q.empty()) {
            int cost=q.top().first,u=q.top().second; q.pop();
            if(cost!=d[u]) continue;
            if(u==n-1) break;
            for(int v=u+1;v<n;++v) {
                int gap=x[v]-x[u];
                if(x[v]>first[u] && gap>s) break;
                int to=dest[v],nd=cost+(gap+s-1)/s;
                if(nd<d[to]) { d[to]=nd; q.push(make_pair(nd,to)); }
            }
        }
        cout << d[n-1] << '\n';
    }
    return 0;
}
