// POJ 1867 - Treequivalence
// Model: gpt-6.1-sol
// Preserve the counter-clockwise cyclic order of incident edges.
// Compare one root of the first tree with every possible root of the second;
// root children may rotate, while a parent edge anchors all other child lists.
// Labels may repeat; they are attributes, not vertex identifiers.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Tree {
    vector<char> label;
    vector< vector<int> > adj;
    int parse(const string& s, int& p, int parent) {
        int u = label.size();
        label.push_back(s[p++]); adj.push_back(vector<int>());
        if (parent >= 0) adj[u].push_back(parent);
        if (p < (int)s.size() && s[p] == '(') {
            ++p;
            while (true) {
                int v = parse(s, p, u); adj[u].push_back(v);
                if (s[p++] == ')') break;
            }
        }
        return u;
    }
    string branch(int u, int parent) const {
        string ans(1,label[u]); ans += '(';
        int d = adj[u].size(), start = 0;
        while (adj[u][start] != parent) ++start;
        for (int k=1;k<d;++k) ans += branch(adj[u][(start+k)%d],u);
        return ans + ')';
    }
    string root(int u) const {
        int d = adj[u].size();
        vector<string> sub;
        for (int i=0;i<d;++i) sub.push_back(branch(adj[u][i],u));
        string best;
        for (int i=0;i<d;++i) {
            string s;
            for (int k=0;k<d;++k) s += sub[(i+k)%d];
            if (i==0 || s<best) best=s;
        }
        return string(1,label[u]) + '(' + best + ')';
    }
};
int main() {
    int t; cin >> t;
    while (t--) {
        string a,b; cin >> a >> b;
        Tree x,y; int p=0; x.parse(a,p,-1); p=0; y.parse(b,p,-1);
        bool same=false;
        if (x.label.size()==y.label.size()) {
            string target=x.root(0);
            for (int i=0;i<(int)y.label.size() && !same;++i)
                if (x.label[0]==y.label[i] && target==y.root(i)) same=true;
        }
        cout << (same ? "same" : "different") << '\n';
    }
    return 0;
}
