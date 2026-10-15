// POJ 1796 - Extra Terrestrial PISA Test
// Model: gpt-6.1-sol
// Simulate long division, recording the first position of each remainder.
// The numerator has no size bound: parse it modulo the denominator.
// A zero remainder terminates; otherwise the repeated remainder starts the period.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return c - 'A' + 10;
}
int main() {
    int cases; cin >> cases;
    for (int cs = 1; cs <= cases; ++cs) {
        int b; string x, y; cin >> b >> x >> y;
        int den = 0;
        for (size_t i = 0; i < y.size(); ++i) den = den * b + digit(y[i]);
        int r = 0;
        for (size_t i = 0; i < x.size(); ++i)
            if (x[i] != '-' && x[i] != '+') r = (r * b + digit(x[i])) % den;
        vector<int> first(den, -1);
        int pos = 0;
        while (r != 0 && first[r] == -1) {
            first[r] = pos++;
            r = r * b % den;
        }
        int period = r == 0 ? 0 : pos - first[r];
        cout << "Scenario #" << cs << ":\n" << period << "\n\n";
    }
    return 0;
}
