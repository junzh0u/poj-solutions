// POJ 2568 - Decode the Tree
// Model: gpt-6-astra
// Repeatedly join the smallest remaining label absent from the remaining
// code to its next entry. Root the reconstructed tree at n and print children
// in label order. Each input line is a case, including the empty code for n=1.
// This variant has n-1 code entries, including the final surviving vertex n.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
void printTree(int u, int parent, const vector< vector<int> >& edges) {
    cout << '(' << u;
    for (size_t i = 0; i < edges[u].size(); ++i) {
        int v = edges[u][i];
        if (v != parent) {
            cout << ' ';
            printTree(v, u, edges);
        }
    }
    cout << ')';
}
int main() {
    string line;
    while (getline(cin, line)) {
        istringstream in(line);
        vector<int> code;
        int x;
        while (in >> x) code.push_back(x);
        int n = static_cast<int>(code.size()) + 1;
        vector<int> count(n + 1, 0), removed(n + 1, 0);
        vector< vector<int> > edges(n + 1);
        for (size_t i = 0; i < code.size(); ++i) ++count[code[i]];
        for (size_t i = 0; i < code.size(); ++i) {
            int leaf = 1;
            while (removed[leaf] || count[leaf]) ++leaf;
            int v = code[i];
            edges[leaf].push_back(v);
            edges[v].push_back(leaf);
            removed[leaf] = 1;
            --count[v];
        }
        for (int i = 1; i <= n; ++i) sort(edges[i].begin(), edges[i].end());
        printTree(n, 0, edges);
        cout << '\n';
    }
    return 0;
}
