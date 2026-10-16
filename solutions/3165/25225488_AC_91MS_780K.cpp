// POJ 3165 - Traveling Trio
// Model: gpt-6.1-sol
// Terminal-subset dynamic programming for directed trees, followed by a
// partition into tree components. With three demands on a DAG, an optimal
// union can be chosen as a forest: a necessary undirected cycle needs at
// least four directed demands (otherwise a branch can be rerouted/deleted).
// A subtree's crossing demands determine the direction of its root edge.
// Disconnected groups are allowed; in particular, a pair may bypass the third.
#include <cstdio>
#include <vector>
#include <queue>
#include <functional>
#include <utility>
#include <algorithm>
using namespace std;
const int INF=1000000;
static int dp[64][1001];
int main() {
    int n,m;
    while(scanf("%d%d",&n,&m)==2) {
        vector<vector<int> > out(n),in(n);
        for(int i=0;i+1<n;++i) {out[i].push_back(i+1);in[i+1].push_back(i);}
        for(int i=0;i<m;++i) {
            int a,b;scanf("%d%d",&a,&b);--a;--b;
            if(a!=b) {out[a].push_back(b);in[b].push_back(a);}
        }
        int terminal[6];
        for(int i=0;i<6;++i) {scanf("%d",&terminal[i]);--terminal[i];}
        for(int mask=0;mask<64;++mask) fill(dp[mask],dp[mask]+n,INF);
        for(int i=0;i<6;++i) dp[1<<i][terminal[i]]=0;
        int best[8];fill(best,best+8,INF);best[0]=0;
        for(int mask=1;mask<64;++mask) {
            for(int a=(mask-1)&mask;a;a=(a-1)&mask) {
                int b=mask^a;
                if(a>b) continue;
                for(int v=0;v<n;++v)
                    dp[mask][v]=min(dp[mask][v],dp[a][v]+dp[b][v]);
            }
            bool forward=false,backward=false;
            for(int i=0;i<3;++i) {
                bool s=(mask&(1<<i))!=0,e=(mask&(1<<(i+3)))!=0;
                if(s&&!e) forward=true;
                if(e&&!s) backward=true;
            }
            if(forward&&!backward) {
                for(int v=0;v<n;++v)
                    for(size_t j=0;j<out[v].size();++j) {
                        int w=out[v][j];dp[mask][w]=min(dp[mask][w],dp[mask][v]+1);
                    }
            } else if(backward&&!forward) {
                for(int v=n-1;v>=0;--v)
                    for(size_t j=0;j<in[v].size();++j) {
                        int w=in[v][j];dp[mask][w]=min(dp[mask][w],dp[mask][v]+1);
                    }
            } else if(!forward&&!backward) {
                typedef pair<int,int> P;
                priority_queue<P,vector<P>,greater<P> > q;
                for(int v=0;v<n;++v) if(dp[mask][v]<INF) q.push(P(dp[mask][v],v));
                while(!q.empty()) {
                    int cost=q.top().first,v=q.top().second;q.pop();
                    if(cost!=dp[mask][v]) continue;
                    for(int dir=0;dir<2;++dir) {
                        const vector<int>& adj=dir?in[v]:out[v];
                        for(size_t j=0;j<adj.size();++j) {
                            int w=adj[j];
                            if(dp[mask][w]>cost+1) {dp[mask][w]=cost+1;q.push(P(cost+1,w));}
                        }
                    }
                }
            }
        }
        for(int mask=1;mask<8;++mask)
            for(int a=mask;a;a=(a-1)&mask) {
                int val=*min_element(dp[a|(a<<3)],dp[a|(a<<3)]+n);
                best[mask]=min(best[mask],best[mask^a]+val);
            }
        printf("%d\n",best[7]);
    }
    return 0;
}
