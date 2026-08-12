// POJ 2133 - Cow Imposters
// Model: gpt-5.6-terra
// Breadth-first search from all one-operation XORs, adding one input strip at
// a time. This also handles dependent cards: an input card might be remade in
// one XOR of two other cards.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int parseBits(const string& s) {
    int v = 0;
    for (int i = 0; i < (int)s.size(); ++i) v = (v << 1) | (s[i] - '0');
    return v;
}

int main() {
    int bits, e;
    if (!(cin >> bits >> e)) return 0;
    string s;
    cin >> s;
    const int goal = parseBits(s);
    vector<int> card;
    for (int i = 0; i < e; ++i) {
        cin >> s;
        int v = parseBits(s);
        if (find(card.begin(), card.end(), v) == card.end()) card.push_back(v);
    }
    const int limit = 1 << bits;
    const int inf = 1000000;
    vector<int> steps(limit, inf);
    vector<int> q;
    for (int i = 0; i < (int)card.size(); ++i) {
        for (int j = i; j < (int)card.size(); ++j) {
            int v = card[i] ^ card[j];
            if (steps[v] == inf) {
                steps[v] = 1;
                q.push_back(v);
            }
        }
    }
    for (int head = 0; head < (int)q.size(); ++head) {
        int cur = q[head];
        for (int i = 0; i < (int)card.size(); ++i) {
            int nxt = cur ^ card[i];
            if (steps[nxt] == inf) {
                steps[nxt] = steps[cur] + 1;
                q.push_back(nxt);
            }
        }
    }
    int best = 0, bestDiff = bits + 1, bestSteps = inf;
    for (int v = 0; v < limit; ++v) {
        if (steps[v] == inf) continue;
        int x = v ^ goal, diff = 0;
        while (x) { diff += x & 1; x >>= 1; }
        if (diff < bestDiff || (diff == bestDiff && (steps[v] < bestSteps || (steps[v] == bestSteps && v < best)))) {
            best = v; bestDiff = diff; bestSteps = steps[v];
        }
    }
    cout << bestSteps << '\n';
    for (int i = bits - 1; i >= 0; --i) cout << ((best >> i) & 1);
    cout << '\n';
    return 0;
}
