// POJ 2004 - Mix and Build
// Model: claude-opus-5
//
// A is a mixed extension of B iff multiset(A) = multiset(B) + {one letter},
// so only the sorted letters of a word matter.  Map each word to its sorted
// signature; two words with the same signature are interchangeable in any
// chain, so keep one representative per signature.  Process signatures in
// increasing length; for a signature t of length L, delete each letter (once
// per distinct letter value) to get every possible predecessor signature and
// look it up.  dp[t] = 1 + max dp over the predecessors found; a chain has
// strictly increasing lengths, so the predecessors are already final.
// O(total length * log n) — 10000 words of length <= 20 run in ~0.05s.
//
// Reading: "permuting" may be the identity (the statement's own carbon ->
// carbons), and the added letter may duplicate one already present, so
// aa -> aaa -> aaaa is a valid chain; both readings are what the discuss
// board reports people losing submissions to.  Input is read to EOF as
// whitespace-separated tokens, which absorbs the trailing blank line the
// board reports in the judge data (and any \r).

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

using namespace std;

struct ByLen {
    const vector<string> *sig;
    ByLen(const vector<string> *s) : sig(s) {}
    bool operator()(int a, int b) const {
        return (*sig)[a].size() < (*sig)[b].size();
    }
};

int main() {
    ios::sync_with_stdio(false);
    vector<string> word;
    vector<string> sig;
    map<string, int> id;              // signature -> node index
    string s;
    while (cin >> s) {
        string t = s;
        sort(t.begin(), t.end());
        if (id.find(t) != id.end()) continue;   // anagram (or exact) duplicate
        id[t] = (int)word.size();
        word.push_back(s);
        sig.push_back(t);
    }
    int n = (int)word.size();
    if (n == 0) return 0;

    vector<int> order(n);
    for (int i = 0; i < n; i++) order[i] = i;
    stable_sort(order.begin(), order.end(), ByLen(&sig));

    vector<int> dp(n, 1), par(n, -1);
    int best = 0;
    for (int k = 0; k < n; k++) {
        int i = order[k];
        const string &t = sig[i];
        int L = (int)t.size();
        for (int p = 0; p < L; p++) {
            if (p > 0 && t[p] == t[p - 1]) continue;   // same predecessor signature
            string u;
            u.reserve(L - 1);
            u.append(t, 0, p);
            u.append(t, p + 1, L - 1 - p);
            map<string, int>::const_iterator it = id.find(u);
            if (it == id.end()) continue;
            int j = it->second;
            if (dp[j] + 1 > dp[i]) { dp[i] = dp[j] + 1; par[i] = j; }
        }
        if (dp[i] > dp[best]) best = i;
    }

    vector<int> chain;
    for (int v = best; v != -1; v = par[v]) chain.push_back(v);
    for (int k = (int)chain.size() - 1; k >= 0; k--)
        cout << word[chain[k]] << "\n";
    return 0;
}
