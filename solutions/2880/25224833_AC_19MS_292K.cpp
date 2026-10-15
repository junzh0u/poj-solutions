// POJ 2880 - Fixing Codes
// Model: gpt-6.1-sol
// Insert the incoming word into the fixed code's binary trie. A missing
// subtree costs zero, while sharing an existing leaf costs two appended
// bits. Descending one edge costs one bit on the incoming word. If an
// existing word is a proper prefix of the incoming word, extend that
// existing word in the opposite direction for a cost of one.
// Each input line is one case; its final word is the incoming word.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Node {
    int child[2]; bool terminal;
    Node() : terminal(false) { child[0] = child[1] = -1; }
};
vector<Node> trie;
int extra(int u) {
    if (u < 0) return 0;
    if (trie[u].terminal) return 2;
    return 1 + min(extra(trie[u].child[0]), extra(trie[u].child[1]));
}
int main() {
    int t; cin >> t;
    string line; getline(cin, line);
    while (t--) {
        getline(cin, line);
        istringstream in(line); vector<string> words; string s;
        while (in >> s) words.push_back(s);
        s = words.back(); words.pop_back();
        trie.clear(); trie.push_back(Node());
        for (unsigned i = 0; i < words.size(); ++i) {
            int u = 0;
            for (unsigned j = 0; j < words[i].size(); ++j) {
                int b = words[i][j] - '0';
                if (trie[u].child[b] < 0) {
                    trie[u].child[b] = trie.size(); trie.push_back(Node());
                }
                u = trie[u].child[b];
            }
            trie[u].terminal = true;
        }
        int u = 0, answer = -1;
        for (unsigned j = 0; j < s.size(); ++j) {
            if (trie[u].terminal) { answer = 1; break; }
            u = trie[u].child[s[j] - '0'];
            if (u < 0) { answer = 0; break; }
        }
        if (answer < 0) answer = words.empty() ? 0 : extra(u);
        cout << answer << '\n';
    }
    return 0;
}
