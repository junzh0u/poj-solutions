// POJ 2717 - Hour Glass
// Model: gpt-6-astra
// Dynamic programming over expiration events in increasing time. At least
// one glass is empty at each event, so only M+N+1 states per time are needed.
// A simultaneous flip counts as one operation; waiting counts as none.
// Both glasses must be flipped initially, as required by the statement.
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
int main() {
    int m,n,T;
    while (cin >> m >> n >> T && m) {
        int w=m+n+1, total=(T+1)*w, inf=1000000;
        vector<int> d(total,inf), pre(total,-1), action(total,0);
        int start=m*w+n-m;
        d[start]=1;
        for(int t=m;t<T;++t) for(int s=0;s<w;++s) {
            int u=t*w+s;
            if(d[u]==inf) continue;
            int a=s>n?s-n:0, b=s<=n?s:0;
            for(int k=0;k<4;++k) {
                int x=(k&1)?m-a:a, y=(k&2)?n-b:b;
                if(x==0 && y==0) continue;
                int dt=x==0?y:(y==0?x:min(x,y));
                if(t+dt>T) continue;
                x=max(0,x-dt); y=max(0,y-dt);
                int v=(t+dt)*w+(x?n+x:y);
                int nd=d[u]+(k!=0);
                if(nd<d[v]) {d[v]=nd;pre[v]=u;action[v]=k;}
            }
        }
        int best=T*w;
        for(int s=1;s<w;++s) if(d[T*w+s]<d[best]) best=T*w+s;
        if(d[best]==inf) cout << "Impossible\n";
        else {
            vector<pair<int,int> > ans;
            while(pre[best]!=-1) {
                if(action[best]) ans.push_back(make_pair(pre[best]/w,action[best]));
                best=pre[best];
            }
            ans.push_back(make_pair(0,3));
            reverse(ans.begin(),ans.end());
            for(size_t i=0;i<ans.size();++i) {
                cout << ans[i].first << ": ";
                if(ans[i].second&1) cout << m;
                if(ans[i].second==3) cout << ',';
                if(ans[i].second&2) cout << n;
                cout << '\n';
            }
        }
        cout << "----------\n";
    }
    return 0;
}
