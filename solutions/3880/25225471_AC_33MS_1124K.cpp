// POJ 3880 - Routing
// Model: gpt-6.1-sol
// Color connections so the two ports of every boundary switch use
// opposite subnetworks. Each component has two colorings; start its
// earliest top port with color zero to minimize the first row.
// Recurse independently in both subnetworks: O(n * 2^n) time.
// The output has 2*n-1 rows (the statement's row count is a typo).
#include <iostream>
#include <vector>
#include <string>
using namespace std;
vector<string> ans;
void solve(const vector<int>& p, int n, int row, int col) {
    int size = (int)p.size(), half = size/2;
    if (n == 1) { ans[row][col] = char('0' + p[0]); return; }
    vector<int> inv(size), color(size, -1), stack;
    for (int i=0;i<size;++i) inv[p[i]]=i;
    for (int i=0;i<size;++i) if (color[i]<0) {
        color[i]=0; stack.push_back(i);
        while (!stack.empty()) {
            int u=stack.back(); stack.pop_back();
            int v[2] = {u^1, p[inv[u]^1]};
            for (int j=0;j<2;++j) if (color[v[j]]<0) {
                color[v[j]]=color[u]^1; stack.push_back(v[j]);
            }
        }
    }
    vector<int> left(half), right(half);
    for (int i=0;i<half;++i) {
        ans[row][col+i]=char('0'+color[2*i]);
        ans[row+2*n-2][col+i]=char('0'+color[p[2*i]]);
    }
    for (int i=0;i<size;++i) {
        if (color[p[i]]==0) left[i/2]=p[i]/2;
        else right[i/2]=p[i]/2;
    }
    solve(left,n-1,row+1,col);
    solve(right,n-1,row+1,col+half/2);
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int n; bool first=true;
    while (cin>>n && n) {
        int size=1<<n; vector<int> p(size);
        for (int i=0;i<size;++i) cin>>p[i];
        ans.assign(2*n-1,string(size/2,'0'));
        solve(p,n,0,0);
        if (!first) cout<<'\n'; first=false;
        for (int i=0;i<2*n-1;++i) cout<<ans[i]<<'\n';
    }
    return 0;
}
