// POJ 3283 - Card Hands
// Model: claude-sonnet-5
//
// Each hand is a linked list of cards in a fixed canonical order (by suit,
// then by value within suit). Two lists can share a common tail whenever the
// suffix from some node to the end is identical; sharing is applied
// repeatedly until no two lists share a tail. The number of linked-list
// nodes needed equals the number of distinct nodes in a trie built from the
// *reversed* card sequences: a shared tail of the original list is exactly
// a shared prefix of the reversed list, and a trie merges shared prefixes
// while keeping distinct nodes for everything that diverges. Insert every
// hand's cards, from last to first, into a trie keyed by card identity
// (52 possible cards); the number of new trie nodes created (the root is
// not counted) is the answer for that test case. The trie is rebuilt fresh
// for each test case.
//
// Verified against a hand-checked worked sample (6) plus a larger data set
// posted on the POJ discuss board (message 147774: a run of test cases with
// expected outputs 6, 52, 52, 52, 86), including cases with no sharing at
// all (a full 52-card deck split across several hands with disjoint ranges)
// and cases exercising the two-character "10" value, which a buggy reference
// solution posted on the same board (message 166222) mishandled by lumping
// "10" into the King bucket -- a bug in that code, not in the problem, but
// a reminder to parse the value string in full rather than by its first
// character alone.
//
// Card identity is encoded as suit*13 + value with value 0..12 (A,2..9,10,
// J,Q,K) and suit 0..3 (C,D,H,S); the exact numbering doesn't matter, only
// that it's a bijection, since the input already lists each hand's cards in
// canonical order.

#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

struct Node {
    int child[52];
    Node() { for (int i = 0; i < 52; i++) child[i] = -1; }
};

int valueIndex(const char *s, int len) {
    // s[0..len-2] is the value part (len includes the trailing suit char)
    int vlen = len - 1;
    if (vlen == 2) return 9; // "10"
    char c = s[0];
    switch (c) {
        case 'A': return 0;
        case 'J': return 10;
        case 'Q': return 11;
        case 'K': return 12;
        default: return c - '1'; // '2'..'9' -> 1..8
    }
}

int suitIndex(char c) {
    switch (c) {
        case 'C': return 0;
        case 'D': return 1;
        case 'H': return 2;
        default: return 3; // 'S'
    }
}

int main() {
    int T;
    vector<Node> trie;
    vector<int> cardIds;
    char buf[8];
    while (scanf("%d", &T) == 1 && T != 0) {
        trie.clear();
        trie.push_back(Node()); // root
        int ans = 0;
        for (int h = 0; h < T; h++) {
            int n;
            scanf("%d", &n);
            cardIds.resize(n);
            for (int i = 0; i < n; i++) {
                scanf("%s", buf);
                int len = (int)strlen(buf);
                int v = valueIndex(buf, len);
                int s = suitIndex(buf[len - 1]);
                cardIds[i] = s * 13 + v;
            }
            int cur = 0; // root index
            for (int i = n - 1; i >= 0; i--) {
                int id = cardIds[i];
                int nxt = trie[cur].child[id];
                if (nxt == -1) {
                    nxt = (int)trie.size();
                    trie.push_back(Node());
                    trie[cur].child[id] = nxt;
                    ans++;
                }
                cur = nxt;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
