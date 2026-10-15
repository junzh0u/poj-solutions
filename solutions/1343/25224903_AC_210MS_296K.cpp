// POJ 1343 - Rooted Trees Isomorphism
// Model: gpt-6.1-sol
// Sort child encodings recursively and take the least encoding over all roots.
// Despite the title and directed input pairs, isomorphism is unrooted and
// ignores edge direction, as clarified on the discussion board.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
vector<int> adj[101];
string encode(int u, int parent) {
    vector<string> children;
    for (size_t i = 0; i < adj[u].size(); ++i)
        if (adj[u][i] != parent) children.push_back(encode(adj[u][i], u));
    sort(children.begin(), children.end());
    string result = "(";
    for (size_t i = 0; i < children.size(); ++i) result += children[i];
    return result + ")";
}
int main() {
    int k, n;
    if (!(cin >> k >> n)) return 0;
    vector<string> keys;
    vector< vector<int> > groups;
    for (int t = 1; t <= k; ++t) {
        for (int i = 1; i <= n; ++i) adj[i].clear();
        for (int i = 1; i < n; ++i) {
            int a, b; cin >> a >> b;
            adj[a].push_back(b); adj[b].push_back(a);
        }
        string best = encode(1, 0);
        for (int i = 2; i <= n; ++i) best = min(best, encode(i, 0));
        size_t g = 0;
        while (g < keys.size() && keys[g] != best) ++g;
        if (g == keys.size()) { keys.push_back(best); groups.push_back(vector<int>()); }
        groups[g].push_back(t);
    }
    for (size_t g = 0; g < groups.size(); ++g) {
        for (size_t j = 0; j < groups[g].size(); ++j) {
            if (j) cout << " = ";
            cout << groups[g][j];
        }
        cout << " ;\n";
    }
    return 0;
}
