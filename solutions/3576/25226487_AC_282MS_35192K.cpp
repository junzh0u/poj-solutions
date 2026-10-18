// POJ 3576 - Language Recognition
// Model: gpt-6-astra
// Build a trie, then intern exact bottom-up state signatures consisting of
// the final-state flag and the equivalence classes of all 26 transitions.
// Equal signatures have equal right languages; distinct right languages
// require distinct DFA states. Missing transitions need no explicit sink.
#include <cstdio>
#include <map>
#include <algorithm>
using namespace std;
struct Signature {
    int a[27];
    bool operator<(const Signature& b) const {
        return lexicographical_compare(a, a+27, b.a, b.a+27);
    }
};
static Signature trie[150001];
static int cls[150001];
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    int nodes = 1;
    char word[32];
    for (int i=0; i<n; ++i) {
        scanf("%31s", word);
        int v=0;
        for (int j=0; word[j]; ++j) {
            int c=word[j]-'a';
            if (!trie[v].a[c]) trie[v].a[c]=nodes++;
            v=trie[v].a[c];
        }
        trie[v].a[26]=1;
    }
    map<Signature,int> classes;
    int count=0;
    for (int v=nodes-1; v>=0; --v) {
        Signature s=trie[v];
        for (int c=0; c<26; ++c)
            if (s.a[c]) s.a[c]=cls[s.a[c]];
        pair<map<Signature,int>::iterator,bool> p =
            classes.insert(make_pair(s,count+1));
        if (p.second) ++count;
        cls[v]=p.first->second;
    }
    printf("%d\n", count);
    return 0;
}
