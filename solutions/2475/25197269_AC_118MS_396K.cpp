// POJ 2475 - Any fool can do it
// Model: gpt-5.6-terra
// Bottom-up recognition of Set, Element, and List over every substring.
// A one-character Element is always an Atom; longer Elements are Sets.
// The same punctuation can be either an Atom or structural syntax, so no
// greedy brace matching is valid.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

static bool isSet(const string &s) {
    int n = (int)s.size();
    bool set[201][201], element[201][201], list[201][201];
    memset(set, 0, sizeof(set));
    memset(element, 0, sizeof(element));
    memset(list, 0, sizeof(list));

    for (int len = 1; len <= n; ++len) {
        for (int left = 0; left + len <= n; ++left) {
            int right = left + len;
            if (len >= 2 && s[left] == '{' && s[right - 1] == '}' &&
                (len == 2 || list[left + 1][right - 1])) {
                set[left][right] = true;
            }
            if (len == 1 || set[left][right]) element[left][right] = true;
            if (element[left][right]) list[left][right] = true;
            for (int split = left + 1; split + 1 < right; ++split) {
                if (s[split] == ',' && element[left][split] &&
                    list[split + 1][right]) {
                    list[left][right] = true;
                }
            }
        }
    }
    return set[0][n];
}

int main() {
    int cases;
    cin >> cases;
    for (int tc = 1; tc <= cases; ++tc) {
        string word;
        cin >> word;
        cout << "Word #" << tc << ": " << (isSet(word) ? "Set" : "No Set") << '\n';
    }
    return 0;
}
