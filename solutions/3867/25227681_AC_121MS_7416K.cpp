// POJ 3867 - Funny Language
// Model: gpt-6-astra
// Best-first traversal of the word-prefix tree. Appending a letter cannot
// increase the number of original words that can supply its multiset.
// Skip original strings as outputs, but expand them: their descendants and
// other permutations remain valid dictionary entries. Equal scores favor
// short words. At most m+n+1 nodes are expanded before n answers are found.
#include <iostream>
#include <string>
#include <set>
#include <queue>
#include <cstring>
using namespace std;
struct Node {
    string word;
    unsigned char count[26];
    unsigned int mask[32];
    int score;
    bool operator<(const Node& b) const {
        if (score != b.score) return score < b.score;
        if (word.size() != b.word.size()) return word.size() > b.word.size();
        return word > b.word;
    }
};
int supply[1000][26];
int main() {
    int n,m;
    if (!(cin >> n >> m)) return 0;
    set<string> forbidden;
    for (int i=0;i<m;++i) {
        string s; cin >> s; forbidden.insert(s);
        for (size_t j=0;j<s.size();++j) ++supply[i][s[j]-'A'];
    }
    priority_queue<Node> q;
    Node root;
    memset(root.count,0,sizeof(root.count));
    memset(root.mask,0,sizeof(root.mask));
    for (int i=0;i<m;++i) root.mask[i/32] |= 1u<<(i%32);
    root.score=m; q.push(root);
    while (n) {
        Node a=q.top(); q.pop();
        if (!a.word.empty() && !forbidden.count(a.word)) {
            cout << a.word << '\n';
            if (--n == 0) break;
        }
        for (int c=0;c<26;++c) {
            Node b=a; b.word += char('A'+c); ++b.count[c]; b.score=0;
            for (int i=0;i<m;++i) if (b.mask[i/32] & (1u<<(i%32))) {
                if (supply[i][c] < b.count[c]) b.mask[i/32] &= ~(1u<<(i%32));
                else ++b.score;
            }
            q.push(b);
        }
    }
    return 0;
}
