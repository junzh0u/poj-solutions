// POJ 2639 - Necklace Decomposition
// Model: gpt-5.6-terra
// Approach: Compute the Chen-Fox-Lyndon factorization with Duval's linear
// algorithm.  Consecutive identical Lyndon factors are combined: their
// concatenation is a (possibly non-primitive) necklace, while unequal
// adjacent factors must stay separate.

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static vector<string> decompose(const string& s) {
    vector<string> lyndon;
    int n = (int)s.size();
    int i = 0;
    while (i < n) {
        int j = i + 1;
        int k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else ++k;
            ++j;
        }
        int period = j - k;
        while (i <= k) {
            lyndon.push_back(s.substr(i, period));
            i += period;
        }
    }
    vector<string> factors;
    for (int p = 0; p < (int)lyndon.size();) {
        int q = p + 1;
        while (q < (int)lyndon.size() && lyndon[q] == lyndon[p]) ++q;
        string factor;
        for (int r = p; r < q; ++r) factor += lyndon[r];
        factors.push_back(factor);
        p = q;
    }
    return factors;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int cases;
    cin >> cases;
    while (cases--) {
        string s;
        cin >> s;
        vector<string> factors = decompose(s);
        for (int i = 0; i < (int)factors.size(); ++i)
            cout << '(' << factors[i] << ')';
        cout << '\n';
    }
    return 0;
}
