// POJ 1829 - 火力防御网
// Model: gpt-6.1-sol
// An edge to a subtree with k leaves costs C*k*k. Allocate leaves to
// the root's distinct connection choices by increasing marginal cost.
// The subtree cost for one leaf is zero (the child is the fort itself).
// A marginal depending on the not-yet-computed subtree cost is deferred.
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
typedef long long Int;
typedef pair<Int,int> Entry;
int main() {
    ios::sync_with_stdio(false);
    int tests; cin >> tests;
    while (tests--) {
        int n,m; cin >> n >> m;
        vector<Int> c(m), f(n+1,0);
        vector<int> k(m,0);
        priority_queue<Entry,vector<Entry>,greater<Entry> > q;
        for(int i=0;i<m;++i) { cin >> c[i]; q.push(Entry(c[i],i)); }
        if(n==1) { cout << q.top().first << '\n'; continue; }
        Int total=0;
        int deferred=-1;
        for(int t=1;t<=n;++t) {
            Entry e=q.top(); q.pop();
            total+=e.first;
            int i=e.second; ++k[i];
            if(t>1) f[t]=total;
            if(deferred!=-1) {
                int j=deferred;
                q.push(Entry(c[j]*(2*k[j]+1)+f[k[j]+1]-f[k[j]],j));
                deferred=-1;
            }
            if(k[i]+1<=t)
                q.push(Entry(c[i]*(2*k[i]+1)+f[k[i]+1]-f[k[i]],i));
            else deferred=i;
        }
        cout << f[n] << '\n';
    }
    return 0;
}
