// POJ 3290 - WFF 'N PROOF
// Model: gpt-6-astra
// Symbols may be freely reordered, not merely taken as a subsequence.
// A formula with b binary operators needs b+1 variables. Use as many
// binary operators as possible and every negation, forming a right chain.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main() {
    string s;
    while (cin >> s && s != "0") {
        string vars, ops, neg;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] >= 'p' && s[i] <= 't') vars += s[i];
            else if (s[i] == 'N') neg += s[i];
            else ops += s[i];
        }
        if (vars.empty()) { cout << "no WFF possible\n"; continue; }
        reverse(vars.begin(), vars.end());
        int b = min((int)ops.size(), (int)vars.size() - 1);
        for (int i = 0; i < b; ++i) cout << ops[i] << vars[i];
        cout << neg << vars[vars.size() - 1] << '\n';
    }
    return 0;
}
