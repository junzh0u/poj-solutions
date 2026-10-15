// POJ 1640 - Poly-polygonal Numbers
// Model: gpt-6.1-sol
// Merge the increasing k-gonal sequences with a minimum heap. Group all
// equal current values before advancing them, reporting groups of size >= 2.
// Whitespace separates input tokens; output indices are sorted explicitly.
// Values promised to fit signed 32-bit long; use double-width integers for
// the next term, which can temporarily exceed the promised answer bound.
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
struct Term {
    long long value, delta;
    int k;
    bool operator<(const Term& b) const { return value > b.value; }
};
int main() {
    int n;
    bool first = true;
    while (cin >> n && n) {
        vector<int> ks(n);
        int i, s;
        for (i = 0; i < n; ++i) cin >> ks[i];
        cin >> s;
        priority_queue<Term> q;
        for (i = 0; i < n; ++i) {
            Term t;
            t.value = 1; t.delta = ks[i] - 1; t.k = ks[i];
            while (t.value < s) {
                t.value += t.delta;
                t.delta += t.k - 2;
            }
            q.push(t);
        }
        if (!first) cout << '\n';
        first = false;
        int found = 0;
        while (found < 5) {
            long long v = q.top().value;
            vector<int> matches;
            do {
                Term t = q.top(); q.pop();
                matches.push_back(t.k);
                t.value += t.delta;
                t.delta += t.k - 2;
                q.push(t);
            } while (q.top().value == v);
            if (matches.size() >= 2) {
                sort(matches.begin(), matches.end());
                cout << v << ':';
                for (i = 0; i < (int)matches.size(); ++i) {
                    if (i) cout << ' ';
                    cout << matches[i];
                }
                cout << '\n';
                ++found;
            }
        }
    }
    return 0;
}
