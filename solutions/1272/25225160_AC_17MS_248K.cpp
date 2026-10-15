// POJ 1272 - Synchronous Design
// Model: gpt-6.1-sol
// Topologically remove asynchronous dependencies, treating every other node
// as a clock boundary. Propagate longest delays without carrying them through
// synchronous nodes. Cycles take precedence over an exceeded clock period.
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int cases; cin >> cases;
    while (cases--) {
        double period; int n; cin >> period >> n;
        vector<char> type(n); vector<double> delay(n), dist(n,-1);
        vector<int> degree(n,0); vector< vector<int> > edge(n);
        for(int i=0;i<n;++i) { cin >> type[i] >> delay[i]; if(type[i]!='a') delay[i]=0; }
        int m; cin >> m;
        while(m--) { int u,v; cin >> u >> v; edge[u].push_back(v); if(type[v]=='a') ++degree[v]; }
        queue<int> q;
        for(int i=0;i<n;++i) if(!degree[i]) q.push(i);
        int removed=0; double maximum=0;
        while(!q.empty()) {
            int u=q.front(); q.pop(); ++removed;
            double value=type[u]=='a'?dist[u]:0;
            for(size_t j=0;j<edge[u].size();++j) {
                int v=edge[u][j];
                double candidate=value+delay[v];
                if(value>=0) maximum=max(maximum,candidate);
                if(type[v]=='a') { if(value>=0) dist[v]=max(dist[v],candidate); if(--degree[v]==0) q.push(v); }
            }
        }
        if(removed!=n) cout << "Circuit contains cycle.\n";
        else if(maximum>period) cout << "Clock period exceeded.\n";
        else cout << "Synchronous design. Maximum delay: " << fixed << setprecision(0) << maximum << ".\n";
    }
}
