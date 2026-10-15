// POJ 4039 - Guess the Numbers
// Model: gpt-6.1-sol
// Enumerate all permutations of the values and evaluate the fully parenthesized
// expression recursively. Each letter occurs once, so leaf order defines the
// assignment. The missing POJ statement is recovered from the SWERC 2011 booklet.
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
string expr;
int values[5], pos, leaf;
int evaluate() {
    if (expr[pos] != '(') { ++pos; return values[leaf++]; }
    ++pos;
    int a = evaluate();
    char op = expr[pos++];
    int b = evaluate();
    ++pos;
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    return a * b;
}
int main() {
    int n, target;
    while (cin >> n && n) {
        for (int i = 0; i < n; ++i) cin >> values[i];
        cin >> target >> expr;
        sort(values, values + n);
        bool found = false;
        do {
            pos = leaf = 0;
            if (evaluate() == target) { found = true; break; }
        } while (next_permutation(values, values + n));
        cout << (found ? "YES" : "NO") << '\n';
    }
    return 0;
}
